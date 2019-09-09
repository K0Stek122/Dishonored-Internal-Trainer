#include "Includes.hpp"
#include "DXHook.hpp"
#include "Hacks.hpp"

DWORD WINAPI HackThread(LPARAM lParam)
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);

	initD3D(); //Enables DirectX Hook For Drawing
	while (true)
	{
		hacks.HackMain(); //All The Cheats Are There
		Sleep(5); //For Optimization
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr)); //Creates A Handle
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}