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
	std::vector<std::string> GetRankedRacersNames(std::string tour);
	ToTGameConfig LoadGameConfig();
	ToTWindowConfig LoadWindowConfig();
	void LoadLanguagePack(std::string path);

	std::vector<std::string> GetTourParameters(std::string tour, int position, int visibility);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);

	void SaveRanking(RankingDetails ranking_info);
	void SaveGameConfig(const ToTGameConfig &game_config);
	void SaveWindowConfig(const ToTWindowConfig& window_config);

	std::vector<std::string> GetRankingDetails(std::string tour, int racer_pos, int classification_type);
	std::string GetRankingFavourite(const std::string& text);
	std::string UpdateRankingFavorites(const std::string& text, const std::string& record_id);
};


