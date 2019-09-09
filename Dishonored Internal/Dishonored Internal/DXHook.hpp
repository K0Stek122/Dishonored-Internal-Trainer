#pragma once
#include "Includes.hpp"

typedef HRESULT(_stdcall * f_EndScene)(IDirect3DDevice9 * pDevice);
extern f_EndScene oEndScene;

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);

void initD3D();

void initMenu();