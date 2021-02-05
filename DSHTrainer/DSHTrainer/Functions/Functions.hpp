#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include <d3d9.h>

namespace Functions
{
	bool GetD3D9Device(void ** pTable, size_t Size);

	void DrawFilledRectangle(IDirect3DDevice9* pDevice, float x, float y, float w, float h, D3DCOLOR color);

	void* PatternScan(char* base, size_t size, char* pattern, char* mask);

	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);

	bool Hook(void * toHook, void * ourFunct, int len);

	void Patch(void* dst, void* src, unsigned int size);
}