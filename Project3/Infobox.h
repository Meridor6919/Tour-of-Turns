#pragma once
#include <fstream>
#include "TextLib.h"
#include "Window.h"

class InfoBox
{
	Window *main_window;
	COORD starting_point;
	short spacing;
	int size_of_visible_infobox;

public:

	std::vector<std::string> infobox;
	InfoBox(const int size, const COORD starting_point, const short spacing, Window &main_window);
	void Push(const std::string special_text, const std::string text);
	void Clear();
};