#pragma once
#include "Window.h"
#include "TextLib.h"

class ToT_Window : public Window
{
protected:
	bool hamachi_enabled;
public:
	ToT_Window(char title[], int color1, int color2, int chars_in_rows, int chars_in_columns);
	bool LoadFiles(std::string path, std::string ext);		//writes list of all files with ext extension to file with specified path.
	void SetHamachiConnectionFlag(bool flag);
	void Title(COORD starting_point, Text::TextAlign text_align); // 41x15 title display
	bool GetHamachiConnectionFlag() { return hamachi_enabled; };
};