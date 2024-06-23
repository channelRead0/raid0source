#pragma once
#include <random>

class RandomNumberGenerator
{
public:
    RandomNumberGenerator() : RandomGenerator(RandomDevice()) {}

    int GenerateInt(const int Minimum, const int Maximum)
    {
        std::uniform_int_distribution<int> Distribution(Minimum, Maximum);
        return Distribution(RandomGenerator);
    }

    double GenerateDouble(const double Minimum, const double Maximum)
    {
        std::uniform_real_distribution<double> Distribution(Minimum, Maximum);
        return Distribution(RandomGenerator);
    }

    float GenerateFloat(const float Minimum, const float Maximum)
    {
        std::uniform_real_distribution<float> Distribution(Minimum, Maximum);
        return Distribution(RandomGenerator);
    }

    bool GenerateBool()
    {
        std::bernoulli_distribution Distribution(0.5);
        return Distribution(RandomGenerator);
    }

    std::string GenerateString(int Length)
    {
        std::string RandomString;

        const std::string Charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        srand(time(0));

        for (int i = 0; i < Length; ++i) {
            RandomString += Charset[rand() % Charset.length()];
        }

        return RandomString;
    }

    void GenerateString(int Length, std::string* String)
    {
        VMProtectBeginUltra("KeyGen");

        std::string RandomString;

        const std::string Charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        srand(time(0));

        for (int i = 0; i < Length; ++i) {
            RandomString += Charset[rand() % Charset.length()];
        }

        *String = RandomString;

        VMProtectEnd();
    }

private:
    std::random_device RandomDevice;
    std::mt19937 RandomGenerator;
};