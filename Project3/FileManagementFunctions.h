#pragma once
#include <vector>
#include ".\MeridorConsoleLib\GeneralFunctions.h"
#include "DataStructures.h"
#include "LanguagePack.h"
#include "ErrorConstants.h"
#include "GameConstants.h"
#include "FileManagementConstants.h"

namespace FileManagement
{
	std::vector<std::string> GetTourNames();
	std::vector<std::string> GetTireNames();
	std::vector<std::string> GetCarNames(std::string tour);
	ToTGameConfig LoadGameConfig();
	ToTWindowConfig LoadWindowConfig();
	void LoadLanguagePack(std::string path);

	std::vector<std::string> GetTourParameters(std::string tour, int position, int visibility);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);

	void SaveGameConfig(const ToTGameConfig &game_config);
	void SaveWindowConfig(const ToTWindowConfig& window_config);
};


