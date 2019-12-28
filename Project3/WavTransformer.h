#pragma once
#include <Windows.h>
#include <fstream>
#include <string>

#pragma comment(lib, "winmm.lib")

class WavTransformer
{
	int file_size = 0;
	byte *bytes;
	float volume = 1.0f;
	bool playing = false;
	DWORD flags = NULL;

public:
	WavTransformer(std::string path)
	{
		std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
		if (!file.good())
		{
			throw 1;
		}
		else
		{
			file_size = static_cast<int>(file.tellg());
			bytes = new byte[file_size];
			file.close();
			file.open(path.c_str(), std::ios::binary);
			for (long i = 0; i < file_size; ++i)
			{
				bytes[i] = file.get();
			}
			file.close();
		}
	}
	void Start(DWORD flags)
	{
		this->flags = flags;
		playing = true;
		PlaySound((LPCSTR)bytes, 0, SND_MEMORY | flags);
	}
	void ChangeVolume(float value)
	{
		if (!value)
		{
			return;
		}
		value /= volume;
		int index = 0;
		for (; index < file_size; ++index)
		{
			if (bytes[index] == 'd' && bytes[index + 1] == 'a' && bytes[index + 2] == 't' && bytes[index + 3] == 'a')
			{
				index += 8;
				break;
			}
		}
		short* widened_byte_ptr = reinterpret_cast<short*>(&bytes[index]);
		for (; index < file_size; index += 2)
		{
			*widened_byte_ptr = static_cast<short>((*widened_byte_ptr) * value);
			++widened_byte_ptr;
		}
		volume *= value;
		if (playing)
		{
			PlaySound((LPCSTR)bytes, 0, SND_MEMORY | flags);
		}
	}
	void Stop()
	{
		playing = false;
		PlaySound(0, 0, 0);
	}
	~WavTransformer()
	{
		PlaySound(0, 0, 0);
		delete[file_size] bytes;
	}
};