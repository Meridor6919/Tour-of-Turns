#include "Participant.h"
#include "ToT_Window.h"

Participant::Participant(InfoBox *infobox)
{
	this->infobox = infobox;
}
void Participant::Init(std::string tour_path)
{
	bool valid = false;
	std::vector<std::string> car_names = GetCarNames(tour_path);
	std::vector<std::string> tire_names = GetTireNames();

	for (int i = 0; i < static_cast<int>(car_names.size()); ++i)
	{
		if (car_names[i] == car_path)
		{
			valid = true;
			break;
		}
	}
	if (!valid)
	{
		car_path = car_names[0];
	}
	valid = false;
	for (int i = 0; i < static_cast<int>(tire_names.size()); ++i)
	{
		if (tire_names[i] == tire_path)
		{
			valid = true;
			break;
		}
	}
	if (!valid)
	{
		tire_path = tire_names[0];
	}
	this->car_modifiers = GetCarParameters(car_path);
	this->tire_modifiers = GetTireParameters(tire_path);
	this->current_durability = static_cast<float>(car_modifiers[CarAttributes::durability]);
}
void Participant::Test(const std::string field, const bool show)
{
	CalculateParameters();
	if (!IsAlive())
	{
		return;
	}
	const char terrain = field[0];
	std::string tire = tire_modifiers[atoi(&terrain)];
	std::string helper = tire;
	const int find = static_cast<int>(tire.find('x'));
	int reqired_tests = atoi(helper.erase(find, helper.size() - find).c_str());
	int number_of_tests = atoi(tire.erase(0, find + 1).c_str());
	int passed_tests = 0;
	int max = 0;
	int min = 100;
	float local_score;
	float formula = EvaluateChance(field, current_speed, drift);

	float dmg = 1.0f - GameConstants::speed_reduction*attacked;
	if (static_cast<int>(attacked*10.0f) % 10)
	{
		++sum_of_performed_attacks;
	}
	current_speed *= dmg;
	if (dmg < 1.0f && show)
	{
		infobox->Push(name + LanguagePack::text[LanguagePack::other_strings][OtherStrings::lost] + std::to_string(static_cast<int>(current_speed - dmg * current_speed)) + LanguagePack::text[LanguagePack::other_strings][OtherStrings::speed],
			LanguagePack::text[LanguagePack::other_strings][OtherStrings::in_result_of_attacks]);
	}

	attacked = 0;

	if (static_cast<int>(field.size()) > 1)
	{
		//testing algorithm
		for (int i = 0; i < number_of_tests; ++i)
		{
			local_score = static_cast<float>(rand() % 100) + static_cast<float>(rand() % 100 + 1) / 100.0f;

			if (local_score > formula)
			{
				if (local_score > max)
					max = static_cast<int>(local_score);

				++passed_tests;
			}
			else if (local_score < min)
			{
				min = static_cast<int>(local_score);
			}
		}
		//consequences and messages of test results
		float durablity_lost = 0;
		int bad_case = 0;
		if (passed_tests >= reqired_tests)
		{
			if (show)
			{
				infobox->Push(name + LanguagePack::text[LanguagePack::accident_effects][0], LanguagePack::text[LanguagePack::other_strings][OtherStrings::required] + std::to_string(static_cast<int>(formula)) + LanguagePack::text[LanguagePack::other_strings][OtherStrings::highest_roll] + std::to_string(max));
			}
		}
		else
		{
			if (show)
			{
				infobox->Push(name + LanguagePack::text[LanguagePack::accident_effects][1], LanguagePack::text[LanguagePack::other_strings][OtherStrings::required] + std::to_string(static_cast<int>(formula)) + LanguagePack::text[LanguagePack::other_strings][OtherStrings::lowest_roll] + std::to_string(min));
			}
			//badly_crashed
			if (formula > static_cast<float>(min + GameConstants::badly_crashed))
			{
				durablity_lost = current_speed * (100.0f + formula - static_cast<float>(min)) / 50.0f;
				bad_case = 2;
				current_speed = 0.0f;
			}
			//crashed
			else if (formula > static_cast<float>(min + GameConstants::crashed))
			{
				durablity_lost = current_speed * (100.0f + formula - static_cast<float>(min)) / 75.0f;
				bad_case = 3;
				current_speed = 0.0f;
			}
			//dangerous_accident
			else if (formula > static_cast<float>(min + GameConstants::dangerous_accident))
			{
				durablity_lost = current_speed * (100.0f + formula - static_cast<float>(min)) / 100.0f;
				bad_case = 4;
				current_speed /= 15.0f;
			}
			//off_route
			else if (formula > static_cast<float>(min + GameConstants::off_route))
			{
				durablity_lost = current_speed;
				bad_case = 5;
				current_speed /= 7.5f;
			}
			//dangerous_slip
			else if (formula > static_cast<float>(min + GameConstants::dangerous_slip))
			{
				durablity_lost = current_speed / 2.0f;
				bad_case = 6;
				current_speed /= 2.0f;
			}
			//slip
			else
			{
				current_speed = static_cast<float>(current_speed) / 1.2f;
				bad_case = 7;
			}
			if (show)
			{
				infobox->Push(name + LanguagePack::text[LanguagePack::accident_effects][bad_case], name + LanguagePack::text[LanguagePack::other_strings][OtherStrings::lost] + (durablity_lost > 0 ? std::to_string(static_cast<int>(durablity_lost)) : "") + LanguagePack::text[LanguagePack::race_player_informations][3]);
			}
			current_durability -= durablity_lost;
			if (current_durability <= 0.f)
			{
				KillParticipant();
			}
		}
	}
	else
	{
		drift = false;
	}
	if (drift == true)
	{
		++sum_of_performed_drifts;
		drift = false;
		score += GameConstants::drift_value;
	}
	else
	{
		score += 100.0f / (1.0f + current_speed * GameConstants::meters_per_second);
	}
}
float Participant::EvaluateChance(std::string field, const float speed, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}
	field.erase(0, 1);
	float base = ((speed / static_cast<float>(atof(field.c_str())) - 1) * 100.0f + speed - static_cast<float>(atof(field.c_str()))) * 100 / (static_cast<float>(car_modifiers[CarAttributes::turn_mod + drift]) - GameConstants::chance_reduction * attacked);

	if (base < 0.0f)
	{
		base = 0.0f;
	}
	if (base > 100.0f)
	{
		base = 100.0f;
	}
	if (drift == true)
	{
		float result = (speed + base) / 2.0f;
		if (base < speed)
		{
			result = (speed + base * 2.0f) / 3.0f;
		}
		if (result > 100.0f)
		{
			result = 100.0f;
		}
		return result;
	}
	else
	{
		return (sqrt(base * -base + 200.0f * base) + 2.0f * base) / 3.0f;
	}
}
void Participant::CalculateParameters()
{
	current_speed += pending_action.first*(GameConstants::friction_scalar + GameConstants::tire_effectivness_multiplier * TireEffectivness(pending_action.second));
	if (current_speed < 0)
	{
		current_speed = 0;
	}
	else if (current_speed > static_cast<float>(car_modifiers[CarAttributes::max_speed]))
	{
		if (current_speed > static_cast<float>(car_modifiers[CarAttributes::max_speed])*GameConstants::maximum_speed_multiplier)
		{
			current_speed = static_cast<float>(car_modifiers[CarAttributes::max_speed] * GameConstants::maximum_speed_multiplier);
		}
		float temp = CalculateBurning(current_speed);
		sum_of_durability_burned += temp;
		current_durability -= temp;
		if (current_durability <= 0.f)
		{
			KillParticipant();
		}
	}
	current_speed *= GameConstants::friction_scalar;
}
float Participant::TireEffectivness(std::string field)
{
	auto Probability = [](double number_of_tests, double wanted_number) {

		double result = 1;
		int extra = static_cast<int>(number_of_tests - wanted_number);
		extra < 1 ? extra = 1 : 0;
		wanted_number < 1 ? wanted_number = 1 : 0;
		while (number_of_tests > 1)
		{
			result *= number_of_tests / wanted_number / extra / 2;
			--number_of_tests;
			--wanted_number < 1 ? wanted_number = 1 : 0;
			--extra < 1 ? extra = 1 : 0;
		}
		return result / 2;
	};

	const int terrain = field[0] - '0';
	double x, y;
	double result = 0.0;
	for (int i = 0; i < static_cast<int>(tire_modifiers[terrain].size()); ++i)
	{
		if (tire_modifiers[terrain][i] == 'x')
		{
			x = atof(tire_modifiers[terrain].substr(0, i).c_str());
			y = atof(tire_modifiers[terrain].substr(i + 1, static_cast<int>(tire_modifiers[terrain].size()) - i - 1).c_str());
			break;
		}
	}
	for (int j = 0; j < static_cast<int>(y - x + 1); ++j)
	{
		result += Probability(y, j);
	}
	return static_cast<float>(result);
}
bool Participant::IsAlive()
{
	return current_durability > 0;
}
void Participant::KillParticipant()
{
	current_durability = 0;
	infobox->Push(LanguagePack::text[LanguagePack::other_strings][OtherStrings::infobox_RIP_title] + name + LanguagePack::text[LanguagePack::other_strings][OtherStrings::infobox_RIP_msg], "");
}
float Participant::CalculateBurning(float value)
{
	float raw = value / static_cast<float>(car_modifiers[CarAttributes::max_speed]) - static_cast<float>(car_modifiers[CarAttributes::max_speed]);
	float result = 0.0f;

	if (raw < 0.0f)
	{
		return 0.0f;
	}
	if (raw > GameConstants::maximum_speed_multiplier - 1.0f)
	{
		raw = GameConstants::maximum_speed_multiplier - 1.0f;
		value = static_cast<float>(car_modifiers[CarAttributes::max_speed]) * (GameConstants::maximum_speed_multiplier - 1.0f);
	}

	int level = static_cast<int>(raw * GameConstants::burning_levels / GameConstants::maximum_speed_multiplier) + GameConstants::base_burning_levels;
	result = value * static_cast<float>(level + level * level) / 100.0f;
	return result;
}

void Participant::QueueAction(float value, std::string current_field)
{
	pending_action = std::make_pair(value, current_field);
}

