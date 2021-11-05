#ifndef __NONG_HOOK_HOOK_H__
#define __NONG_HOOK_HOOK_H__

#include "../includes.h"
#include "curl.h"
#include <fstream>

namespace NongHook {

	static std::vector<int> levels;

	inline std::string*(__thiscall* GJGameLevel__getAudioFileName)(gd::GJGameLevel*, void* edx, std::string*);
	std::string* __fastcall GJGameLevel__getAudioFileNameHook(gd::GJGameLevel*, void* edx, std::string*);

	void getData(int id, std::string fileStr);
	void getLevels();
	std::string getFile(int id);
	size_t writeFunctionFile(void* ptr, size_t size, size_t nmemb, FILE* data);
	size_t writeFunctionString(void* ptr, size_t size, size_t nmemb, std::string* data);

}

#endif