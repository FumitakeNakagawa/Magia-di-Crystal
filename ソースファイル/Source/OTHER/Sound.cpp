#include "OTHER\Sound.h"


void SOUND::Load_Sound()
{
	SetBGM("DATA\\SOUND\\Music\\BGM1.wav", "BGM1");
	SetBGM("DATA\\SOUND\\Music\\BGM2.wav", "BGM2");

	SetSE("DATA\\SOUND\\SE\\30second_01_0.wav", "30second");
	SetSE("DATA\\SOUND\\SE\\finish_01_0.wav", "finish");
}