#pragma once
#include <iostream>
#include <fstream>

#include ".\MeridorMiscModules\WavTransformer.h"
#include ".\MeridorConsoleLib\Window.h"
#include ".\MeridorConsoleLib\TextLib.h"
#include ".\MeridorConsoleLib\Infobox.h"
#include "GameConstants.h"
#include "ValidationConstants.h"
#include "FileManagementFunctions.h"
#include "ValidationFunctions.h"
#include "LanguagePack.h"

using namespace MeridorConsoleLib;
using namespace FileManagement;
using namespace Validation;

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
	float music_volume;

	WavTransformer wav_transformer;

	//Viability 
	bool playable;
	bool enable_ranking;

public:
	//Attributes
	std::shared_ptr<InfoBox> infobox;

	//General methods
	ToT_Window(const WindowInfoEx& window_info);
	void Title(const COORD starting_point, const MeridorConsoleLib::TextAlign text_align);

	//Get methods
	bool GetHamachiConnectionFlag();
	int GetAIs();
	std::string GetName();
	int GetTimerSettings();
	std::string GetLanguage();
	bool RankingFound();
	bool IsPlayable();
	bool GetMultiplayer();
	float GetMusicVolume();
	
	//Set methods
	void SetHamachiConnectionFlag(const bool flag);
	void SetAIs(int number_of_ais);
	void SetName(std::string name);
	void SetTimerSettings(int timer_settings);
	void SetLanguage(std::string lang);
	void SetMultiplayer(bool multiplayer);
	void SetMusic(float volume);
	
};