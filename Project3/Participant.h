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
private:
	int ai_type;
	int CarPoints(std::string car_path);
	int TiresPoints(int terrain[], std::string tires_path);

public:

	bool alive;
	std::string name;
	std::vector<int>car_modifiers;
	std::vector<std::string>tire_modifiers;
	float score;
	float current_speed;
	float current_durability;
	SinglePlayer *network_role;
	std::string car_path;	
	int attacked;
	bool drift;

	Participant(std::string name, std::string car_path, std::string tire_path, SinglePlayer &network_role);	//Creating human player
	Participant(SinglePlayer *network_role, std::string tour_path);	//creating AI
	void TakeAction(int safe_speed, bool turn);	//ai action
	void Test(std::string field, bool show);
	float EvaluateChance(std::string field);
};