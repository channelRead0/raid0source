#include "pch.h"
#include "clicker.hpp"
#include <src/cheat/module/modules/combat/autoclicker.hpp>

double ClickerEngine::CalculateRhythmicPattern(const std::vector<double>& Data, const int Period)
{
    const int DataSize = Data.size();
    double Sum = 0.0;
    for (int Index = 0; Index < DataSize; Index++)
    {
        const double T = static_cast<double>(Index) / DataSize;
        const double Component = std::sin(2.0 * M_PI * T / Period);
        Sum += Data[Index] * Component;
    }
    return Sum / DataSize;
}

std::vector<double> ClickerEngine::HeavyTailedDistribution(std::vector<double>& Data, int OutlierCount)
{
    const double DataMin = *std::ranges::min_element(Data);
    const double DataMax = *std::ranges::max_element(Data);

    const double OutlierMin = DataMin - (DataMax - DataMin);
    const double OutlierMax = DataMax + (DataMax - DataMin);

    static std::random_device Device = std::random_device();
    static std::mt19937 Engine = std::mt19937(Device());

    std::vector<double> Outliers;
    std::uniform_real_distribution<double> Distribution(OutlierMin, OutlierMax);

    while (Outliers.size() < OutlierCount)
    {
        constexpr double Expo = 1.0;
        const double Random = static_cast<double>(rand()) / RAND_MAX;
        double WeightedExpo = std::pow(Distribution(Engine), Expo) * Random;

        Outliers.push_back(WeightedExpo);
    }

    std::vector<double> Result = Data;
    Result.insert(Result.end(), Outliers.begin(), Outliers.end());

    return Result;
}

std::vector<double> ClickerEngine::GenerateDelayVector(const float Std, const float Rtpm, const float Mean, int Count)
{
    std::vector<double> GeneratedBuffer;
    std::uniform_real_distribution<> Distribution(Mean, Std);

    static std::random_device Device = std::random_device();
    static std::mt19937 Engine = std::mt19937(Device());

    for (int Index = 0; Index < Count; Index++)
    {
        double Delay = Distribution(Engine);
        GeneratedBuffer.push_back(Delay);
    }

    const double RhythmicPattern = CalculateRhythmicPattern(GeneratedBuffer, 10);
    const double RhythmicPatternDelta = Rtpm - RhythmicPattern;
    for (int Index = 0; Index < Count; Index++)
    {
        const double T = static_cast<double>(Index) / Count;
        const double RhythmicComponent = std::sin(2.0 * M_PI * T / 10);
        GeneratedBuffer[Index] += RhythmicPatternDelta * RhythmicComponent;
    }

    return GeneratedBuffer;
}

float ClickerEngine::GenerateDelay()
{
    float MinCpsRange = AutoClicker::MinCpsRange;
    float MaxCpsRange = AutoClicker::MaxCpsRange;

    float OutlierChance = AutoClicker::OutlierChance;
    float SpikeChance = AutoClicker::SpikeChance;
    float DropChance = AutoClicker::DropChance;
    float DoubleClickChance = AutoClicker::DoubleClickChance;
    float ChangeChance = AutoClicker::ChangeChance;

    float TargetCPS = AutoClicker::TargetCPS;
    float LowerBoundCPS = TargetCPS - MinCpsRange;
    float UpperBoundCPS = TargetCPS + MaxCpsRange;

    float PreviousCPS = TargetCPS;
    float GeneratedCPS = TargetCPS;

    std::vector<float> ClickDelaysPass1;

    int Previous = TargetCPS;
    bool PreviousDoubleClick = false;
    bool PreviousOutlier = false;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);  // Standard normal distribution
    std::uniform_real_distribution<> u(0.0, 1.0); // Uniform distribution for drop, spike, and outliers

    // Fleishman coefficients
    double b = (std::sqrt(1 + AutoClicker::Kurtosis) - 1) / 2;
    double a = -b;
    double c = std::sqrt(2 * b + 1);

    auto Delay = Random.GenerateFloatNormal(TargetCPS - MinCpsRange, TargetCPS + MaxCpsRange);

    double u_val = d(gen);
    Delay = a + b * u_val + c * u_val * u_val;
    Delay = (TargetCPS) + AutoClicker::StdDev * Delay;

    while ((int)Delay == (int)Previous && !INSTANCE_DESTRUCT)
    {
        if (Random.GenerateFloat(0.F, 1.F) < (1.F - ChangeChance)) {
            break;
        }

        if (Random.GenerateFloat(0.F, 1.F) < DropChance)
        {
            //DROP
            while ((int)Delay > (int)Previous && !INSTANCE_DESTRUCT)
            {
                double u_val = d(gen);
                Delay = a + b * u_val + c * u_val * u_val;
                Delay = (TargetCPS)+AutoClicker::StdDev * Delay;
            }
        }
        else
        {
            while ((int)Delay > (int)Previous && !INSTANCE_DESTRUCT)
            {
                double u_val = d(gen);
                Delay = a + b * u_val + c * u_val * u_val;
                Delay = (TargetCPS)+AutoClicker::StdDev * Delay;
            }
        }

        double u_val = d(gen);
        Delay = a + b * u_val + c * u_val * u_val;
        Delay = (TargetCPS)+AutoClicker::StdDev * Delay;
    }

    if (Random.GenerateFloat(0.F, 1.F) < OutlierChance && !PreviousOutlier)
    {
        Delay = Random.GenerateFloatNormal(3.F, 5.F);
        Previous = Delay;

        PreviousOutlier = true;

        return 1000.F / Delay;
    }
    else
    {
        PreviousOutlier = false;
    }

    if (Random.GenerateFloat(0.F, 1.F) < DoubleClickChance && !PreviousDoubleClick)
    {
        Delay = Previous;

        PreviousDoubleClick = true;

        return  1000.F / Delay;
    }
    else
    {
        PreviousDoubleClick = false;
    }

    Previous = Delay;
    return 1000.F / Delay;
}

std::vector<double> generate_synthetic_data_b(double mean, double stddev, double kurtosis, double rate_of_change, double drop_rate, double spike_rate, double outlier_chance, double outlier_range, size_t num_samples) {
    // Random device and Mersenne Twister engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);  // Standard normal distribution
    std::uniform_real_distribution<> u(0.0, 1.0); // Uniform distribution for drop, spike, and outliers

    // Fleishman coefficients
    double b = (std::sqrt(1 + kurtosis) - 1) / 2;
    double a = -b;
    double c = std::sqrt(2 * b + 1);


    static RandomNumberGenerator Random;
    // Generate initial data
    size_t generations_since_outlier = 0;
    std::vector<double> data(num_samples);
    for (size_t i = 0; i < num_samples; ++i) {
        double u_val = d(gen);
        data[i] = a + b * u_val + c * u_val * u_val;
        data[i] = mean + stddev * data[i];  // Scale and shift to desired mean and stddev

        // Apply outlier chance (negative outliers only)
        if (u(gen) < outlier_chance && generations_since_outlier > AutoClicker::OutlierCoolDown) {
            data[i] = 1000.0 / Random.GenerateFloatNormal(outlier_range, outlier_range + 2.F);
            generations_since_outlier = 0; // Reset the counter
        }
        else {
            generations_since_outlier++;
        }
        //if (u(gen) < outlier_chance && generations_since_outlier > AutoClicker::OutlierCoolDown) {
        //    double outlier_value = 0.75 * outlier_range + (u(gen) * (outlier_range - 0.75 * outlier_range));
        //    double step_size = (outlier_value - data[i]) / AutoClicker::OutlierSteps;
        //    for (size_t step = 0; step < AutoClicker::OutlierSteps; ++step) {
        //        data[i] += step_size;
        //        if (i + 1 < num_samples) {
        //            data[i + 1] = 1000.F / step_size; // Propagate the outlier transition to the next data point
        //        }
        //    }
        //    generations_since_outlier = 0; // Reset the counter
        //}
        //else {
        //    generations_since_outlier++;
        //}
    }

    // Apply rate of change, drop rate, and spike rate
    std::vector<double> result(num_samples);
    result[0] = data[0];
    for (size_t i = 1; i < num_samples; ++i) {
        if (u(gen) < rate_of_change) {
            result[i] = data[i];
        }
        else {
            result[i] = result[i - 1];
        }

        // Apply drop rate
        if (u(gen) < drop_rate) {
            result[i] = (((result[i]) < (result[i - 1] - 1.0)) ? (result[i]) : (result[i - 1] - 1.0));
        }

        // Apply spike rate
        if (u(gen) < spike_rate) {
            result[i] = (((result[i]) > (result[i - 1] + 1.0)) ? (result[i]) : (result[i - 1] + 1.0));
        }
    }

    return result;
}

// Function to generate synthetic data with specified parameters
std::vector<double> generate_synthetic_data(double mean, double stddev, double kurtosis, double rate_of_change, double drop_rate, double spike_rate, size_t num_samples) {
    // Random device and Mersenne Twister engine
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);  // Standard normal distribution
    std::uniform_real_distribution<> u(0.0, 1.0); // Uniform distribution for drop and spike

    // Fleishman coefficients
    double b = (std::sqrt(1 + kurtosis) - 1) / 2;
    double a = -b;
    double c = std::sqrt(2 * b + 1);

    // Generate data
    std::vector<double> data(num_samples);
    for (size_t i = 0; i < num_samples; ++i) {
        double u = d(gen);
        std::uniform_real_distribution<> uA(0.0, 1.0); // Uniform distribution for drop and spike
        data[i] = a + b * u + c * u * u;
        data[i] = mean + stddev * data[i];  // Scale and shift to desired mean and stddev

        // Apply drop rate
        if (i > 0 && uA(gen) < drop_rate) {
            data[i] = (((data[i]) < (data[i - 1] - 1)) ? (data[i]) : (data[i - 1] - 1));
        }

        // Apply spike rate
        if (i > 0 && uA(gen) < spike_rate) {
            data[i] = (((data[i]) > (data[i - 1] + 1)) ? (data[i]) : (data[i - 1] + 1));
        }
    }

    // Apply rate of change
    std::vector<double> result(num_samples);
    result[0] = data[0];
    for (size_t i = 1; i < num_samples; ++i) {
        if (u(gen) < rate_of_change) {
            result[i] = data[i];
        }
        else {
            result[i] = result[i - 1];
        }
    }

    return result;
}


float ClickerEngine::GenerateDelayExp()
{
    return 0.0f;
}

int ClickerEngine::GetNextDelay(float TargetCPS)
{
    static int Index = 0;
    static float LastTarget;

    const auto Mean = 1000.0 / TargetCPS;

    float MinCpsRange = AutoClicker::MinCpsRange;
    float MaxCpsRange = AutoClicker::MaxCpsRange;

    float OutlierChance = AutoClicker::OutlierChance;
    float SpikeChance = AutoClicker::SpikeChance;
    float DropChance = AutoClicker::DropChance;
    float DoubleClickChance = AutoClicker::DoubleClickChance;
    float ChangeChance = AutoClicker::ChangeChance;


    if (DelayBuffer.empty() || Index >= static_cast<int>(DelayBuffer.size()) || LastTarget != TargetCPS)
    {
        DelayBuffer.clear();

        auto DelayVectorPreSkew = generate_synthetic_data_b(Mean, AutoClicker::StdDev, AutoClicker::Kurtosis, ChangeChance, DropChance, SpikeChance, OutlierChance, AutoClicker::OutlierRange, 100);
        DelayBuffer =/* HeavyTailedDistribution(DelayVectorPreSkew, static_cast<int>(DelayVectorPreSkew.size() * Random.GenerateFloat(0.1F, 0.3F)))*/DelayVectorPreSkew;

        LastTarget = TargetCPS;
        Index = 0;
    }

    return std::abs(DelayBuffer[Index++]);
}

ClickerEngine* ClickerEnginePtr = new ClickerEngine();