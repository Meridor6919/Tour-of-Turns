#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace LanguagePack
{
	bool LoadVector(std::string path);
	extern std::vector<std::vector<std::string>> vector_of_strings;
	enum
	{
		car_modifiers,
		tire_modifiers,
		main_menu,
		game_information,
		introduction,
		tour_information,
		car_information,
		tire_information,
		game_mechanics_information,
		accidents_information,
		game_lobby,
		multiplayer,
		game_options,
		information_box_titles,
		colors,
		race_actions,
		race_chances,
		leaderboard,
		race_interface,
		race_attribs,
		race_distance,
		race_boxes,
		race_infobox,
		participant_infobox,
		credits,
		on_off,
		title_main,
		title_additional,
		ranking_search_menu,
		ranking_classification_types,
		ranking_details,
		other_string,
		last
	};
}
namespace InformationBoxTitle
{
	enum
	{
		player_info,
		tour_info,
		tour_length,
		turns,
		champion,
		win_rate,
		avg_place,
		last

	};
}
namespace OtherStrings
{
	enum
	{
		default_name,
		action_confirm,
		infobox_RIP1,
		infobox_RIP2,
		attack,
		unable_to_move,
		meta,
		lost,
		required,
		high_roll,
		low_roll,
		behaviour,
		speed,
		border,
		last
	};
}
namespace CarModifiers
{
	enum
	{
		max_speed,
		max_accelerating,
		max_braking,
		hand_brake_value,
		durability,
		visibility,
		turn_mod,
		drift_mod,
		last
	};
}
namespace TireModifiers
{
	enum
	{
		asphalt,
		grass,
		gravel,
		sand,
		mud,
		ice,
		last
	};
}
namespace ValidationConstants
{
	const int main_menu_size = 7;
	const int game_lobby_size = 8;
	const int multiplayer_lobby_size = 3;
	const int option_size = 6;
	const int possible_colors = 14;
	const int possible_actions = 5;
	const int race_attribs = 4;
	const int race_chances_size = 4;
	const int race_ranking_size = 3;
	const int race_distance_size = 7;
	const int race_boxes = 3;
	const int participant_infobox_size = 8;
	const int ranking_search_menu = 5;
	const int ranking_classification_types = 3;
	const int ranking_details = 12;
}
namespace FolderName
{
	const std::string tour = "Maps";
	const std::string car = "Cars";
	const std::string tire = "Tires";
	const std::string main = "GameFiles";
	const std::string language = "LangPack";
}
namespace FileName
{
	const std::string tire = "Tire.txt";
	const std::string tour = "Tour.txt";
	const std::string ranking = "Ranking.txt";
	const std::string config = "config.txt";
	const std::string music = "test.wav";
	const std::string language = "lang.txt";
}
namespace ExtName
{
	const std::string tour = ".tour";
	const std::string car = ".car";
	const std::string tire = ".tire";
	const std::string ranking = ".rank";
	const std::string language = ".lang";
}
namespace ErrorTitle
{
	const std::string unplayable = "Unplayable Game";
	const std::string missing_file = "Missing Files";
	const std::string corrupted_file = "Corupted Files";
	const std::string pipe_error = "Pipe error";
	const std::string language_error = "Language pack critical error";
	const std::string placeholder_language = "Used different language";
}
namespace ErrorMsg
{
	const std::string unplayable = "You can't start new race with current game state, please repair files or reinstall game";
	const std::string missing_file = "Files not found, please reinstall your game or repair missing files";
	const std::string ranking_missing = "There are no ranking files";
	const std::string corrupted_file = " cannot be read, please repair or delete corrupted files";
	const std::string pipe_error = "Please call a plumber";
	const std::string language_error = "Game cannot find any valid language packs, please repair files or reinstall game";
	const std::string placeholder_language = " is current language. \nPreviously selected language pack has been corrupted";
	const std::string available_cars = " has no cars available";
}
namespace GameValues
{
	const float drift_value = 2.0f;
	const float friction_scalar = 0.9f;
}
namespace MathFunctions
{
	inline long Factorial(int i) 
	{
		if (i > 1)
		{
			return Factorial(i - 1)*i;
		}
		else
		{
			return 1;
		}
	};
	template<class T>
	inline T PowerInt(T number, int power)
	{
		T value = 1;
		for (int i = 0; i < power; i++)
		{
			value *= number;
		}
		return value;
	}
}


