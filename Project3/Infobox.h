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
	InfoBox(const int size, const Text::TextAlign text_align, const COORD starting_point, const int spacing, Window &main_window);
	void Push(const std::string special_text, const std::string text);
	void Clear();
	
};