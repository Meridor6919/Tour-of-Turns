#pragma once
#include "Constants.h"

namespace LanguagePack
{
	std::vector<std::vector<std::string>> vector_of_strings;
	
	bool ValidateVectorOfStrings()
	{
		if (static_cast<int>(LanguagePack::vector_of_strings.size()) != LanguagePack::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::car_modifiers].size()) != CarModifiers::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::tire_modifiers].size()) != TireModifiers::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::main_menu].size()) != ValidationConstants::main_menu_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::game_lobby].size()) != ValidationConstants::game_lobby_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::multiplayer].size()) != ValidationConstants::multiplayer_lobby_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::game_options].size()) != ValidationConstants::option_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::information_box_titles].size()) != InformationBoxTitle::last)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::colors].size()) != ValidationConstants::possible_colors)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::race_actions].size()) != ValidationConstants::possible_actions)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::race_chances].size()) != ValidationConstants::race_chances_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::race_attribs].size()) != ValidationConstants::race_attribs)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::leaderboard].size()) != ValidationConstants::race_ranking_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::race_interface].size()) != ValidationConstants::possible_actions)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::race_distance].size()) != ValidationConstants::race_distance_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::race_boxes].size()) != ValidationConstants::race_boxes)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::race_infobox].size()) != TireModifiers::last * 2)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::participant_infobox].size()) != ValidationConstants::participant_infobox_size)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::on_off].size()) != 2)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::ranking_search_menu].size()) != ValidationConstants::ranking_search_menu)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::ranking_classification_types].size()) != ValidationConstants::ranking_classification_types)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::ranking_details].size()) != ValidationConstants::ranking_details)
		{
			return false;
		}
		if (static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::other_string].size()) != OtherStrings::last)
		{
			return false;
		}
		return true;
	}
	bool LoadVector(std::string path)
	{
		LanguagePack::vector_of_strings.clear();
		LanguagePack::vector_of_strings.push_back({});
		std::ifstream fvar;
		std::string line;
		int position = 0;
		fvar.open(path.c_str());
		while (std::getline(fvar, line))
		{
			if (line == "&&&")
			{
				LanguagePack::vector_of_strings.push_back({});
				++position;
			}
			else
			{
				LanguagePack::vector_of_strings[position].push_back(line);
			}
		}
		return ValidateVectorOfStrings();
	}
}