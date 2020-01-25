#pragma once
#include <iostream>
#include <fstream>
#include "Window.h"
#include "TextLib.h"
#include "Constants.h"
#include "Infobox.h"

class ToT_Window : public Window
{
protected:
	//Options
	std::string name;
	std::string lang = "Eng.lang";
	bool hamachi_enabled;
	int ais;

	//Viability 
	bool playable;
	bool ranking_found;

	void LoadAtributes();
	bool ValidateGameFiles();
	bool ValidateTourFiles();
	bool ValidateCarFiles();
	bool ValidateTireFiles();
	bool SaveFileNames(std::string src_path, std::string dst_path, const std::string ext);

public:
	//Attributes
	std::shared_ptr<InfoBox> infobox;

	//General methods
	ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns);
	void RemoveExtension(std::vector<std::string> &vector, std::string extension);
	void Title(const COORD starting_point, const Text::TextAlign text_align);

	//Get methods
	std::vector<std::string> GetTourNames();
	std::vector<std::string> GetCarNames(std::string tour);
	std::vector<std::string> GetTireNames();
	std::vector<std::string> GetTourParameters(std::string tour, int position, int visibility);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	bool GetHamachiConnectionFlag();
	int GetAIs();
	std::string GetName();
	std::string GetLanguage();
	bool RankingFound();
	bool IsPlayable();

	//Set methods
	void SetHamachiConnectionFlag(const bool flag);
	void SetAIs(int number_of_ais);
	void SetName(std::string name);
	bool SetLanguage(std::string lang);
	void SaveAtributes();
};