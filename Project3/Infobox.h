#pragma once
#include <fstream>
#include "TextLib.h"
#include "Window.h"

class InfoBox
{
	Window *main_window;
	Text::TextAlign text_align;
	COORD starting_point;
	int spacing;
	int size;

public:

	std::vector<std::string> info;
	InfoBox(int size, Text::TextAlign text_align, COORD starting_point, int spacing, Window &main_window);
	void Push(std::string special_text, std::string text);
	void Clear();
	
};