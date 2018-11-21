#ifndef _CPUID
#define _CPUID
#pragma once
#pragma comment(lib, "cpuid.lib")

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
int flags;
int misc;
int apic;
int signature;
int exflags;
} CPU, * PCPU;

bool getCPUID();
bool getVendorString(char* pvendorstr);
bool getProcessorString(char* pcpustr);
bool getFeatureFlags(PCPU pcpu);
int getCoresIntel();
int getCoresAMD();

#ifdef __cplusplus
}
#endif

#endif