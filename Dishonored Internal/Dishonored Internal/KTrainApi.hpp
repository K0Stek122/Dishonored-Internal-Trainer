#pragma once
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "detours.lib")

#include <Windows.h>
#include <TlHelp32.h>
#include <d3dx9.h>
#include <d3d9.h>

#include "detours.h"
#include "d3dHooking.h"

typedef struct EasyButton
{
	bool isOn = false;
	LPCSTR text;
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
};

class EasyTrainer
{
private:
	/* VARIABLES */
	bool isFontCreated = false;
	int numOfItems = NULL;

	int height = 20;

	int x = 25;
	int y = 25;

	ID3DXFont* pFont;

	D3DCOLOR menuColor = D3DCOLOR_ARGB(255, 255, 0, 0);

	/* FUNCTIONS */
	void DrawTextFunc(LPCSTR string, int x, int y, D3DCOLOR color);

	void DrawFilledRectangle(IDirect3DDevice9* pDevice, float x, float y, float w, float h, D3DXCOLOR color);

	void DrawBorderBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h, int thickness, D3DXCOLOR color);

public:
	/* VARIABLES */
	EasyButton button[50];
	int width = 155;
	bool Visible = false;

	/* FUNCTIONS */
	void MakeFont(IDirect3DDevice9* pDevice, LPCWSTR choiceFont);

	int endConfig();

	void Render(IDirect3DDevice9* pDevice);
};