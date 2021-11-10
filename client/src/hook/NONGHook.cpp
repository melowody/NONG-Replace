#include "NONGHook.h"
#include <filesystem>
#include <nlohmann/json.hpp>

void runCouldntConnectPopup() {
	float x = 1.0f;
	float y = .6f;
	std::string copyText = "Couldn't connect to server!";

	auto textBoxAddr = reinterpret_cast<void* (__stdcall*)(std::string)>(gd::base + 0x1450B0);
	__asm {
		movss xmm0, x
		movss xmm1, y
	}
	CCLayer* ret = (CCLayer*)textBoxAddr(copyText);
	__asm add esp, 0x18

	CCDirector * director = CCDirector::sharedDirector();
	CCScene* scene = director->getRunningScene();
	ret->setZOrder(510);
	scene->addChild(ret);
	auto fadeto = CCFadeTo::create(0.14, 100);
	ret->runAction(fadeto);
}

std::string* __fastcall NongHook::GJGameLevel__getAudioFileNameHook(gd::GJGameLevel* self, void* edx, std::string* refString) {

	new(refString) std::string("");

	int levelID = self->m_bIsEditable ? self->m_nOriginalLevel : self->m_nLevelID;

	if (std::count(levels.begin(), levels.end(), levelID)) {
		std::string fileStr = getFile(levelID);
		std::ifstream file(fileStr.c_str());
		if (file) {
			*refString = fileStr;
		} else {
			getData(levelID, fileStr);

			if (std::filesystem::file_size(fileStr) == 0) {
				auto pMDM = gd::MusicDownloadManager::sharedState();
				pMDM->pathForSong(refString, self->m_nSongID);
				remove(fileStr.c_str());
			} else {
				*refString = fileStr;
			}
		}
	} else if (self->m_nSongID != 0) {
		auto pMDM = gd::MusicDownloadManager::sharedState();
		pMDM->pathForSong(refString, self->m_nSongID);
	} else {
		gd::LevelTools::getAudioFileName(refString, self->m_nAudioTrack);
	}

	return refString;

}

std::string NongHook::getFile(int id) {
	CCFileUtils* pFU = CCFileUtils::sharedFileUtils();
	std::string dir = pFU->getWritablePath() + "Replacements";
	if (!std::filesystem::is_directory(dir) || !std::filesystem::exists(dir)) {
		std::filesystem::create_directory(dir);
	}
	return dir + "\\" + std::to_string(id) + ".mp3";
}

void NongHook::getLevels() {
	CURLcode res;
	NongHook::levels.clear();
	std::string d;

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http://play.gear.is:3000/levels");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunctionString);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &d);
	res = curl_easy_perform(curl);

	if (res == CURLE_OK) {
		auto j = nlohmann::json::parse(d);
		for (int i : j) {
			levels.push_back(i);
		}
	} else {
		runCouldntConnectPopup();
	}

	curl_easy_cleanup(curl);
	curl = NULL;
}

void NongHook::getData(int id, std::string fileStr) {
	FILE* file;
	fopen_s(&file, getFile(id).c_str(), "wb");

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http://play.gear.is:3000/" + std::to_string(id));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NongHook::writeFunctionFile);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl = NULL;

	fclose(file);
}

size_t NongHook::writeFunctionFile(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

size_t NongHook::writeFunctionString(void* ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}