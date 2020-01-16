#pragma once
#include <vector>

class ToT_Windowd;

class Participant 
{
public:
	bool alive = true;
	std::string name;
	std::vector<int>car_modifiers;
	std::vector<std::string>tire_modifiers;
	float score = 0.0f;
	float current_speed = 0.0f;
	float current_durability;
	std::string car_path;	
	ToT_Windowd *main_window;
	float attacked = 0.0f;
	bool drift = false;

	Participant(const std::string name, const std::string car_path, const std::string tire_path, ToT_Windowd *main_window);
	Participant() = delete;
	void Test(const std::string field, const bool show);
	float EvaluateChance(std::string field, const float speed, const bool drift);
	float CalculateBurning(float value);
	float EvaluateSpeed(std::string field, const float chance, const bool drift);
	void CalculateParameters(float value, std::string current_field);
	float TireEffectivness(std::string field);
};

