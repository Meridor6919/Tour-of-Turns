#pragma once
#include ".\MeridorMiscModules\WavTransformer.h"
#include ".\MeridorConsoleLib\Window.h"
#include ".\MeridorConsoleLib\TextLib.h"
#include "FileManagementFunctions.h"
#include "RankingManagement.h"
#include "ValidationFunctions.h"
#include "LanguagePack.h"
#include "Title.h"

using namespace MeridorConsoleLib;
using namespace FileManagement;
using namespace Validation;

class ToT_Window : public Window
{
protected:

	float music_volume;
	bool hamachi_flag;
	ToTGameConfig tot_game_config;
	Title title;
	WavTransformer wav_transformer;

public:
	void Init();

	//Title
	void DrawTitle(bool clear = false);
	std::string GetTitleTheme();
	void SetTitleTheme(std::string name);

	//Get methods
	const ToTGameConfig& GetToTGameConfig();
	ToTWindowConfig GetToTWindowConfig();
	bool GetHamachiConnectionFlag();
	int GetAIs();
	std::string GetName();
	int GetTimerSettings();
	std::string GetLanguage();
	float GetMusicVolume();
	
	//Set methods
	void SetHamachiConnectionFlag(const bool flag);
	void SetAIs(int number_of_ais);
	void SetName(std::string name);
	void SetTimerSettings(int timer_settings);
	void SetLanguage(std::string lang);
	void SetMusic(float volume);
};