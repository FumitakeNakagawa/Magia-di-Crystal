#pragma once
#include "iextreme.h"
#include <unordered_map>
#include <string>
#include <assert.h>

class SOUND
{

public:
	SOUND() {
		IEX_InitAudio();

		index = 0;
		bgmList.clear();
		seList.clear();
	}


	~SOUND()
	{
		IEX_ReleaseAudio();
	}

	void Load_Sound();

	void SetSE(char* filePath, std::string key)
	{
		bool result = IEX_SetWAV(index, filePath) == TRUE;
		assert(result && "SEÇÃì«Ç›çûÇ›Ç…é∏îsÇµÇ‹ÇµÇΩ.");

		seList[key] = index;
		index++;
	}

	void SetBGM(char* filePath, std::string key)
	{
		bool result = IEX_SetWAV(index, filePath) == TRUE;
		assert(result && "SEÇÃì«Ç›çûÇ›Ç…é∏îsÇµÇ‹ÇµÇΩ.");

		bgmList[key] = index;
		index++;
	}

	void PlaySE(std::string key)
	{
		
		IEX_PlaySound(seList[key], false);
	}

	void PlayBGM(std::string key)
	{
		IEX_PlaySound(bgmList[key], true);
	}

	void StopBGM(std::string key)
	{
		IEX_StopSound(bgmList[key]);
	}

	void SetSoundVolume(std::string key, s32 volume)
	{
		IEX_SetSoundVolume(bgmList[key], volume);
	}



private:
	int index;

	std::unordered_map<std::string, int> bgmList;
	std::unordered_map<std::string, int> seList;

};

extern SOUND* g_sound;