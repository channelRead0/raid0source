#pragma once
#include <vector>
#include <globals/utilities/maths/random/rng.hpp>

class ClickerEngine
{
private:
	RandomNumberGenerator Random;

	std::vector<double> DelayBuffer;

	double CalculateRhythmicPattern(const std::vector<double>& Data, const int Period);
	std::vector<double> HeavyTailedDistribution(std::vector<double>& Data, int OutlierCount);
	std::vector<double> GenerateDelayVector(const float Std, const float Rtpm, const float Mean, int Count);
public:
	int GetNextDelay(float TargetCPS);
	float GenerateDelay();
	float GenerateDelayExp();
};

extern ClickerEngine* ClickerEnginePtr;