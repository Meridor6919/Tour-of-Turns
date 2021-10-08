#pragma once
#include <string>
#include <vector>
#include <conio.h>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		int Button() noexcept;
		int Button(const bool* loop, int out_of_loop_return_value, std::chrono::nanoseconds delay = default_delay) noexcept;
		[[nodiscard]] float GetTextAlignScalar(Align text_align) noexcept;
		void SetColor(HANDLE output_handle, Color color) noexcept;

		template<class T>
		[[nodiscard]] std::string GetMonoCharacterString(T size, char character) noexcept
		{
			std::string ret{};
			ret.resize(size, character);
			return ret;
		}
		template<class T>
		[[nodiscard]] std::string Spaces(T size) noexcept
		{
			return GetMonoCharacterString(size, ' ');
		}
	}
}