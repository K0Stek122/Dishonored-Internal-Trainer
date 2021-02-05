#include <iostream>
#include <Windows.h>
#include <string>

#include "Functions/Functions.hpp"
#include "Functions/Hooks/Hooks.hpp"
#include "Functions/SpeedHack/SpeedHack.hpp"
//#include "Functions/detours.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "SDK/Entity/Entity.hpp"
#include "SDK/Player/Player.hpp"

//Typedefs and external definitions

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* f_EndScene)(IDirect3DDevice9* pDevice);
f_EndScene oEndScene;

typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
WNDPROC oWndProc;

struct cheatVars
{
	bool isMenuVisible = false;

	bool infiniteHealth = false;
	bool infiniteMana = false;

	bool infiniteAmmo = false;
	bool infiniteBolt = false;
	bool infiniteIncendiary = false;
	bool infiniteSleep = false;
	bool infiniteSpringrazor = false;

	bool noclip = false;
}cheatVars;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (cheatVars.isMenuVisible)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		ImGuiIO& io = ImGui::GetIO();

		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[0];
			return 0;
		case WM_RBUTTONDOWN:
			io.MouseDown[1] = !io.MouseDown[1];
			return 0;
		case WM_MBUTTONDOWN:
			io.MouseDown[2] = !io.MouseDown[2];
			return 0;
		case WM_MOUSEWHEEL:
			return 0;
		case WM_MOUSEMOVE:
			io.MousePos.x = (signed short)(lParam);
			io.MousePos.y = (signed short)(lParam >> 16);
			return 0;
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

//Actual Code

TrampHook thEndScene;

Player* lPlayer = new Player();

HRESULT __stdcall EndScene(IDirect3DDevice9* pDevice)
{
	static bool init = true;
	if (init)
	{
		init = false;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplWin32_Init(FindWindowA(NULL, "Dishonored"));
		ImGui_ImplDX9_Init(pDevice);
	}
	//ImGui Starts

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (cheatVars.isMenuVisible)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow); //Show Cursor

		ImGui::Begin("Cheat Menu"); //First Window

		//Health
		if (ImGui::CollapsingHeader("Health"))
		{
			ImGui::Text("Current Health: ");
			ImGui::SliderInt("", lPlayer->GetCurrentHealth(), 0, *lPlayer->GetMaxHealth());
			ImGui::Checkbox("Infinite Health", &cheatVars.infiniteHealth);
		}

		//Magic
		if (ImGui::CollapsingHeader("Magic"))
		{
			ImGui::Text("Current Mana: ");
			ImGui::SliderInt("", lPlayer->GetCurrentMana(), 0, *lPlayer->GetMaxMana());
			ImGui::Checkbox("Infinite Mana", &cheatVars.infiniteMana);
			ImGui::InputInt("Runes", lPlayer->GetRunes());
		}

		if (ImGui::CollapsingHeader("Items"))
		{
			ImGui::InputInt("Coins", lPlayer->GetCoins());
			ImGui::SliderInt("Health Potions", lPlayer->GetHealthPotionAmount(), 0, 10);
			ImGui::SliderInt("Mana Potions", lPlayer->GetManaPotionAmount(), 0, 10);
			ImGui::Text("Warning: Requires at least 1 to work");
			ImGui::Checkbox("Infinite Ammo", &cheatVars.infiniteAmmo);
			ImGui::Checkbox("Infinite Bolts", &cheatVars.infiniteBolt);
			ImGui::Checkbox("Infinite Incendiary", &cheatVars.infiniteIncendiary);
			ImGui::Checkbox("Infinite Sleep", &cheatVars.infiniteSleep);
			ImGui::Checkbox("Infinite Springrazor", &cheatVars.infiniteSpringrazor);
		}

		if (ImGui::CollapsingHeader("Other"))
		{
			ImGui::Checkbox("FlyCheat", &cheatVars.noclip);
			ImGui::SliderFloat("SpeedHack Multiplier", &multiplier, 1.0f, 10.0f);
		}

		ImGui::End();
	}
	else
		ImGui::SetMouseCursor(ImGuiMouseCursor_None); //Hide Cursor

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	//ImGui Ends

	return oEndScene(pDevice);
	//return EndScene(pDevice);
}

DWORD WINAPI MainThread(LPVOID lParam)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	oWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, "Dishonored"), GWL_WNDPROC, (LONG_PTR)WndProc);

	//Directx9 Init
	void* d3dDevice[119];
	if (Functions::GetD3D9Device(d3dDevice, sizeof(d3dDevice)))
		oEndScene = (f_EndScene)thEndScene.trampHook((char*)d3dDevice[42], (char*)EndScene, 7);

	uintptr_t yCoord = (uintptr_t)GetModuleHandle("Dishonored.exe") + 0x24DD4B; //Gets Assembly addy of Y Coordinate Writer

	//SpeedHackEnable(); DOESN'T WORK, ILL FIX IT LATER.

	std::cout << Functions::PatternScan((char*)GetModuleHandle("Dishonored.exe"), 12, "\x89\x86\x00\x00\x00\x00\x7E\x00\x8B\xC1\x8D\x8E", "xx????x?xxxx");

	while (!GetAsyncKeyState(VK_F11))
	{
		lPlayer->SetMaxHealth(*lPlayer->GetMaxHealth()); //Without It There will be a bug, don't remember what bug xD.

		if (GetAsyncKeyState(VK_INSERT) & 1) //Menu Button
		{
			cheatVars.isMenuVisible = !cheatVars.isMenuVisible;
			lPlayer->Update();
		}

		if (GetAsyncKeyState(0x57) & 1) //0x57 = W
			lPlayer->Update();

		if (cheatVars.infiniteHealth && lPlayer->GetCurrentHealth() != lPlayer->GetMaxHealth()) //Infinite Health
			lPlayer->SetCurrentHealth(*lPlayer->GetMaxHealth());
		if (cheatVars.infiniteMana && lPlayer->GetCurrentMana() != lPlayer->GetMaxMana()) //Infinite Mana
			lPlayer->SetCurrentMana(*lPlayer->GetMaxMana());

		//Items Cheats
		if (cheatVars.infiniteAmmo)
			lPlayer->SetAmmoAmount(10, AmmoTypes::Pistol);
		if (cheatVars.infiniteBolt)
			lPlayer->SetAmmoAmount(10, AmmoTypes::Bolt);
		if (cheatVars.infiniteIncendiary)
			lPlayer->SetAmmoAmount(10, AmmoTypes::IncendiaryBolt);
		if (cheatVars.infiniteSleep)
			lPlayer->SetAmmoAmount(10, AmmoTypes::SleepDart);
		if (cheatVars.infiniteSpringrazor)
			lPlayer->SetAmmoAmount(10, AmmoTypes::Springrazor);

		//Other Cheats
		if (cheatVars.noclip)
		{
			/*
			if (init)
			{
				init = false;

				Functions::Patch((void*)zCoord, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
			}
			*/
			Functions::Patch((void*)yCoord, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
		}
		else
			Functions::Patch((void*)yCoord, "\xF3\x0F\x11\x86\xCC\x00\x00\x00", 8);

		Sleep(1);
	}

	FreeLibraryAndExitThread(NULL, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL));
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}