#pragma once
#include <iostream>
#include <fstream>
#include "Window.h"
#include "TextLib.h"
#include "Constants.h"

class ToT_Window : public Window
{
protected:
	bool hamachi_enabled;
	bool music_enabled;
	int ais;
	std::string name;
	bool playable;
	bool ranking_found;

	void LoadAtributes();
	bool ValidateGameFiles();
	bool ValidateTourFiles();
	bool ValidateCarFiles();
	bool ValidateTireFiles();
	std::vector<std::string> ReadFile(std::string path);
	bool SaveFileNames(std::string src_path, std::string dst_path, const std::string ext);

public:
	ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns);
	std::vector<std::string> GetTourNames();
	std::vector<std::string> GetCarNames(std::string tour);
	std::vector<std::string> GetTireNames();
	std::vector<std::string> GetTourParameters(std::string tour, int position, int visibility);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	void SaveAtributes();
	void Title(const COORD starting_point, const Text::TextAlign text_align);
	void SetHamachiConnectionFlag(const bool flag);
	void SetMusic(const std::string sound_file, const bool playing);
	void SetAIs(int number_of_ais);
	void SetName(std::string name);
	bool GetHamachiConnectionFlag() { return hamachi_enabled; }
	bool GetMusic() { return music_enabled; }
	int GetAIs() { return ais; }
	std::string GetName() { return name; }
	bool RankingFound() { return ranking_found; }
	bool IsPlayable() { return playable; }
};