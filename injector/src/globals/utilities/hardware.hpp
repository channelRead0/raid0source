#pragma once
#include "pch.h"
#include <string>
#include <intrin.h>
#include <wtypes.h>
#include <ostream>
#include <sstream>
#include <VMProtectSDK.h>

namespace HardwareUtilities
{
	inline std::string GetHardwareIdentifier()
	{
		VMProtectBegin("HWID");

		int CpuInfoRegisters[4];
		__cpuid(CpuInfoRegisters, 1);

		const int Family = ((CpuInfoRegisters[0] >> 8) & 0xF) + ((CpuInfoRegisters[0] >> 20) & 0xFF);
		const int Model = ((CpuInfoRegisters[0] >> 4) & 0xF) | ((CpuInfoRegisters[0] >> 12) & 0xF0);
		const int Stepping = CpuInfoRegisters[0] & 0xF;

		const std::string CpuIdentifier = std::to_string(Family) + "-" + std::to_string(Model) + "-" + std::to_string(Stepping);

		char WindowsDirectory[MAX_PATH];
		GetWindowsDirectory(WindowsDirectory, MAX_PATH);
		std::string DrivePath = WindowsDirectory;
		DrivePath = DrivePath.substr(0, DrivePath.find('\\') + 1);

		DWORD Dvs = 0;
		if (!GetVolumeInformation(DrivePath.c_str(), nullptr, NULL, &Dvs, nullptr, nullptr, nullptr, NULL)) {
			Dvs = -1;
		}

		std::ostringstream DvsIdentifier;
		DvsIdentifier << std::hex << std::uppercase << Dvs;

		std::string HardwareIdentifier = CpuIdentifier + "-" + DvsIdentifier.str();

		return HardwareIdentifier;

		VMProtectEnd();
	}
}