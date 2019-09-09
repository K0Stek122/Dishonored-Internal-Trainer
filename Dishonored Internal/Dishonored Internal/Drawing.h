#pragma once
#include "Functions.hpp"
#include "detours.h"
#include <d3dx9.h>
#include <d3d9.h>
//LPD3DXFONT test;

namespace drawing
{
	void CreateFontFunc(IDirect3DDevice9* pDevice, LPCWSTR choiceFont);

	void DrawTextFunc(LPCSTR string, int x, int y, D3DCOLOR color);

	void DrawFilledRectangle(IDirect3DDevice9* pDevice, float x, float y, float w, float h, int a, int r, int g, int b);

	void DrawBorderBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h, int thickness, int a, int r, int g, int b);
}