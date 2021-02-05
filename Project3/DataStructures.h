#pragma once
#include ".\MeridorConsoleLib\DataStructures.h"

struct ToTGameConfig
{
	std::string name;
	int ais;
	std::string lang;
	int timer_settings;
};
struct ToTWindowConfig
{
	float music_volume;
	bool hamachi_flag;
	std::string title_theme;
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