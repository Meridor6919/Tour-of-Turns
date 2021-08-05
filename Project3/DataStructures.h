#pragma once
#include ".\MeridorConsoleLib\DataStructures.h"

struct ToTGameConfig
{
	std::string name;
	int number_of_ais;
	int timer_settings;
};
struct ToTWindowConfig
{
	std::string ai_module;
	float music_volume;
	bool hamachi_flag;
	std::string theme_name;
	std::string lang;
	COORD windowed_mode_window_size;
	MeridorConsoleLib::WindowInfoEx window_info;
};
struct RacerLeaderboardInfo
{
	std::string tour;
	std::string name;
	int place;
	double score;
	bool crash;
	int attacks;
	int drifts;
	double durability_burning;
	std::string car;
	std::string tires;
	int ais;
	bool multiplayer_flag;
};
struct TitleTheme
{
	std::string name = "";
	std::string main_left = "";
	std::string main_right = "";
	std::string secondary_left = "";
	std::string secondary_right = "";
	short secondary_pos_left = 0;
	short secondary_pos_right = 0;
	short decoration_distance = 5;
	bool decoration_wobble = false;
};