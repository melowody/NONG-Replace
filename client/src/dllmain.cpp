#include "includes.h"
#include "HackProButton.h"
#include "hook/NONGHook.h"
#include <thread>

DWORD WINAPI thread_func(void* hModule) {

	/*AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stderr);*/
	
	// initialize minhook
	MH_Initialize();

	if (Hackpro::Initialise()) {
		while (!Hackpro::IsReady()) Sleep(50);
	}

	NongHackProButton::Create();
	
	return 0;
}

BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0x100, thread_func, handle, 0, 0);
	}
	return TRUE;
}