// ReSharper disable CppInconsistentNaming
#pragma once
#include <numeric>
#include "cheat/module/module.hpp"

class AutoClicker : public Module
{
private:
    std::atomic<int> ComboCount = 0;
    std::atomic<DWORD> LastAttackTime = 0;
    RandomNumberGenerator Random;
public:

    enum HITSELECT_MODE
    {
        BURST = 0,
        SINGLE = 1
    };
    //ONPRE MOTION

    inline static float MinCpsRange = 4.F;
    inline static float MaxCpsRange = 4.F;
    inline static float OutlierChance = 0.03F;
    inline static float SpikeChance = 0.5F;
    inline static float DropChance = 0.5F;
    inline static float Kurtosis = 1.0F;
    inline static float StdDev = 1.0F;
    inline static float DoubleClickChance = 0.03F;
    inline static float OutlierRange = 5.F;
    inline static float OutlierCoolDown = 5.F;
    inline static float OutlierSteps = 10.F;
    inline static float ChangeChance = 0.9F;

    inline static bool HitSelect = false;
    inline static bool FakeSwing = false;
    inline static bool FilterAir = true;
    inline static int CancelRate = 100;
    inline static int ComboThreshold = 1;
    inline static int AttackWindow = 2;

    inline static bool OnlyClicking = true;
    inline static bool AllowInventory = true;
    inline static bool NoHitDelay = true;
    inline static bool BreakBlocks = true;
    inline static bool PreventUnrefill = true;
    inline static bool FavorCriticals = true;

    inline static float TargetCPS = 15.F;

    inline static bool ItemWhitelist = false;//done

    inline static bool WhitelistSwords = false;//
    inline static bool WhitelistAxes = false;//
    inline static bool WhitelistFist = false;//
    inline static bool WhitelistSticks = false;//
    inline static bool WhitelistPickAxe = false;//

	virtual void Run(JNIEnv* Env) override;
    virtual void OnHandleStatusUpdate(JNIEnv* Env, const std::shared_ptr<Player>& TargetInstance, jbyte Byte, bool* ShouldCancel) override;
};


// Function to split the dataset into given number of parts
inline std::vector<std::vector<float>> splitDataset(const std::vector<float>& dataset, int numParts) {
    std::vector<std::vector<float>> parts(numParts);
    int partSize = dataset.size() / numParts;
    int remainder = dataset.size() % numParts;
    int index = 0;

    for (int i = 0; i < numParts; ++i) {
        int partLength = partSize + (i < remainder ? 1 : 0);
        parts[i].insert(parts[i].begin(), dataset.begin() + index, dataset.begin() + index + partLength);
        index += partLength;
    }

    return parts;
}

// Function to combine and shuffle the parts
inline std::vector<float> combineAndShuffle(const std::vector<std::vector<float>>& parts) {
    std::vector<float> combined;
    for (const auto& part : parts) {
        combined.insert(combined.end(), part.begin(), part.end());
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(combined.begin(), combined.end(), g);

    return combined;
}

namespace clicker_utils
{
    inline std::random_device device = std::random_device();
    inline std::mt19937 engine = std::mt19937(device());

    struct generation_offsets
    {
        double mean;
        double std_dev;
        double rythmic_patterns;
    };

    void add_outliers(std::vector<double>& data);

    double GetNextClick();

    std::vector<double> generate_log_normal_vector(int size, double average, double lower_bound, double upper_bound);
    std::vector<double> heavy_tailed_distribution(const std::vector<double>& data, int outlier_cout);
    std::vector<double> generate_delay_vector(const generation_offsets& offsets, const int num_delays);
}

inline std::vector<double> clicker_utils::generate_log_normal_vector(const int size, const double average, const double lower_bound, const double upper_bound)
{
    std::vector<double> result;
    result.reserve(size);

    unsigned seed = std::random_device()();
    std::default_random_engine generator(seed);

    double mu = log(average) - 0.5 * log(1 + pow((upper_bound - lower_bound) / (2 * average), 2));
    double sigma = sqrt(log(1 + pow((upper_bound - lower_bound) / (2 * average), 2)));

    std::lognormal_distribution<double> distribution(mu, sigma);

    for (int i = 0; i < size; i++) {
        double value;
        do {
            value = distribution(generator);
        } while (value < lower_bound || value > upper_bound);
        result.push_back(value);
    }

    return result;
}

inline void clicker_utils::add_outliers(std::vector<double>& data)
{
    double sum = 0.0;
    double sumSquared = 0.0;

    for (double value : data) {
        sum += value;
        sumSquared += value * value;
    }

    double mean = sum / data.size();
    double variance = (sumSquared / data.size()) - (mean * mean);
    double standardDeviation = sqrt(variance);

    double outlierMean = 3.0 * standardDeviation; // Mean of outliers
    double outlierStdDev = 0.5 * standardDeviation; // Standard deviation of outliers

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(outlierMean, outlierStdDev);

    int numOutliers = 5; // You can adjust the number of outliers as needed
    for (int i = 0; i < numOutliers; ++i) {
        double outlierValue = distribution(generator);
        data.push_back(outlierValue);
    }
}

inline double clicker_utils::GetNextClick()
{
    static int index;
    static float last_average;
    static std::vector<double> generated_delays{};

    RandomNumberGenerator rng;

    auto average_cps = ( AutoClicker::TargetCPS);

    if (generated_delays.empty() || index >= static_cast<int>(generated_delays.size()) || last_average != average_cps)
    {
        generated_delays.clear();

        const auto offsets = clicker_utils::generation_offsets((1000.F / average_cps), 32.4985428736089, 12.8359667493512F);
        const auto pre_distributed_data = generate_delay_vector(offsets, rng.GenerateInt(250, 500));
        generated_delays = clicker_utils::heavy_tailed_distribution(pre_distributed_data, static_cast<int>(pre_distributed_data.size() * rng.GenerateFloat(0.1F, 0.3F)));

        last_average = average_cps;
        index = 0;
    }

    const auto final_delay = std::abs(generated_delays[index++]);

    return final_delay;
}

inline std::vector<double> clicker_utils::heavy_tailed_distribution(const std::vector<double>& data, const int outlier_cout)
{
    const double data_min = *std::ranges::min_element(data);
    const double data_max = *std::ranges::max_element(data);

    const double outlier_min = data_min - (data_max - data_min);
    const double outlier_max = data_max + (data_max - data_min);

    std::vector<double> outliers;
    std::uniform_real_distribution<double> dist(outlier_min, outlier_max);

    while (outliers.size() < outlier_cout)
    {
        constexpr double exponent = 1.0;
        const double value = dist(engine);
        const double random = static_cast<double>(rand()) / RAND_MAX;
        double weighted_value = std::pow(value, exponent) * random;

        outliers.push_back(weighted_value);
    }

    std::vector<double> result = data;
    result.insert(result.end(), outliers.begin(), outliers.end());

    return result;
}

inline double calculate_rhythmic_pattern(const std::vector<double>& values, const int period)
{
    const int num_values = values.size();
    double sum = 0.0;
    for (int i = 0; i < num_values; i++)
    {
        const double t = static_cast<double>(i) / num_values;
        const double rhythmic_component = std::sin(2.0 * M_PI * t / period);
        sum += values[i] * rhythmic_component;
    }
    return sum / num_values;
}


inline std::vector<double> clicker_utils::generate_delay_vector(const generation_offsets& offsets, const int num_delays)
{
    const double original_mean = offsets.mean;
    const double original_std_dev = offsets.std_dev;
    constexpr int original_period = 10;
    const double original_rhythmic_pattern = offsets.rythmic_patterns;

    std::vector<double> generated_delays;
    std::normal_distribution<> dist(original_mean, original_std_dev);

    for (int j = 0; j < num_delays; j++)
    {
        double delay = dist(engine);
        generated_delays.push_back(delay);
    }

    const double generated_rhythmic_pattern = calculate_rhythmic_pattern(generated_delays, original_period);
    const double rhythmic_pattern_difference = original_rhythmic_pattern - generated_rhythmic_pattern;
    for (int j = 0; j < num_delays; j++)
    {
        const double t = static_cast<double>(j) / num_delays;
        const double rhythmic_component = std::sin(2.0 * M_PI * t / original_period);
        generated_delays[j] += rhythmic_pattern_difference * rhythmic_component;
    }

    return generated_delays;
}