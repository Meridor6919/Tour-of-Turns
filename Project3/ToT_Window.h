#pragma once
#include ".\MeridorMiscModules\WavTransformer.h"
#include ".\MeridorConsoleLib\Window.h"
#include ".\MeridorConsoleLib\TextLib.h"
#include "FileManagementFunctions.h"
#include "ValidationFunctions.h"
#include "LanguagePack.h"

using namespace MeridorConsoleLib;
using namespace FileManagement;
using namespace Validation;

class ToT_Window : public Window
{
protected:
	ToTConfig tot_config;
	WavTransformer wav_transformer;

public:
	ToT_Window(const WindowInfoEx& window_info);

	//Get methods
	const ToTConfig& GetToTConfig();
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