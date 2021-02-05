#include "Hooks.hpp"

bool Hook::hook(char* src, char* dst, int len)
{
	// Check Length
	if (len < 5)
		return false;

	// Change Protection
	DWORD curProtection;
	if (!VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection))
		return false;

	// Save for restore
	memcpy(this->orignal, src, len);
	this->length = len;
	this->address = src;

	memset(src, 0x90, len);

	uintptr_t relativeAddress = (uintptr_t)(dst - src - 5);

	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddress;

	// Restore Protection
	DWORD temp;
	if (!VirtualProtect(src, len, curProtection, &temp))
		return false;

	return true;
}

bool Hook::unhook()
{
	// Change Protection
	DWORD curProtection;
	if (!VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &curProtection))
		return false;

	// Copy Original
	memcpy(address, orignal, length);

	// Restore Protection
	DWORD temp;
	if (!VirtualProtect(address, length, curProtection, &temp))
		return false;

	// Clean up
	memset(orignal, 0x0, length);
	length = 0;
	address = nullptr;

	return true;
}


char* TrampHook::trampHook(char* src, char* dst, unsigned int len)
{
	if (len < 5)
		return 0;

	// Create the gateway (len + 5 for the overwritten bytes + the jmp)
	gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	this->length = len + 5;


	// Put the bytes that will be overwritten in the gateway
	memcpy(gateway, src, len);

	// Get the gateway to destination addy
	uintptr_t gateJmpAddy = (uintptr_t)(src - gateway - 5);

	// Add the jmp opcode to the end of the gateway
	*(gateway + len) = (char)0xE9;

	// Add the address to the jmp
	*(uintptr_t*)(gateway + len + 1) = gateJmpAddy;

	// Place the hook at the destination
	if (h.hook(src, dst, len))
		return gateway;

	return nullptr;
}

bool TrampHook::trampUnhook()
{
	h.unhook();

	if (!VirtualFree(gateway, 0, MEM_RELEASE))
		return false;

	return true;
}