#pragma once
#include <vector>

class SinglePlayer;

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
	void CalculateParameters(float value, std::string current_field);
	float TireEffectivness(std::string field);
};

