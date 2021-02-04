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
	static bool UpdateIntegerValue(const RacerLeaderboardInfo& racer_leaderboard_info, int index, int& value);
	static bool UpdateDoubleValue(const RacerLeaderboardInfo& racer_leaderboard_info, int index, double& value);
	static void UpdateRecordFinished(std::vector<std::string>& ranking_data, const RacerLeaderboardInfo& racer_leaderboard_info, int classification, int index);
	static void UpdateRecordCrashed(std::vector<std::string>& ranking_data, int classification, int index);

	static void AdjustStringVector(std::vector<std::string>& record_data);
	static void SanitizeRankingDetails(std::vector<std::string>& record_data);

	static std::vector<std::string> LoadRawData(const char* path);
	static void SaveData(std::vector<std::string>& ranking_data, const char* path);

	static void AddEmptyRecord(std::vector<std::string>& ranking_data, const std::string& name);

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