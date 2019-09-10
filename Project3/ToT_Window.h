#pragma once
#include "Window.h"
#include "TextLib.h"
#include <iostream>
#include <fstream>
#include "Constants.h"

class ToT_Window : public Window
{
protected:
	bool hamachi_enabled;
	bool music_enabled;
	void LoadAtributes();

public:
	ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns);
	//writes list of all files with ext extension to file with specified path.
	bool LoadFiles(std::string src_path, std::string dst_path, const std::string ext);
	void SaveAtributes();
	void SetHamachiConnectionFlag(const bool flag);
	void SetMusic(const std::string sound_file, const bool playing);
	void Title(const COORD starting_point, const Text::TextAlign text_align);
	bool GetHamachiConnectionFlag() { return hamachi_enabled; }
	bool GetMusic() { return music_enabled; }
};