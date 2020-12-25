#pragma once
#include <iostream>
#include <fstream>

#include ".\MeridorConsoleLib\Window.h"
#include ".\MeridorConsoleLib\TextLib.h"
#include ".\MeridorConsoleLib\Infobox.h"
#include "Constants.h"

using namespace MeridorConsoleLib;

class ToT_Window : public Window
{
protected:
	//Options
	std::string name = "";
	std::string lang = "";
	bool hamachi_enabled = false;
	bool multiplayer = false;
	int timer_settings = 0;
	int ais = 0;

	//Viability 
	bool playable;
	bool enable_ranking;

	void LoadAtributes();
	bool ValidateGameFiles();
	bool ValidateTourFiles();
	bool ValidateCarFiles();
	bool ValidateTireFiles();
	bool ValidateRanking();
	bool SaveFileNames(std::string src_path, std::string dst_path, const std::string ext);

	//Ranking
	std::string UpdateRankingFavorites(std::string text, std::string phrase, int added_value);

public:
	//Attributes
	std::shared_ptr<InfoBox> infobox;
	MeridorConsoleLib::WindowInfo window_info;

	//General methods
	ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns);
	void RemoveExtension(std::vector<std::string> &vector, std::string extension);
	void Title(const COORD starting_point, const MeridorConsoleLib::TextAlign text_align);
	std::string GetClassifiedDetail(std::string text, int classification_type);

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
	int GetTimerSettings();
	std::string GetLanguage();
	bool RankingFound();
	bool IsPlayable();
	bool GetMultiplayer();

	//Set methods
	void SetHamachiConnectionFlag(const bool flag);
	void SetAIs(int number_of_ais);
	void SetName(std::string name);
	void SetTimerSettings(int timer_settings);
	void SetLanguage(std::string lang);
	void SetMultiplayer(bool multiplayer);
	void SaveRanking(std::string tour, std::string name, int place, int score, bool crash, int attacks, int drifts, int durability_burning, std::string car, std::string tires);
	void SaveAtributes();
};