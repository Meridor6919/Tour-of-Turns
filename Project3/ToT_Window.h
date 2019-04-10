#pragma once
#include "Window.h"
#include "TextLib.h"

class ToT_Window_ : public Window
{
protected:
	bool hamachi_enabled;
public:
	ToT_Window_(char title[], int color1, int color2, int font_size, bool fullscreen, int chars_in_rows = 0, int chars_in_columns = 0);
	bool LoadFiles(std::string path, std::string ext);		//writes list of all files with ext extension to file with specified path.
	void SetHamachiConnectionFlag(bool flag);
	void Title();
	bool GetHamachiConnectionFlag() { return hamachi_enabled; };
};