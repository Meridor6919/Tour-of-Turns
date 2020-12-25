#pragma once
#include <Windows.h>
#include <fstream>
#include <string>

#pragma comment(lib, "winmm.lib")

class WavTransformer
{
	unsigned long file_size = 0;
	byte* bytes;
	float current_volume = 1.0f;
	DWORD flags = NULL;

	void LoadBytes(const std::string& path);
	unsigned long GetDataStartingIndex();

public:
	bool Init(std::string path);
	void SetFlags(DWORD flags);
	void StartPlaying(float volume);
	void StopPlaying();
	~WavTransformer();
};
