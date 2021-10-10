#pragma once
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>

#pragma comment(lib, "winmm.lib")
namespace MeridorWavTransformer
{
	namespace Internal
	{
		struct SoundContainer
		{
			byte* bytes;
			const char* name;
			size_t size;
			float volume = 1.0;
		};

		constexpr const char* unique_identifier = "data";
		constexpr size_t unique_identifier_length = std::char_traits<char>::length(unique_identifier);
		constexpr unsigned long data_offset = 8;
	}
	class WavTransformer
	{
		std::vector<Internal::SoundContainer> sounds;
		DWORD flags = SND_ASYNC;
		bool not_muted = true;
		size_t current_index = 0;

		[[nodiscard]] long long FindSound(const char* name) const noexcept;
		void LoadBytes(const char* path, size_t index) noexcept;
		[[nodiscard]] unsigned long GetDataStartingIndex(size_t index) const noexcept;

	public:
		bool Add(const char* path) noexcept;
		bool Erase(size_t index) noexcept;
		bool Erase(const char* name) noexcept;

		void StartPlaying(float volume = 1.0f) noexcept;
		bool ChangeSound(size_t index) noexcept;
		bool ChangeSound(const char* name) noexcept;
		void StopPlaying() noexcept;

		void SetFlags(DWORD flags) noexcept;
		void ChangeVolume(float volume = 1.0f) noexcept;
		[[nodiscard]] float GetVolume() const noexcept;
		~WavTransformer();
	};
}