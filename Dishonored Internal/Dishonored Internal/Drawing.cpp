#include "Drawing.h"

ID3DXFont* pFont;

void drawing::CreateFontFunc(IDirect3DDevice9* pDevice, LPCWSTR choiceFont)
{
	D3DXCreateFont(pDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		choiceFont, &pFont);
}

void drawing::DrawTextFunc(LPCSTR string, int x, int y, D3DCOLOR color)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	pFont->DrawTextA(NULL, string, -1, &rect, DT_NOCLIP, color);
}

void drawing::DrawFilledRectangle(IDirect3DDevice9* pDevice, float x, float y, float w, float h, int a, int r, int g, int b)
{
	D3DCOLOR color = D3DCOLOR_ARGB(a, r, g, b);
	D3DRECT rect = { x, y, w, h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void drawing::DrawBorderBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h, int thickness, int a, int r, int g, int b)
{
	DrawFilledRectangle(pDevice, x, y, w, y + thickness, a, r, g, b); // x
	DrawFilledRectangle(pDevice, x, y, x + thickness, h, a, r, g, b); // y
	DrawFilledRectangle(pDevice, x, h, w, h + thickness, a, r, g, b); // w
	DrawFilledRectangle(pDevice, w, y, w + thickness, h + thickness, a, r, g, b); // h
}