#pragma once
#include <Windows.h>
#include "../Detours/detours.h"

#pragma comment(lib, "Winmm.lib")

extern"C"
{
	static BOOL(WINAPI *originalQueryPerformanceCounter)(LARGE_INTEGER *performanceCounter) = QueryPerformanceCounter;
	static DWORD(WINAPI *originalGetTickCount)() = GetTickCount;
	static DWORD(WINAPI *originalTimeGetTime)() = timeGetTime;
}

HMODULE hModule;

float multiplier = 2.0f;																// Game speed multiplier

LARGE_INTEGER prevLi;
LARGE_INTEGER currentLi;
LARGE_INTEGER falseLi;

// QueryPerformanceCounter is generally what is used to calculate how much time has passed between frames. It will set the performanceCounter to the amount of micro seconds the machine has been running
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx

BOOL WINAPI newQueryPerformanceCounter(LARGE_INTEGER *counter)
{
	originalQueryPerformanceCounter(&currentLi);									// Get real current performance counter
	falseLi.QuadPart += ((currentLi.QuadPart - prevLi.QuadPart) * multiplier);		// Add the difference between this frame and the pervious * our multiplier to our false counter variable
	prevLi = currentLi;																// Set the previous to our current we just calculated

	*counter = falseLi;																// Make sure the caller gets our fake counter value
	return true;																	// Return true
}


DWORD prevTickCount;
DWORD currentTickCount;
DWORD falseTickCount;

// GetTickCount can also be used to calculate time between frames, but is used less since it's less accurate than QueryPerformanceCounter
// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724408%28v=vs.85%29.aspx

DWORD WINAPI newGetTickCount()
{
	currentTickCount = originalGetTickCount();										// Get the real current tick count
	falseTickCount += ((currentTickCount - prevTickCount) * multiplier);			// Add the difference between this frame and the pervious * our multiplier to our false tick count variable
	prevTickCount = currentTickCount;												// Set the previous to our current we just calculated

	return falseTickCount;															// Return false tick count
}

DWORD prevTime;
DWORD currentTime;
DWORD falseTime;

// timeGetTime can also be used to caluclate time between frames, as with GetTickCount it isn't as accurate as QueryPerformanceCounter
// https://msdn.microsoft.com/en-us/library/windows/desktop/dd757629(v=vs.85).aspx

DWORD WINAPI newTimeGetTime()
{
	currentTime = originalTimeGetTime();											// Get real current time
	falseTime += ((currentTime - prevTime) * multiplier);							// Add the difference between this frame and the pervious * our multiplier to our false tick count variable
	prevTime = currentTime;															// Set the previous to our current we just calculated

	return falseTime;																// Return false time
}


void SpeedHackEnable()
{
	falseLi = prevLi;																// Set false value which we use to keep track of the returned value each frame

	prevTickCount = originalGetTickCount();											// Set previous frame GetTickCount since it hasn't been hooked yet
	falseTickCount = prevTickCount;													// Set false value which we use to keep track of the returned value each frame

	prevTime = originalTimeGetTime();												// Set previous frame timeGetTime since it hasn't been hooked yet
	falseTime = prevTime;															// Set false value which we use to keep track of the returned value each frame
																					// Basic detours
	DisableThreadLibraryCalls(hModule);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)originalQueryPerformanceCounter, newQueryPerformanceCounter);
	DetourAttach(&(PVOID&)originalGetTickCount, newGetTickCount);
	DetourAttach(&(PVOID&)originalTimeGetTime, newTimeGetTime);
	DetourTransactionCommit();
}