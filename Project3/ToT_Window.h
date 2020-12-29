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
	ToTConfig window_config;
	WavTransformer wav_transformer;

public:
	ToT_Window(const WindowInfoEx& window_info);

	//Get methods
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