#include "KTrainApi.hpp"

/* DRAWING */

void EasyTrainer::DrawTextFunc(LPCSTR string, int x, int y, D3DCOLOR color)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	this->pFont->DrawTextA(NULL, string, -1, &rect, DT_NOCLIP, color);
}

void EasyTrainer::DrawFilledRectangle(IDirect3DDevice9* pDevice, float x, float y, float w, float h, D3DXCOLOR color)
{
	D3DRECT rect = { x, y, w, h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void EasyTrainer::DrawBorderBox(IDirect3DDevice9* pDevice, int x, int y, int w, int h, int thickness, D3DXCOLOR color)
{	
	DrawFilledRectangle(pDevice, x, y, w, y + thickness, color); // x
	DrawFilledRectangle(pDevice, x, y, x + thickness, h, color); // y
	DrawFilledRectangle(pDevice, x, h, w, h + thickness, color); // w
	DrawFilledRectangle(pDevice, w, y, w + thickness, h + thickness, color); // h
}

void EasyTrainer::MakeFont(IDirect3DDevice9* pDevice, LPCWSTR choiceFont)
{
	if (this->isFontCreated == false)
	{
		D3DXCreateFont(pDevice, 20, 0, FW_BOLD, 0, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, choiceFont, &this->pFont);
		this->isFontCreated = true;
	}
}

int EasyTrainer::endConfig()
{
	for (short int i = 0; i < 20; i++)
	{
		this->height += 19;
		if (this->button[i].isOn == false)
		{
			this->numOfItems = i;
			break;
		}
	}
	return this->numOfItems;
}

void EasyTrainer::Render(IDirect3DDevice9* pDevice)
{
	if (this->Visible)
	{
		this->DrawBorderBox(pDevice, this->x, this->y, this->width, this->height, 3, this->menuColor);

		this->DrawTextFunc(this->button[0].text, this->x + 5, 30, this->button[0].color);

		for (short int i = 1; i < this->numOfItems; i++)
		{
			this->DrawTextFunc(this->button[i].text, this->x + 5, 30 + i * 20,
				this->button[i].color);
		}
	}
}