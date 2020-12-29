#pragma once
#include <vector>
#include <string>

namespace LanguagePack
{
	inline std::vector<std::vector<std::string>> text = {};
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
		multiplayer_before_game_lobby,
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
		players_in_lobby,
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