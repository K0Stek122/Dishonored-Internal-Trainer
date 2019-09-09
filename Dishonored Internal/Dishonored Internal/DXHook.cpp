#pragma comment(lib, "detours.lib")
#include <string>
#include <sstream>

#include "DXHook.hpp"
#include "Includes.hpp"
#include "d3dHooking.h"
#include "detours.h"
#include "Hacks.hpp"

typedef HRESULT(_stdcall * f_EndScene)(IDirect3DDevice9* pDevice);
f_EndScene oEndScene;

bool fontCreated = false;

//Draws Whole Menu
void drawMenu(IDirect3DDevice9* pDevice)
{
	drawing::DrawBorderBox(pDevice, 25, 25, 155, 175, 3, 255, 204, 0, 0);

	//Health
	if (!infHealth.isActive)
		drawing::DrawTextFunc("[F1]GodMode", 30, 30, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		drawing::DrawTextFunc("[F1]GodMode", 30, 30, D3DCOLOR_ARGB(255, 255, 0, 0));

	//Mana
	if (!infMana.isActive)
		drawing::DrawTextFunc("[F2]Inf Mana", 30, 50, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		drawing::DrawTextFunc("[F2]Inf Mana", 30, 50, D3DCOLOR_ARGB(255, 255, 0, 0));

	//Runes
	if (!infGadgets.isActive)
		drawing::DrawTextFunc("[F3]Inf Gadgets", 30, 70, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		drawing::DrawTextFunc("[F3]Inf Gadgets", 30, 70, D3DCOLOR_ARGB(255, 255, 0, 0));

	//Potions
	if (!infPotions.isActive)
		drawing::DrawTextFunc("[F4]Inf Potions", 30, 90, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		drawing::DrawTextFunc("[F4]inf Potions", 30, 90, D3DCOLOR_ARGB(255, 255, 0, 0));

	//Coins
	if (!infCoins.isActive)
		drawing::DrawTextFunc("[F5]Inf Coins", 30, 110, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		drawing::DrawTextFunc("[F5]inf Coins", 30, 110, D3DCOLOR_ARGB(255, 255, 0, 0));

	//Runes
	if (!infRunes.isActive)
		drawing::DrawTextFunc("[F6]Inf Runes", 30, 130, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		drawing::DrawTextFunc("[F6]inf Runes", 30, 130, D3DCOLOR_ARGB(255, 255, 0, 0));

	//One Hit Kill
	if (!OneHitKill.isActive)
		drawing::DrawTextFunc("[F7]OneHitKill", 30, 150, D3DCOLOR_ARGB(255, 255, 255, 255));
	else
		drawing::DrawTextFunc("[F7]OneHitKill", 30, 150, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void drawCoords(IDirect3DDevice9* pDevice)
{
	//All StringStreams Convert Const Char* To String
	std::stringstream xPos; 
	xPos << hacks.plrPos.x;

	std::stringstream yPos;
	yPos << hacks.plrPos.y;

	std::stringstream zPos;
	zPos << hacks.plrPos.z;

	//Actually Draws Position And Stuff
	drawing::DrawTextFunc("Position: ", 15, 920, D3DCOLOR_ARGB(255, 255, 255, 255));
	drawing::DrawTextFunc(xPos.str().c_str(), 15, 940, D3DCOLOR_ARGB(255, 255, 255, 255));
	drawing::DrawTextFunc(yPos.str().c_str(), 15, 960, D3DCOLOR_ARGB(255, 255, 255, 255));
	drawing::DrawTextFunc(zPos.str().c_str(), 15, 980, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT _stdcall Hooked_EndScene(IDirect3DDevice9 * pDevice) //DirectX Hook
{
	// Do Drawing Here
	if (!fontCreated)
	{
		drawing::CreateFontFunc(pDevice, L"Arial"); //Creates Font
		fontCreated = true;
	}

	drawMenu(pDevice); //Draws Menu
	drawCoords(pDevice); //Draws Coords

	Sleep(1); //For Optimization or Sumthing.
	return oEndScene(pDevice); //Returns (Duh.)
}

void initD3D() //Initializes Hook
{
	void* d3dDevice[119];
	if (d3dHook::GetD3D9Device(d3dDevice, sizeof(d3dDevice)))
	{
		oEndScene = (f_EndScene)DetourFunction((PBYTE)d3dDevice[42], (PBYTE)Hooked_EndScene);
	}
}