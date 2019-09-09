#include "Functions.hpp"


/* EXTERNAL */
DWORD externalFuncs::GetProcID(wchar_t* exeName)
{
	PROCESSENTRY32 procEntry = { 0 };
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!hSnapshot)
	{
		return 0;
	}

	procEntry.dwSize = sizeof(procEntry);

	if (!Process32First(hSnapshot, &procEntry))
	{
		return 0;
	}

	do
	{
		if (!wcscmp(procEntry.szExeFile, exeName))
		{
			CloseHandle(hSnapshot);
			return procEntry.th32ProcessID;
		}
	} while (Process32Next(hSnapshot, &procEntry));

	CloseHandle(hSnapshot);
	return 0;
}

MODULEENTRY32 externalFuncs::GetModuleEntry(DWORD dwProcID, wchar_t* moduleName)
{
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr))
		{
			do
			{
				if (!wcscmp(curr.szModule, moduleName))
				{
					modEntry = curr;
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

uintptr_t externalFuncs::GetModuleBaseAddy(DWORD procID, const wchar_t* moduleName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, moduleName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

uintptr_t externalFuncs::FindDmaAddyEx(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

void* externalFuncs::PatternScanEx(HANDLE hProcess, uintptr_t begin, uintptr_t end, char* pattern, char* mask)
{
	uintptr_t currentChunk = begin;
	SIZE_T bytesRead;

	while (currentChunk < end)
	{
		char buffer[4096];

		DWORD oldprotect;
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), PAGE_EXECUTE_READWRITE, &oldprotect);
		ReadProcessMemory(hProcess, (void*)currentChunk, &buffer, sizeof(buffer), &bytesRead);
		VirtualProtectEx(hProcess, (void*)currentChunk, sizeof(buffer), oldprotect, &oldprotect);

		if (bytesRead == 0)
		{
			return nullptr;
		}

		void* internalAddress = internalFuncs::PatternScan((char*)&buffer, bytesRead, pattern, mask);

		if (internalAddress != NULL)
		{
			//calculate from internal to external
			uintptr_t offsetFromBuffer = (uintptr_t)internalAddress - (uintptr_t)&buffer;
			return (void*)(currentChunk + offsetFromBuffer);
		}
		else
		{
			//advance to next chunk
			currentChunk = currentChunk + bytesRead;
		}
	}
	return nullptr;
}

void* externalFuncs::PatternScanExModule(HANDLE hProcess, wchar_t* exeName, wchar_t* moduleName, char* pattern, char* mask)
{
	DWORD processID = GetProcID(exeName);
	MODULEENTRY32 modEntry = externalFuncs::GetModuleEntry(processID, moduleName);

	if (!modEntry.th32ModuleID)
	{
		return nullptr;
	}


	uintptr_t begin = (uintptr_t)modEntry.modBaseAddr;
	uintptr_t end = begin + modEntry.modBaseSize;
	return externalFuncs::PatternScanEx(hProcess, begin, end, pattern, mask);
}

void externalFuncs::PatchEx(HANDLE hProcess, BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

void externalFuncs::NopEx(HANDLE hProcess, void* dst, unsigned int size)
{
	byte* nopArray = new byte[size];
	memset(nopArray, 0x90, size);

	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, nopArray, size, NULL);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
	delete[] nopArray;
}

bool externalFuncs::FunctionHookEx(HANDLE hProcess, void* whereToHook, void* function, int length)
{
	if (length < 5)
	{
		return false;
	}

	DWORD oldProtect;
	VirtualProtectEx(hProcess, whereToHook, length, PAGE_EXECUTE_READWRITE, &oldProtect);

	WriteProcessMemory(hProcess, whereToHook, function, length, NULL);

	DWORD relativeAddy = ((DWORD)function - (DWORD)whereToHook - 5);

	*(BYTE*)whereToHook = 0xE9;
	*(DWORD*)((DWORD)whereToHook + 1) = relativeAddy;

	DWORD temp;
	VirtualProtectEx(hProcess, whereToHook, length, oldProtect, &temp);

	return true;
}

/* INTERNAL */
MODULEINFO internalFuncs::GetModuleInfo(char* Module)
{
	MODULEINFO modInfo = { 0 };
	HMODULE hModule = GetModuleHandle((LPCWSTR)Module);

	if (hModule == NULL || hModule == 0)
		return modInfo;

	GetModuleInformation(GetCurrentProcess, hModule, &modInfo, sizeof(MODULEINFO));

	return modInfo;
}

void* internalFuncs::PatternScan(char* base, size_t size, char* pattern, char* mask)
{
	size_t patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(base + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (void*)(base + i);
		}
	}
	return NULL;
}

void internalFuncs::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void internalFuncs::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

uintptr_t internalFuncs::FindDmaAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

bool internalFuncs::FunctionHook(void* whereToHook, void* function, int length)
{
	if (length < 5)
	{
		return false;
	}

	DWORD oldProtect;
	VirtualProtect(whereToHook, length, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(whereToHook, 0x90, length);

	DWORD relativeAddy = ((DWORD)function - (DWORD)whereToHook - 5);

	*(BYTE*)whereToHook = 0xE9;
	*(DWORD*)((DWORD)whereToHook + 1) = relativeAddy;

	DWORD temp;
	VirtualProtect(whereToHook, length, oldProtect, &temp);

	return true;
}