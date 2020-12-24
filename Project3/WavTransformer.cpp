#include "WavTransformer.h"

void WavTransformer::LoadBytes(const std::string& path)
{
	std::ifstream file(path.c_str(), std::ios::binary);

	for (unsigned long i = 0; i < file_size; ++i)
	{
		bytes[i] = file.get();
	}
	file.close();
}

unsigned long WavTransformer::GetDataStartingIndex()
{
	unsigned long  index = 0;

	for (; index < file_size; ++index)
	{
		if (bytes[index] == 'd' && bytes[index + 1] == 'a' && bytes[index + 2] == 't' && bytes[index + 3] == 'a')
		{
			return index + 8;
		}
	}
	return 0;
}

bool WavTransformer::Init(std::string path)
{
	std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
	file_size = static_cast<unsigned long>(file.tellg());
	bytes = new byte[file_size];
	bool valid_file = file.good();
	file.close();

	if (valid_file)
	{
		LoadBytes(path);
	}
	return valid_file;
}
void WavTransformer::SetFlags(DWORD flags)
{
	this->flags = flags;
}
void WavTransformer::StartPlaying(float value)
{
	if (!value)
	{
		StopPlaying();
	}
	else
	{
		//Data is stored in 2-byte long intervals and because of that short type is used
		//Multiplying those values by numbers from range 0.0 - 1.0 reduces volume by according percentage
		//There is built in inaccuracy, because of floating point multiplication with short type
		value /= volume;
		unsigned long index = GetDataStartingIndex();
		short* widened_byte_ptr = reinterpret_cast<short*>(&bytes[index]);
		for (; index < file_size; index += 2)
		{
			*widened_byte_ptr = static_cast<short>((*widened_byte_ptr) * value);
			++widened_byte_ptr;
		}
		volume *= value;
		PlaySound(reinterpret_cast<LPCSTR>(bytes), 0, SND_MEMORY | flags);
	}
}
void WavTransformer::StopPlaying()
{
	PlaySound(0, 0, 0);
}
WavTransformer::~WavTransformer()
{
	StopPlaying();
	delete[file_size] bytes;
}
