#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Window.h"

namespace Text
{
	enum TextAlign
	{
		left,
		center,
		right
	};
	enum Atributes
	{
		endl,
		color2,
	};
	namespace Choose
	{
		int Horizontal(std::vector<std::string> text, int starting_position, COORD starting_point, TextAlign text_align, bool clear_after, Window &main_window);
		int Veritcal(std::vector<std::string> text, int starting_position, COORD starting_posint, int spacing, TextAlign text_align, bool clear_after, Window &main_window);
		int Numeric(int max, COORD starting_point, bool zero_allowed, Window &main_window);
	}
	
	void OrdinaryText(std::vector<std::string> text, std::vector<Atributes> atribute, const TextAlign text_align, const short spacing, const short position, Window &main_window, bool clearing = false);
	void TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short starting_line, Window &main_window, bool clearing = false);
	void Spaces(int i);
}