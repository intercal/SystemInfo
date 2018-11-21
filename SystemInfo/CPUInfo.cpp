#include "CPUInfo.h"

CPUInfo::CPUInfo()
{
	if (!getCPUID()) return;
	getVendorString(VendorString);
	getProcessorString(ProcessorString);
	getFeatureFlags(&FeatureFlags);
	CPUCores = *(int *)VendorString == 0x68747541 ? getCoresAMD() : getCoresIntel();
}

bool CPUInfo::HasFeature(int feature) const
{
	switch(feature)
	{
		case MMX:
			return (FeatureFlags.flags & (1<<23)) != 0;
		case FXSR:
			return (FeatureFlags.flags & (1<<24)) != 0;
		case SSE:
			return (FeatureFlags.flags & (1<<25)) != 0;
		case SSE2:
			return (FeatureFlags.flags & (1<<26)) != 0;
		case SSE3:
			return (FeatureFlags.misc & 1) != 0;
		case SSSE3:
			return (FeatureFlags.misc & (1<<9)) != 0;
		case SSE41:
			return (FeatureFlags.misc & (1<<19)) != 0;
		case SSE42:
			return (FeatureFlags.misc & (1<<20)) != 0;
		case FMA:
			return (FeatureFlags.misc & (1<<12)) != 0;
		case _3DNOW:
			return (FeatureFlags.exflags & (1<<30)) != 0;
		case _3DNOWp:
			return (FeatureFlags.exflags & (1<<31)) != 0;
		case MMXp:
			return (FeatureFlags.exflags & (1<<22)) != 0;
		case IA64:
			return (FeatureFlags.flags & (1<<30)) != 0;
		case x64:
			return (FeatureFlags.exflags & (1<<29)) != 0;
		case AES:
			return (FeatureFlags.misc & (1<<25)) != 0;
		case AVX:
			return (FeatureFlags.misc & (1<<28)) != 0;
		case HTT:
			return (FeatureFlags.flags & (1<<28)) != 0;
		default:
			return false;
	}
}
