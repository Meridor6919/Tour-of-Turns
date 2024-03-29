#pragma once
#include "MeridorConsoleLib/Color.h"

namespace Validation
{
	constexpr int minimum_tour_lenght = 5;
	constexpr int digits_of_max_speed = 9;
	constexpr int max_visibility = 7;
	constexpr int max_number_of_tests = 20;

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
	constexpr COORD minimum_window_size = MeridorConsoleLib::minimum_window_size;
	constexpr int box_width = 84;
	constexpr int max_title_size = 1024;
	constexpr int min_title_size = 1;

	constexpr int volume_levels = 10;
	constexpr MeridorConsoleLib::Color first_color{ FOREGROUND_BLUE };
	constexpr MeridorConsoleLib::Color last_color{ FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE };
	constexpr MeridorConsoleLib::Color default_main_color = last_color;
	constexpr MeridorConsoleLib::Color default_secondary_color{ FOREGROUND_INTENSITY | FOREGROUND_GREEN };
	constexpr MeridorConsoleLib::Color colors = { last_color - first_color + 1 };

	constexpr short maximum_title_decoration_size = 20;
	constexpr short minimum_title_decoration_distance = 3;
	constexpr short maximum_title_decoration_distance = 10;

	constexpr short small_spacing = 2;
	constexpr short avarage_indent = 3;
	constexpr short avarage_spacing = 3;

	constexpr COORD top_left_box_position = { 0, 20 };
	constexpr COORD character_capacity = { 240, 78 };
	constexpr short main_centered_Y = 25;

	constexpr const char* default_title_name = "Tour of Turns";
	constexpr const char* default_theme_name = "Empty";
	constexpr const char* default_title_theme_file_content = "Empty\t \t \t \t \t5\t0";
	constexpr const char* default_game_config_file_content = "0\nRacer\nEnglish.lang\n0";
}