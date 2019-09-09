#pragma once
#include "Includes.hpp"
#include <vector>
#define LOG(x) std::cout << x << std::endl;

struct vector3
{
	float x, y, z;
};

struct offsets
{
	uintptr_t dwLocalPlayer = 0x01052DE8;
	uintptr_t dwHealth = 0x344;
	uintptr_t dwVelocity = 0x660;

	uintptr_t dwXOrigin = 0xC4;
	uintptr_t dwZOrigin = 0xC8;
	uintptr_t dwYOrigin = 0xCC;

	uintptr_t dwXAngle = 0x50;
	uintptr_t dwYAngle = 0x54;

	std::vector<unsigned int> dwMana = { 0x5F0, 0xFC, 0x0, 0x60 };
	std::vector<unsigned int> dwRunes = { 0x59C, 0xC8, 0x7C };
	std::vector<unsigned int> dwCoins = { 0x59C, 0xC8, 0x34 };

	uintptr_t assAmmoGadgets = 0x00C04F1B;
	uintptr_t assHealth = 0x00583FA9;
	uintptr_t assMana = 0x00AB1815;
	uintptr_t assPotions = 0x00C04DF9;
	uintptr_t assRunes = 0x00C0F53C;

}ofs;

struct cheatBools
{
	bool isActive = false;
}infHealth, infMana, infGadgets, infPotions, infCoins, infRunes, OneHitKill;

DWORD jmpBackAddy;
void __declspec(naked) OneHitKillFunc()
{
	__asm
	{
		cmp dword ptr [ebx+0xA4],0x41200000
		jne hitKill
		sub [ebx+0x344],edx
		jmp jmpBackAddy

		hitKill:
		sub dword ptr [ebx+0x344],1000
		jmp jmpBackAddy
	}
}

struct Hacks
{
private:
	uintptr_t localPlayer;
	uintptr_t gameModule = (uintptr_t)GetModuleHandle(L"Dishonored.exe");

public:
	vector3 plrPos;

	void switchBool(bool &cheat)
	{
		if (!cheat)
		{
			cheat = true;
			Beep(400, 150);
			Beep(600, 150);
		}
		else
		{
			cheat = false;
			Beep(600, 150);
			Beep(400, 150);
		}
	}

	void HackMain()
	{
		this->getPlayerPos(); //Gets Player Position (Duh)

		this->hackInfHealth(); //Infinite Health
		this->hackInfMana(); //Infinite Mana
		this->hackInfGadgets(); //Infinite Gadgets
		this->hackInfPotions(); //Infinite Potions
		this->hackInfCoins(); //Infinite Coins
		this->hackInfRunes(); //Infinite Runes
		this->hackOneHitKill(); //One Hit Kill Cheat
	}

	//Other Utilities
	uintptr_t getLocalPlayer()
	{
		this->localPlayer = *(uintptr_t*)(this->gameModule + ofs.dwLocalPlayer);
		return this->localPlayer;
	}//Gets LocalPlayer Addy

	void getPlayerPos() //Gets Player Position
	{
		if (this->getLocalPlayer() != NULL)
		{
			this->plrPos.x = *(float*)(this->getLocalPlayer() + ofs.dwXOrigin); //Gets X Position
			this->plrPos.y = *(float*)(this->getLocalPlayer() + ofs.dwYOrigin); //Gets Y Position
			this->plrPos.z = *(float*)(this->getLocalPlayer() + ofs.dwZOrigin); //Gets Z Position
		}
	}

	//Cheats
	void hackInfHealth()
	{
		//Toggle Cheat
		if (GetAsyncKeyState(VK_F1) & 1)
			switchBool(infHealth.isActive);

		//Actual Cheat
		if (infHealth.isActive)
			*(int*)(this->getLocalPlayer() + ofs.dwHealth) = 1337;
	}

	void hackInfMana()
	{
		if (GetAsyncKeyState(VK_F2) & 1)
			switchBool(infMana.isActive);

		if (infMana.isActive)
		{
			*(int*)internalFuncs::FindDmaAddy(this->gameModule + ofs.dwLocalPlayer, ofs.dwMana) = 1337;
		}
	}

	void hackInfGadgets()
	{
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			switchBool(infGadgets.isActive);
			if (infGadgets.isActive)
				internalFuncs::Patch((BYTE*)ofs.assAmmoGadgets, (BYTE*)"\x90\x90", 2);
			else
				internalFuncs::Patch((BYTE*)ofs.assAmmoGadgets, (BYTE*)"\x89\x06", 2);
		}
	}

	void hackInfPotions()
	{
		if (GetAsyncKeyState(VK_F4) & 1)
		{
			switchBool(infPotions.isActive);

			if (infPotions.isActive)
				internalFuncs::Patch((BYTE*)ofs.assPotions, (BYTE*)"\x90\x90\x90\x90\x90\x90\x90", 7);
			else
				internalFuncs::Patch((BYTE*)ofs.assPotions, (BYTE*)"\xFF\x8C\x86\xD4\x00\x00\x00", 7);
		}
	}

	void hackInfCoins()
	{
		if (GetAsyncKeyState(VK_F5) & 1)
			switchBool(infCoins.isActive);

		if (infCoins.isActive)
		{
			*(int*)internalFuncs::FindDmaAddy(this->gameModule + ofs.dwLocalPlayer, ofs.dwCoins) = 99999;
		}
	}

	void hackInfRunes()
	{
		if (GetAsyncKeyState(VK_F6) & 1)
			switchBool(infRunes.isActive);

		if (infRunes.isActive)
		{
			*(int*)internalFuncs::FindDmaAddy(this->gameModule + ofs.dwLocalPlayer, ofs.dwRunes) = 99;
		}
	}

	void hackOneHitKill()
	{
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			switchBool(OneHitKill.isActive);

			if (OneHitKill.isActive)
			{
				internalFuncs::FunctionHook((BYTE*)0x583FA9, OneHitKillFunc, 6);
				jmpBackAddy = ofs.assHealth + 6;
			}
			else
				internalFuncs::Patch((BYTE*)0x583FA9, (BYTE*)"\x29\x93\x44\x03\x00\x00", 6);
		}
	}

}hacks;