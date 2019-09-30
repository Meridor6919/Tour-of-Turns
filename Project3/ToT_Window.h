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
	int default_ais;
	std::string default_name;
	bool playable;
	bool ranking_found;

	void LoadAtributes();
	bool ValidateGameFiles();
	bool ValidateTourFiles();
	bool ValidateCarFiles();
	bool ValidateTireFiles();
	std::vector<std::string> ReadFile(std::string path);
	

public:
	std::vector<std::string> GetTourNames();
	std::vector<std::string> GetCarNames(std::string tour);
	std::vector<std::string> GetTireNames();
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	std::vector<std::string> GetTourParameters(std::string tour, int position, int visibility);
	ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns);
	bool SaveFileNames(std::string src_path, std::string dst_path, const std::string ext);
	void SaveAtributes();
	void SetHamachiConnectionFlag(const bool flag);
	void SetMusic(const std::string sound_file, const bool playing);
	void Title(const COORD starting_point, const Text::TextAlign text_align);
	void SetDefaultAIs(int number_of_ais);
	void SetDefaultName(std::string name);
	bool GetHamachiConnectionFlag() { return hamachi_enabled; }
	bool GetMusic() { return music_enabled; }
	bool IsPlayable() { return playable; }
	bool RankingFound() { return ranking_found; }
	int DefaultAis() { return default_ais; }
	std::string DefaultName() { return default_name; }
};