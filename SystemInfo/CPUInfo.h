#pragma once

#include "cpuid.h"

class CPUInfo
{
	public:
	CPUInfo();

	protected:
	int CPUCores;
	char VendorString[13];
	char ProcessorString[49];
	enum
	{
		MMX,
		FXSR,
		SSE,
		SSE2,
		SSE3,
		SSSE3,
		SSE41,
		SSE42,
		FMA,
		_3DNOW,
		_3DNOWp,
		MMXp,
		IA64,
		x64,
		AES,
		AVX,
		HTT,
		MAX_FEATURE
	};
	bool HasFeature(int) const;
	
	private:
	CPU FeatureFlags;
};