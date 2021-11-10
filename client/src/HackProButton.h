#ifndef __NONG_HACK_PRO_BUTTON_H__
#define __NONG_HACK_PRO_BUTTON_H__
#include "includes.h"
#include "hook/NONGHook.h"

using namespace Hackpro;

void WriteMemory(char data[], size_t size, LPVOID addr) {

	// Get the GD Process
	HWND hWnd = FindWindowA(0, "Geometry Dash");
	DWORD pId;
	GetWindowThreadProcessId(hWnd, &pId);
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);

	// Write data
	SIZE_T bytesWritten;
	WriteProcessMemory(hProc, addr, (LPCVOID)data, size, &bytesWritten);

}

class NongHackProButton : public Hackpro::Extension {

protected:

	void __stdcall enableHook() {
		NongHook::getLevels();
		MH_EnableHook((LPVOID)(gd::base + 0xBDC70));
	}

	void __stdcall disableHook() {
		MH_DisableHook((LPVOID)(gd::base + 0xBDC70));
	}

	void __stdcall openForm() {
		auto app = CCApplication::sharedApplication();
		app->openURL("https://forms.gle/sjpXjvuCUmC2zGqw7");
	}

	void Initialise() 
	{
		WriteMemory(new char[]{ (char)0xEB }, 1, (LPVOID)(gd::base + 0x17A0CE));
		this->AddCheckbox("Replace Songs", callback_func(NongHackProButton::enableHook), callback_func(NongHackProButton::disableHook));
		this->AddButton("Suggest Song", callback_func(NongHackProButton::openForm));
		this->Commit();
	}

public:

    static NongHackProButton* Create() {
		MH_CreateHook(
			(PVOID)(gd::base + 0xBDC70),
			NongHook::GJGameLevel__getAudioFileNameHook,
			(LPVOID*)&NongHook::GJGameLevel__getAudioFileName
		);
        auto pRet = static_cast<NongHackProButton*>(Extension::Create("NONG Replace"));
        pRet->Initialise();
        return pRet;
    }

};

#endif