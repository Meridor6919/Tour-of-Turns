#pragma once
#include "Window.h"
#include <iostream>

class ToT_Window : public Window
{
protected:
	bool hamachi_enabled;
public:
	ToT_Window(char title[], int color1, int color2, int font_size, bool fullscreen, int chars_in_rows = 0, int chars_in_columns = 0);
	bool LoadFiles(std::string file_name, std::string ext);
	void SetHamachiConnectionFlag(bool flag);
	bool GetHamachiConnectionFlag() { return hamachi_enabled; };
};