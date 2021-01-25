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
	static std::vector<std::string> LoadRawData(const char* path);
	static void AddNewRecord(std::vector<std::string>& ranking_data, const std::string& name);
	static void UpdateRecordFinished(std::vector<std::string>& ranking_data, const std::vector<std::string>& record_data, int classification, int index);
	static void UpdateRecordCrashed(std::vector<std::string>& ranking_data, const std::string& name, int classification, int index);
	static void SaveData(std::vector<std::string>& ranking_data, const char* path);
	static void AdjustStringVector(std::vector<std::string>& record_data);
	static std::vector<std::string> RankingInfoToStringVector(const RacerLeaderboardInfo& racer_leaderboard_info);
	static void ValidateRankingDetails(std::vector<std::string>& record_data);

	//returns -1 if racer is not ranked
	static int GetRankedRacerPosition(const std::vector<std::string>& ranking_data, std::string name);

	static std::string GetFavourite(const std::string& text);
	static std::string UpdateFavorite(const std::string& text, const std::string& record_id);

	RankingManagement() {};
public:

	static void Save(RacerLeaderboardInfo racer_leaderboard_info);
	static std::vector<std::string> GetRankedRacersNames(std::string tour);
	static std::vector<std::string> GetTextToDisplay(std::string tour, int racer_pos, int classification_type);
	
};