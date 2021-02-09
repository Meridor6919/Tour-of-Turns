#pragma once

namespace Validation
{
	constexpr int main_menu = 8;
	constexpr int game_menu = 8;
	constexpr int multiplayer_menu = 10;
	constexpr int multiplayer_before_game_lobby = 3;
	constexpr int tot_options = 9;
	constexpr int actions = 5;
	constexpr int general_info = 4;
	constexpr int chancebox = 4;
	constexpr int leaderboard = 3;
	constexpr int distances = 7;
	constexpr int names_of_boxes = 3;
	constexpr int accidents = 8;
	constexpr int ranking_menu = 5;
	constexpr int ranking_classification = 3;
	constexpr int ranking_details = RankingInfo::last;

	constexpr int maximum_name_length = 20;
	constexpr int maximum_timer = 60;
	constexpr COORD minimum_window_size = { 240, 77 };

	constexpr int first_color = FOREGROUND_BLUE;
	constexpr int last_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	constexpr int default_main_color = last_color;
	constexpr int default_secondary_color = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
	constexpr int colors = last_color - first_color + 1;

	constexpr short maximum_title_decoration_size = 20;
	constexpr short minimum_title_decoration_distance = 3;
	constexpr short maximum_title_decoration_distance = 10;
}