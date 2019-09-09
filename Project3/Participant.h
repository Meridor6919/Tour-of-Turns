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
	int CarPoints(const std::string car_path);
	float TiresPoints(const int terrain[], const std::string tires_path);
	void GetRandomName(const int id);
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

	Participant(const std::string name, const std::string car_path, const std::string tire_path, SinglePlayer &network_role);
	Participant(const int id, const std::string tour_path, SinglePlayer &network_role);
	Participant() = delete;
	void TakeAction(const int turn);
	void Test(const std::string field, const bool show);
	float EvaluateChance(std::string field, const float speed, const bool drift);
	float CalculateBurning(float value);
	float EvaluateSpeed(std::string field, const float chance, const bool drift);
};