#include "WavTransformer.h"
namespace MeridorWavTransformer
{
	long long WavTransformer::FindSound(const char* name) const noexcept
	{
		for (int i = 0; i < sounds.size(); ++i)
		{
			if (sounds[i].name == name)
			{
				return i;
			}
		}
		return -1;
	}
	void WavTransformer::LoadBytes(const char* path, size_t index) noexcept
	{
		std::ifstream file(path, std::ios::binary);

		for (unsigned long i = 0; i < sounds[index].size; ++i)
		{
			sounds[index].bytes[i] = file.get();
		}
		file.close();
	}
	bool WavTransformer::Erase(size_t index) noexcept
	{
		if (index < sounds.size())
		{
			delete[] sounds[index].bytes;
			sounds.erase(sounds.begin() + index);
			return true;
		}
		return false;
	}
	bool WavTransformer::Erase(const char* name) noexcept
	{
		return Erase(FindSound(name));
	}
	bool WavTransformer::Add(const char* path) noexcept
	{
		bool already_loaded = FindSound(path) > 0;

		if (!already_loaded)
		{
			std::ifstream file(path, std::ios::binary | std::ios::ate);
			size_t file_size = static_cast<size_t>(file.tellg());
			sounds.emplace_back(new byte[file_size], path, file_size);

			bool valid_file = file.good();
			file.close();

			if (valid_file)
			{
				LoadBytes(path, sounds.size() - 1);
			}
			return valid_file;
		}
		return true;
	}
	unsigned long WavTransformer::GetDataStartingIndex(size_t index) const noexcept
	{
		unsigned long  i = 0;

		for (; i < sounds[index].size; ++i)
		{
			for (int j = 0; j < strlen(Internal::unique_identifier); ++j)
			{
				if (sounds[index].bytes[i] != Internal::unique_identifier[i])
				{
					break;
				}
				if (j == Internal::unique_identifier_length)
				{
					return i + Internal::data_offset;
				}
			}
		}
		return 0;
	}

	void WavTransformer::SetFlags(DWORD flags) noexcept
	{
		this->flags = flags;
	}
	void WavTransformer::StartPlaying(float volume) noexcept
	{
		if (not_muted && sounds.size() > 0)
		{
			PlaySoundA(reinterpret_cast<LPCSTR>(sounds[current_index].bytes), 0, SND_MEMORY | flags);
		}
		else
		{
			StopPlaying();
		}
	}
	bool WavTransformer::ChangeSound(size_t index) noexcept
	{
		if (index < sounds.size())
		{
			current_index = index;
			return true;
		}
		return false;
	}
	bool WavTransformer::ChangeSound(const char* name) noexcept
	{
		return ChangeSound(FindSound(name));
	}
	void WavTransformer::StopPlaying() noexcept
	{
		PlaySoundA(0, 0, 0);
	}
	WavTransformer::~WavTransformer()
	{
		StopPlaying();
		for (long long i = static_cast<long long>(sounds.size()) - 1; i >= 0; --i)
		{
			Erase(i);
		}
	}
	void WavTransformer::ChangeVolume(float volume) noexcept
	{
		if (volume)
		{
			//Data is stored in 2-byte long intervals and because of that short type is used
			//Multiplying those values by numbers from range 0.0 - 1.0 reduces volume by according percentage
			//There is built-in inaccuracy, because of floating point multiplication with short type
			not_muted = false;
			volume /= sounds[current_index].volume;
			unsigned long index = GetDataStartingIndex(current_index);
			short* widened_byte_ptr = reinterpret_cast<short*>(&sounds[current_index].bytes[index]);
			for (; index < sounds[current_index].size; index += 2)
			{
				*widened_byte_ptr = static_cast<short>((*widened_byte_ptr) * volume);
				++widened_byte_ptr;
			}
			sounds[current_index].volume *= volume;
		}
		else
		{
			not_muted = false;
		}
	}
	float WavTransformer::GetVolume() const noexcept
	{
		return not_muted * sounds[current_index].volume;
	}
}