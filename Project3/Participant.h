#pragma once
#include <vector>

class SinglePlayer;

namespace Modifiers {
	const std::vector<std::string> car_modifiers = { "max_speed","max_accelerating","max_braking","hand_brake_value","durability","visibility","turn_mod","drift_mod" };
	const std::vector<std::string> tire_modifiers = { "asphalt","grass","gravel","sand","mud","ice" };
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
	int CarPoints(std::string car_path);
	float TiresPoints(const int terrain[], std::string tires_path);
	void GetRandomName(int id);
	void GetOptimalCar(const std::string tour_path);
	void GetOptimalTires();

public:
	bool alive = true;
	std::string name;
	std::vector<int>car_modifiers;
	std::vector<std::string>tire_modifiers;
	float score = 0.0f;
	float current_speed = 0.0f;
	float current_durability;
	SinglePlayer *network_role;
	std::string car_path;	
	float attacked = 0.0f;
	bool drift = false;

	Participant(std::string name, std::string car_path, std::string tire_path, SinglePlayer &network_role);	//Creating human player
	Participant(int id, std::string tour_path, SinglePlayer &network_role);	//creating AI
	Participant() = delete;
	void TakeAction(const int turn);	//ai action
	void Test(std::string field, bool show);
	float EvaluateChance(std::string field, float speed, bool drift);
	float CalculateBurning(float value);
	float EvaluateSpeed(std::string field, float chance, bool drift);
};