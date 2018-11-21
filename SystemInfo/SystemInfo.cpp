#include <sstream>
#include "SystemInfo.h"

SystemInfo::SystemInfo(HINSTANCE hInstance, int nCmdShow)
{
	EnumerateGPUsDescs();
	if(!InitWindow(hInstance, nCmdShow)) return;
	MessageLoop();
}

bool SystemInfo::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	char szTitle[] = "SystemInfo";
	char szWindowClass[] = "SystemInfoClass";

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return reinterpret_cast<SystemInfo*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))->WndProc(hWnd, message, wParam, lParam);
	};
	wcex.cbWndExtra	    = sizeof(LONG_PTR);
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, IDI_INFORMATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(NULL, IDI_INFORMATION);

	if (!RegisterClassEx(&wcex)) return false;

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd) return false;

	SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToUlong(this));

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}

bool SystemInfo::EnumerateGPUsDescs()
{
    IDXGIAdapter * pAdapter = NULL; 
    IDXGIFactory* pFactory = NULL; 
	DXGI_ADAPTER_DESC AdapterDesc = {0};

    if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) return false;

    for (unsigned i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        pAdapter->GetDesc(&AdapterDesc);
		AdaptersDescs.push_back(AdapterDesc);
    } 

    if(pFactory) pFactory->Release();

    return true;
}

int SystemInfo::MessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}

LRESULT CALLBACK SystemInfo::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		{
			for (unsigned i=0, p=0, size=AdaptersDescs.size(); i<size; i++, p+=32)
			{
				TextOutW(hdc,0,p,AdaptersDescs[i].Description,lstrlenW(AdaptersDescs[i].Description));
				std::stringstream ss;
				ss << "[MB] VRAM: " << AdaptersDescs[i].DedicatedVideoMemory / 1048576 << " RAM: " << AdaptersDescs[i].DedicatedSystemMemory / 1048576 << " Shared RAM: " << AdaptersDescs[i].SharedSystemMemory / 1048576;
				TextOut(hdc,0,p+16,ss.str().c_str(),ss.str().length());
			}
			TextOut(hdc,400,0,VendorString,lstrlen(VendorString));
			TextOut(hdc,400,16,ProcessorString,lstrlen(ProcessorString));
			std::stringstream threads, ext;
			threads << "Number of " << (HasFeature(HTT) ? "threads " : "cores ") << CPUCores;
			TextOut(hdc,400,32,threads.str().c_str(),threads.str().length());
			ext << "Extensions:" << (HasFeature(MMX) ? " MMX" : "") << (HasFeature(FXSR) ? " FXSR" : "") << (HasFeature(SSE) ? " SSE" : "") << (HasFeature(SSE2) ? " SSE2" : "")
			<< (HasFeature(SSE3) ? " SSE3" : "") << (HasFeature(SSSE3) ? " SSSE3" : "") << (HasFeature(SSE41) ? " SSE4.1" : "") << (HasFeature(SSE42) ? " SSE4.2" : "") << (HasFeature(_3DNOW) ? " 3DNOW" : "") << (HasFeature(_3DNOWp) ? " 3DNOW+" : "")
			<< (HasFeature(MMXp) ? " MMX+" : "") << (HasFeature(IA64) ? " IA64" : "") << (HasFeature(x64) ? " x64" : "") << (HasFeature(AES) ? " AES" : "") << (HasFeature(AVX) ? " AVX" : "");
			TextOut(hdc,400,48,ext.str().c_str(),ext.str().length());
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
