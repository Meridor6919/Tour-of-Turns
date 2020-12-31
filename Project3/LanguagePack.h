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
		main_menu,
		general_options,
		selectable_colors,
		gamepedia_sections,
		gamepedia_introduction,
		gamepedia_tours,
		gamepedia_cars,
		gamepedia_tires,
		gamepedia_gameplay_formulas,
		gamepedia_accidents,
		credits,
		ranking_menu,
		ranking_classification_types,
		ranking_details,
		game_options,
		game_options_multiplayer,
		multiplayer_menu,
		tour_information,
		race_actions,
		race_chancebox,
		race_leaderboard,
		race_actions_descritions,
		race_player_informations,
		race_distances,
		race_box_names,
		tour_segments_descriptions,
		accident_effects,
		car_attributes,
		terrain_types,
		on_off,
		multiplayer_client_lobby,
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
		loadingai_title,
		loadingplayer_title,
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