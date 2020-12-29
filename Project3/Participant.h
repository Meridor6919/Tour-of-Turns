#pragma once
#include <vector>

#include "GameConstants.h"
#include "LanguagePack.h"
#include ".\MeridorConsoleLib\Infobox.h"

class ToT_Window;

class Participant 
{
private:
	MeridorConsoleLib::InfoBox *infobox;
	void CalculateParameters();

public:
	//Attributes
	std::pair<float, std::string> pending_action;
	std::string name = LanguagePack::text[LanguagePack::other_strings][OtherStrings::default_name];
	std::vector<int>car_modifiers;
	std::vector<std::string>tire_modifiers;
	float score = 0.0f;
	float current_speed = 0.0f;
	float current_durability = 0.0f;
	bool action_performed = true;
	bool attack_performed = true;
	float attacked = 0.0f;
	bool drift = false;
	int place;

	//Ranking data
	std::string car_path = "";
	std::string tire_path = "";
	int sum_of_performed_drifts = 0;
	int sum_of_performed_attacks = 0;
	float sum_of_durability_burned = 0.0f;

	Participant(MeridorConsoleLib::InfoBox* infobox);
	void Init(std::string tour_path);
	void Test(const std::string field, const bool show);
	float EvaluateChance(std::string field, const float speed, const bool drift);
	float CalculateBurning(float value);
	void QueueAction(float value, std::string current_field);
	float TireEffectivness(std::string field);
	bool IsAlive();
	void KillParticipant();
};

