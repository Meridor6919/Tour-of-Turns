#pragma once
#include "Constants.h"

namespace LanguagePack
{
	std::vector<std::vector<std::string>> text;
	
	bool ValidateVectorOfStrings()
	{
		if (static_cast<int>(LanguagePack::text.size()) != LanguagePack::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::car_attributes].size()) != CarAttributes::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::terrain_types].size()) != TerrainTypes::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::main_menu_options].size()) != GameConstants::validate_main_menu)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::game_menu_options].size()) != GameConstants::validate_game_menu)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_menu_options].size()) != GameConstants::validate_multiplayer_menu)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::tot_general_options].size()) != GameConstants::validate_tot_options)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::game_lobby_informations].size()) != GameLobbyInformations::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::selectable_colors].size()) != GameConstants::validate_colors)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_actions].size()) != GameConstants::validate_actions)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_chancebox_informations].size()) != GameConstants::validate_chancebox)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_general_informations].size()) != GameConstants::validate_general_info)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_leaderboard].size()) != GameConstants::validate_leaderboard)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_actions_descritions].size()) != GameConstants::validate_actions)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_distances].size()) != GameConstants::validate_distances)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_names_of_boxes].size()) != GameConstants::validate_names_of_boxes)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_segments_descriptions].size()) != TerrainTypes::last * 2)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::race_accident_effects].size()) != GameConstants::validate_accidents)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::on_off].size()) != 2)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::ranking_search_menu].size()) != GameConstants::validate_ranking_menu)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::ranking_classification_types].size()) != GameConstants::validate_ranking_classification)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::ranking_details].size()) != GameConstants::validate_ranking_details)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::other_strings].size()) != OtherStrings::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::loading_wheel].size()) != 8)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_lobby].size()) < Multiplayer::last)
		{
			return false;
		}
		return true;
	}
	bool LoadLanguagePack(std::string path)
	{
		LanguagePack::text.clear();
		LanguagePack::text.push_back({});
		std::ifstream fvar;
		std::string line;
		int position = 0;
		fvar.open(path.c_str());
		while (std::getline(fvar, line))
		{
			if (line == "&&&")
			{
				LanguagePack::text.push_back({});
				++position;
			}
			else
			{
				LanguagePack::text[position].push_back(line);
			}
		}
		return ValidateVectorOfStrings();
	}
}