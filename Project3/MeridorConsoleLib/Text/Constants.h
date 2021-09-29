#pragma once
#define WIN32_LEAN_AND_MEAN

#include <chrono>
#include <windows.h>

namespace MeridorConsoleLib
{
	namespace Text
	{
		enum class Align : unsigned int
		{
			left,
			center,
			right,
			last
		};

		enum KeyCodes : int
		{
			backspace = 8,
			enter = 13,
			space = 32,
			up_arrow = 72,
			left_arrow = 75,
			right_arrow = 77,
			down_arrow = 80,
		};
		enum NumericSystems
		{
			binary = 2,
			octal = 8,
			decimal = 10,
			hexadecimal = 16
		};

		constexpr int failed = -1;

		constexpr short default_spacing = 1;
		constexpr short default_number_of_painted_rows = 1;
		constexpr short default_number_of_columns = 1;
		constexpr std::chrono::milliseconds default_delay = std::chrono::milliseconds(50);
	}
}