#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Window.h"
#include <mutex>

namespace Text
{
	enum TextAlign
	{
		left,
		center,
		right
	};
	char Button(bool *loop, char default_value, int delay);
	namespace Choose
	{
		int Horizontal(const std::vector<std::string> text, int starting_position, const COORD starting_point, const TextAlign text_align, const bool clear_after, Window &main_window, std::mutex *mutex, bool *loop = nullptr, char defalut_value = 0, int delay = 20);
		int Horizontal(const std::vector<std::string> text, int starting_position, const COORD starting_point, const TextAlign text_align, const bool clear_after, Window &main_window);
		int Veritcal(const std::vector<std::string> text, short starting_position, const COORD starting_point, const short spacing, const TextAlign text_align, const bool clear_after, Window &main_window, std::mutex *mutex, bool *loop = nullptr, char defalut_value = 0, int delay = 20);
		int Veritcal(const std::vector<std::string> text, short starting_position, const COORD starting_point, const short spacing, const TextAlign text_align, const bool clear_after, Window &main_window);
		int Numeric(const int max, COORD starting_point, const bool zero_allowed, Window &main_window, std::mutex *mutex, bool *loop = nullptr, char defalut_value = 0, int delay = 20);
		int Numeric(const int max, COORD starting_point, const bool zero_allowed, Window &main_window);
	}
	void OrdinaryText(std::vector<std::string> text, const TextAlign text_align, const short spacing, const COORD position, Window &main_window, std::mutex *mutex, const bool clearing = false);
	void OrdinaryText(std::vector<std::string> text, const TextAlign text_align, const short spacing, const COORD position, Window &main_window, const bool clearing = false);
	void TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short vertical_spacing, const COORD starting_point, Window &main_window, std::mutex *mutex, const bool clearing = false);
	void TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short vertical_spacing, const COORD starting_point, Window &main_window, const bool clearing = false);
	void Spaces(const int i, std::mutex *mutex);
	void Spaces(const int i);
}