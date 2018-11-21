#pragma once

#include <windows.h>
#include <vector>
#include <DXGI.h>
#include "CPUInfo.h"

class SystemInfo: public CPUInfo
{
	public:
	SystemInfo(HINSTANCE, int);

	private:
	bool InitWindow(HINSTANCE, int);
	int MessageLoop();
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	bool EnumerateGPUsDescs();

	std::vector <DXGI_ADAPTER_DESC> AdaptersDescs;
};