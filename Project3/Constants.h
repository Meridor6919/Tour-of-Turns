#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace LanguagePack
{
	bool LoadLanguagePack(std::string path);
	extern std::vector<std::vector<std::string>> text;
	enum
	{
		title_main,
		title_additional,
		main_menu_options,
		tot_general_options,
		selectable_colors,
		game_information_options,
		game_information_introduction,
		game_information_tours,
		game_information_cars,
		game_information_tires,
		game_information_gameplay_formulas,
		game_information_accidents,
		credits,
		ranking_search_menu,
		ranking_classification_types,
		ranking_details,
		game_menu_options,
		multiplayer_menu_options,
		game_lobby_informations,
		race_actions,
		race_chancebox_informations,
		race_leaderboard,
		race_actions_descritions,
		race_general_informations,
		race_distances,
		race_names_of_boxes,
		race_segments_descriptions,
		race_accident_effects,
		car_attributes,
		terrain_types,
		on_off,
		multiplayer_lobby,
		other_strings,
		last
	};
}
namespace ConnectionCodes
{
	enum
	{
		Start = 10,
		GetInit,
		NewTurn,
		GetCarNames,
		GetTireNames,
		GetCarParams,
		GetTireParams,
		GetAllAttributes,
		GetTour,
		SetName,
		SetCar,
		SetTires,
		SetAction,
		SetAttack,
		last
	};
}
namespace GameLobbyInformations
{
	enum
	{
		player_information_title,
		tour_information_title,
		length_of_tour,
		number_of_turns,
		current_champion,
		win_ratio,
		games_in_total,
		average_place,
		last

	};
}
namespace CarAttributes
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
namespace TerrainTypes
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
namespace OtherStrings
{
	enum
	{
		default_name,
		action_confirm,
		infobox_RIP_title,
		infobox_RIP_msg,
		dont_attack,
		unable_to_move,
		meta,
		lost,
		required,
		highest_roll,
		lowest_roll,
		in_result_of_attacks,
		speed,
		border,
		loadingai_title,
		loadingplayer_title,
		loading_wheel,
		race_finished,
		lobby_size,
		last
	};
}
namespace Multiplayer {
	enum
	{
		active_games = 0,
		join,
		refresh,
		back,
		last
	};
}
namespace GameConstants
{
	//game balance
	const int maximum_timer = 60;
	const int maximum_name_length = 20;
	const int minimum_drift_speed = 40;
	const int attack_forward_distance = 4;
	const int attack_backward_distance = 6;
	const float drift_value = 2.0f;
	const float friction_scalar = 0.9f;

	//validation
	const int validate_main_menu = 7;
	const int validate_game_menu = 8;
	const int validate_multiplayer_menu = 3;
	const int validate_tot_options = 6;
	const int validate_colors = 14;
	const int validate_actions = 5;
	const int validate_general_info = 4;
	const int validate_chancebox = 4;
	const int validate_leaderboard = 3;
	const int validate_distances = 7;
	const int validate_names_of_boxes = 3;
	const int validate_accidents = 8;
	const int validate_ranking_menu = 5;
	const int validate_ranking_classification = 3;
	const int validate_ranking_details = 12;
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
	const std::string ai = "ToTAI.exe";
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
	const std::string cheating_attempt = "Cheating detected";
	const std::string ai_connection = "AI module error";
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
	const std::string cheating_attempt = " has made an attempt to cheat. He is now eliminated :)";
	const std::string ai_connection = "There is an issue with your AI module, please repair files or reinstall the game";
}


