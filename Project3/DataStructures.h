#pragma once
#include ".\MeridorConsoleLib\DataStructures.h"

struct WindowConfig
{
	std::string name;
	int main_color;
	int secondary_color;
	float music_volume;
	bool hamachi_flag;
	int ais;
	std::string lang;
	int timer_settings;
};
struct RankingInfo
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
	bool multiplayer;
};