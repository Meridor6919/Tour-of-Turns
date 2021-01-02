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
	std::vector<std::string> GetRankingNames(std::string tour);
	ToTConfig LoadWindowConfig(int* main_color, int* secondary_color);
	void LoadLanguagePack(std::string path);

	std::vector<std::string> GetTourParameters(std::string tour, int position, int visibility);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	std::vector<std::string> GetRankingDetails(std::string tour, int racer_pos, int classification_type);

	void SaveRanking(RankingDetails ranking_info);
	bool SaveFileNames(std::string src_path, std::string dst_path, const std::string ext, HANDLE handle);
	void SaveWindowConfig(ToTConfig window_config, int main_color, int secondary_color);

	void RemoveExtension(std::vector<std::string>& vector, std::string extension);
	std::string GetSeparatedValue(const std::string& text, int index, char separator = '\t');
	std::string SetSeparatedValue(const std::string& original_text, const std::string& text_to_put, int index, char separator = '\t');

	std::string GetRankingFavourite(const std::string& text);
	std::string UpdateRankingFavorites(const std::string& text, const std::string& record_id);
};


