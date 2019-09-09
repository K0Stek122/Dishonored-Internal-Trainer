#pragma once
#pragma comment(lib, "d3dx9.lib")

#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <d3dx9.h>
#include <d3d9.h>

namespace d3dHook
{
	BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam);

	HWND GetProcessWindow();

	bool GetD3D9Device(void ** pTable, size_t Size);
}