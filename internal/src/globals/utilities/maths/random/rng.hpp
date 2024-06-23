#pragma once

#define NOMINMAX
#include <random>
#include <algorithm>

class RandomNumberGenerator
{
public:
    RandomNumberGenerator() {}

    int GenerateInt(const int Minimum, const int Maximum)
    {
        static std::random_device Device = std::random_device();
        static std::mt19937 Engine = std::mt19937(Device());

        std::uniform_int_distribution<int> Distribution(Minimum, Maximum);
        return Distribution(Engine);
    }

    double GenerateNormalClamp(double mean = 0.0, double stddev = 1.0)
    {
        static std::random_device Device = std::random_device();
        static std::mt19937 Engine = std::mt19937(Device());
        std::normal_distribution<double> distribution(mean, stddev);

        double number = distribution(Engine);

        // -inf to inf clamp within ±3 std dev
        number = ((((((number) < (mean + 3 * stddev)) ? (number) : (mean + 3 * stddev))) > (mean - 3 * stddev)) ? ((((number) < (mean + 3 * stddev)) ? (number) : (mean + 3 * stddev))) : (mean - 3 * stddev));

        double normalized = (number - (mean - 3 * stddev)) / (6 * stddev);

        // clamp within bounds
        if (normalized < 0.0) normalized = 0.0;
        if (normalized >= 1.0) normalized = std::nextafter(1.0, 0.0);

        return normalized;
    }

    float GenerateFloatNormal(const float Minimum, const float Maximum, const int Chance = 100)
    {
        if (GenerateInt(0, 100) > Chance) {
            return (Minimum + Maximum) / 2.0;
        }

        return Minimum + ((Maximum - Minimum) * GenerateNormalClamp());
    }

    double GenerateDouble(const double Minimum, const double Maximum, const int Chance = 100)
    {
        if (GenerateInt(0, 100) > Chance) {
            return (Minimum + Maximum) / 2.0;
        }

        static std::random_device Device = std::random_device();
        static std::mt19937 Engine = std::mt19937(Device());

        std::uniform_real_distribution<double> Distribution(Minimum, Maximum);
        return Distribution(Engine);
    }

    float GenerateFloat(const float Minimum, const float Maximum, const int Chance = 100)
    {
        if (GenerateInt(0, 100) > Chance) {
            return (Minimum + Maximum) / 2.0;
        }

        static std::random_device Device = std::random_device();
        static std::mt19937 Engine = std::mt19937(Device());

        std::uniform_real_distribution<float> Distribution(Minimum, Maximum);
        return Distribution(Engine);
    }

    bool GenerateBool()
    {
        static std::random_device Device = std::random_device();
        static std::mt19937 Engine = std::mt19937(Device());

        std::bernoulli_distribution Distribution(0.5);
        return Distribution(Engine);
    }

private:
};