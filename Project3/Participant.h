#pragma once
#include <vector>

class SinglePlayer;

namespace Modifiers {
	const std::string car_modifiers[] = { "max_speed","max_accelerating","max_braking","hand_brake_value","durability","visibility","turn_mod","drift_mod" };
	const std::string tire_modifiers[] = { "asphalt","grass","gravel","sand","mud","ice" };
}

enum CarModifiers
{
	max_speed,
	max_accelerating,
	max_braking,
	hand_brake_value,
	durability,
	visibility,
	turn_mod,
	drift_mod
};
enum TireModifiers
{
	asphalt,
	grass,
	gravel,
	sand,
	mud,
	ice
};

class Participant 
{
public:

	std::vector<int>car_modifiers;
	std::vector<std::string>tire_modifiers;
	float score;
	float current_speed;
	float current_durability;
	SinglePlayer *network_role;
	std::string car_path;

	Participant(std::string car_path, std::string tire_path, SinglePlayer &network_role);//Creating human player
	Participant(SinglePlayer *network_role);//creating AI
};