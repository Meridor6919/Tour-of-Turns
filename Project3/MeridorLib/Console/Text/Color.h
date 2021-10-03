#pragma once
namespace MeridorConsoleLib
{
	enum class Color : unsigned short
	{
		blue = 1,
		green,
		cyan,
		red,
		magenta,
		brown,
		light_gray,
		dark_gray,
		light_blue,
		light_green,
		light_cyan,
		light_red,
		light_magenta,
		yellow,
		white,
		last
	};
	inline constexpr Color operator+(const Color color, const int i)
	{
		return static_cast<Color>(static_cast<int>(color) + i);
	}
	inline constexpr Color operator-(const Color color, const int i)
	{
		return static_cast<Color>(static_cast<int>(color) + i);
	}
	inline constexpr Color operator-(const Color color, const Color other_color)
	{
		return static_cast<Color>(static_cast<int>(color) - static_cast<int>(other_color));
	}
	inline constexpr Color operator+(const Color color, const Color other_color)
	{
		return static_cast<Color>(static_cast<int>(color) + static_cast<int>(other_color));
	}
}


