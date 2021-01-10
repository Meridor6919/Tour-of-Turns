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
	MeridorConsoleLib::WindowInfoEx window_info;
};
struct RankingDetails
{
	std::string tour;
	std::string name;
	int place;
	int score;
	bool crash;
	int attacks; 
	int drifts;
	int durability_burning;
	std::string car;
	std::string tires;
	int ais;
	bool multiplayer_flag;
};