#pragma once
#include <vector>
#include ".\MeridorConsoleLib\GeneralFunctions.h"
#include "DataStructures.h"
#include "LanguagePack.h"
#include "ErrorConstants.h"
#include "GameConstants.h"
#include "FileManagementConstants.h"
#include "ValidationConstants.h"

class RankingManagement
{
	static std::vector<std::string> LoadRankingData(const char* path);
	static void AddNewRecord(std::vector<std::string>& ranking_data, const std::string& name);
	static void UpdateRecord(std::vector<std::string>& ranking_data, const std::vector<std::string>& ranking_details, int classification, int index);
	static void SaveRankingData(std::vector<std::string>& ranking_data, const char* path);
	static void AdjustRankingDetails(std::vector<std::string>& ranking_details);
	static std::vector<std::string> GetRankingDetails(const RankingDetails& ranking_info);

	//returns -1 if racer is not ranked
	static int GetRankedRacerPosition(const std::vector<std::string>& ranking_data, std::string name);

	static std::string GetRankingFavourite(const std::string& text);
	static std::string UpdateRankingFavorites(const std::string& text, const std::string& record_id);

	RankingManagement() {};
public:

	static std::vector<std::string> GetRankedRacersNames(std::string tour);

	static void SaveRanking(RankingDetails ranking_info);

	static std::vector<std::string> GetRankingDetails(std::string tour, int racer_pos, int classification_type);
	
};