#pragma once
#include <string>
#include <vector>
#include <conio.h>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		int Button();
		int Button(const bool* loop, int out_of_loop_return_value, std::chrono::nanoseconds delay = default_delay);
		float GetTextAlignScalar(Align text_align);
		void SetColor(HANDLE output_handle, Color color);

		template<class T>
		std::string GetMonoCharacterString(T size, char character)
		{
			std::string ret{};
			ret.resize(size, character);
			return ret;
		}
		template<class T>
		std::string Spaces(T size)
		{
			return GetMonoCharacterString(size, ' ');
		}
	}
}