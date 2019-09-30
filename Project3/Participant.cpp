#include "Participant.h"
#include "NetworkRole.h"

Participant::Participant(const std::string name, const std::string car_path, const std::string tire_path, SinglePlayer &network_role)
{
	this->name = name;
	this->car_path = car_path;
	this->network_role = &network_role;
	this->car_modifiers = network_role.GetWindowPtr()->GetCarParameters(car_path);
	this->tire_modifiers = network_role.GetWindowPtr()->GetTireParameters(tire_path);
	this->current_durability = static_cast<float>(car_modifiers[CarModifiers::durability]);
}
Participant::Participant(const int id, const std::string tour_path, SinglePlayer &network_role)
{
	this->network_role = &network_role;
	GetRandomName(id);
	GetOptimalCar(tour_path);
	GetOptimalTires();
}
float Participant::TiresPoints(const int terrain[], const std::string tires_path)
{
	std::function <float(int)> factorial;
	factorial = [&factorial](int i) -> float {
		if (i > 1)
		{
			return static_cast<float>(i) * factorial(i - 1);
		}
		else
		{
			return 1.0f;
		}
	};
	const std::vector<std::string>tires_atrib = network_role->GetWindowPtr()->GetTireParameters(tires_path);
	float total_points = 0.0f;
	int x;
	int y;
	
	for (int i = 0; i < static_cast<int>(tires_atrib.size()); ++i)
	{

		for (int j = 0; j < static_cast<int>(tires_atrib[i].size()); ++j)
		{
			if (tires_atrib[i][j] == 'x')
			{
				x = static_cast<int>(atoi(tires_atrib[i].substr(0, j).c_str()));
				y = static_cast<int>(atoi(tires_atrib[i].substr(j+1, static_cast<int>(tires_atrib[i].size()) - j-j).c_str()));
			}
		}
		for (int j = 0; j <= y - x; ++x)
		{
			total_points += factorial(y) / factorial(y - x)*factorial(x) * pow(0.5f, x) * pow(0.5f, y - x) * static_cast<float>(terrain[i]);
		}
	}
	return total_points;
}
void Participant::GetRandomName(const int id)
{
	static std::vector<std::string> names = { "Paul Harackovy", "Mark Driver", "Max McDonald", "Gordon Goodman", "Miguela Aguela", "Hans Ufner", "Isao Fujimoto", "Igor Belov",
		"John Hill", "Andrew Anderson", "Jane Turning", "Lester King", "Drew McNeil", "Sam Samson","Gaston Reveneu", "Roman Torbon",
		"Helga Dick", "Mogore the Ogre", "David Black", "Reta Rdest", "Bloodwyn", "Quality Racer", "Sui Cide", "Ivan Atakovic", "Blu Apostrof" };

	const int name_id = rand() % (static_cast<int>(names.size()) - id);
	name = std::move(names[name_id]);
	names[name_id] = std::move(names[static_cast<int>(names.size()) - 1 - id]);
	names[static_cast<int>(names.size()) - 1 - id] = name;
}
void Participant::GetOptimalCar(const std::string tour_path)
{
	const std::vector<std::string> cars = network_role->GetWindowPtr()->GetCarNames(tour_path);
	int current_best = 0;
	int best_points = 0;
	for (int i = 0; i < static_cast<int>(cars.size()); ++i)
	{
		int j = CarPoints(cars[i]);
		if (j > best_points)
		{
			best_points = j;
			current_best = i;
		}
	}
	this->car_path = cars[current_best];
	this->car_modifiers = network_role->GetWindowPtr()->GetCarParameters(cars[current_best]);
	this->current_durability = static_cast<float>(car_modifiers[CarModifiers::durability]);
}
void Participant::GetOptimalTires()
{
	std::vector<std::string> tires = network_role->GetWindowPtr()->GetTireNames();
	std::vector<std::string> tour = network_role->GetWindowPtr()->GetTourParameters(network_role->tour, 0, INT_MAX);
	int terrain[6];
	memset(terrain, 0, 6);
	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		if (static_cast<int>(tour[i].size()) > 1)
		{
			++terrain[static_cast<int>(tour[i][0]) - 48];
		}
	}
	int current_best = 0;
	int best_points = 0;
	for (int i = 0; i < static_cast<int>(tires.size()); ++i)
	{
		int j = static_cast<int>(TiresPoints(terrain, tires[i]));
		if (j > best_points)
		{
			best_points = j;
			current_best = i;
		}
	}
	this->tire_modifiers = network_role->GetWindowPtr()->GetTireParameters(tires[current_best]);
}
int Participant::CarPoints(const std::string cars_path)
{
	std::vector<int> car_params;
	float total_points = 0.0f;
	int result;
	car_params = network_role->GetWindowPtr()->GetCarParameters(cars_path);

	result = car_params[CarModifiers::max_accelerating] - car_params[CarModifiers::max_speed] > 0 ? car_params[CarModifiers::max_speed] : car_params[CarModifiers::max_accelerating];
	total_points += static_cast<float>(result);

	result = car_params[CarModifiers::max_speed] - car_params[CarModifiers::max_accelerating] * 4 > 0 ? car_params[CarModifiers::max_accelerating] * 4 : car_params[CarModifiers::max_speed];
	total_points += static_cast<float>(result) * 0.2f;

	result = car_params[CarModifiers::max_braking] - car_params[CarModifiers::max_accelerating] > 0 ? car_params[CarModifiers::max_accelerating] : car_params[CarModifiers::max_braking];
	total_points += static_cast<float>(result) * 0.2f;

	result = 200 - 5 * (car_params[CarModifiers::hand_brake_value] - 20) * static_cast<int>(car_params[CarModifiers::hand_brake_value] > 20);
	total_points += static_cast<float>(result * static_cast<int>(result > 0))*0.1f;
	
	float value = static_cast<float>(car_params[CarModifiers::max_speed] + car_params[CarModifiers::max_accelerating]);
	float raw = value / static_cast<float>(car_params[CarModifiers::max_speed]);
	if (raw > 0.25f)
	{
		raw = 0.25f;
		value = static_cast<float>(car_params[CarModifiers::max_speed]) * 0.25f;
	}
	float durability = 0;
	for (float i = 1.0f; i < raw / 0.05 + 3.0f; i += 1.0f)
	{
		durability += i * value;
	}
	total_points += static_cast<float>(car_params[CarModifiers::durability]) / durability * 10.0f;

	total_points += static_cast<float>(car_params[CarModifiers::visibility]*5.0f);
	total_points *= static_cast<float>(car_params[CarModifiers::turn_mod])/100.0f;
	total_points *= static_cast<float>(car_params[CarModifiers::drift_mod])/100.0f;

	return static_cast<int>(total_points * (1.0f + static_cast<float>(rand() % 20) / 100.0f));
}
void Participant::TakeAction(const int turn)
{
	//AI LOGIC TO CHANGE
	std::vector<std::string> tour = network_role->GetWindowPtr()->GetTourParameters(network_role->tour,turn, car_modifiers[CarModifiers::visibility]);
	
	float safe_speed = static_cast<float>(car_modifiers[CarModifiers::max_speed])*1.0f;
	float risk = 40.0f;

	for (int i = 0; i < static_cast<int>(tour.size()); ++i)
	{
		if (static_cast<int>(tour[i].size()) > 1)
		{
			float normal_speed = EvaluateSpeed(tour[i], risk, false)/0.9f;
			for (int j = 0; j < i; ++j)
			{
				normal_speed = (normal_speed + static_cast<float>(car_modifiers[CarModifiers::max_braking])) / 0.9f;
			}
			if (normal_speed < safe_speed)
			{
				safe_speed = normal_speed;
			}
		}
	}
	current_speed = safe_speed;

	if (current_speed < 0.0f)
	{
		current_speed = 0.0f;
	}
	else if (current_speed > static_cast<float>(car_modifiers[CarModifiers::max_speed]))
	{
		if (current_speed > static_cast<float>(car_modifiers[CarModifiers::max_speed])*1.25f)
		{
			current_speed = static_cast<float>(car_modifiers[CarModifiers::max_speed] * 1.25f);
		}
		current_durability -= CalculateBurning(current_speed - car_modifiers[CarModifiers::max_speed]);
	}
	current_speed = current_speed*0.9f;
}
void Participant::Test(const std::string field, const bool show)
{
	if (!alive)
	{
		return;
	}
	const char terrain = field[0];
	std::string tire = tire_modifiers[atoi(&terrain)];
	std::string helper = tire;
	int find = static_cast<int>(tire.find("x"));
	int reqired_tests = atoi(helper.erase(find, helper.size() - find).c_str());
	int number_of_tests = atoi(tire.erase(0, find + 1).c_str());
	int passed_tests = 0;
	int max = 0;
	int min = 100;
	float local_score;
	float formula = EvaluateChance(field, current_speed, drift);

	float dmg = 1.0f - 0.05f*attacked;
	current_speed *= dmg;
	if (dmg < 1.0f && show)
	{
		network_role->infobox->Push(name + " lost " + std::to_string(static_cast<int>(current_speed - dmg * current_speed)) + " speed,", "in result of other racers behaviour");
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
		if (passed_tests >= reqired_tests)
		{
			if (show)
			{
				network_role->infobox->Push(name + " have manage to turn, ", "required - " + std::to_string(static_cast<int>(formula)) + " highest roll - " + std::to_string(max));
			}
		}
		else
		{
			if (show)
			{
				network_role->infobox->Push(name + " had mistaken, ", "required - " + std::to_string(static_cast<int>(formula)) + " lowest roll - " + std::to_string(min));
			}
			if (formula > static_cast<float>(min + 50))
			{
				current_durability -= current_speed * (100.0f + formula - static_cast<float>(min)) / 50.0f;
				if (show)
				{
					network_role->infobox->Push(name + " badly crashed !!! ", name + " lost " + std::to_string(static_cast<int>(current_speed * (100.0f + formula - static_cast<float>(min)) / 50)) + " durability");
				}
				current_speed = 0.0f;
			}
			else if (formula > static_cast<float>(min + 40))
			{
				current_durability -= current_speed * (100.0f + formula - static_cast<float>(min)) / 75.0f;
				if (show)
				{
					network_role->infobox->Push(name + " crashed !!!, ", name + " lost " + std::to_string(static_cast<int>(current_speed * (100.0f + formula - static_cast<float>(min)) / 75.0f)) + " durability");
				}
				current_speed = 0.0f;
			}
			else if (formula > static_cast<float>(min + 30))
			{
				current_durability -= current_speed * (100.0f + formula - static_cast<float>(min)) / 120.0f;
				if (show)
				{
					network_role->infobox->Push(name + " had an dangerous accident, ", name + " lost " + std::to_string(static_cast<int>(current_speed * (100.0f + formula - static_cast<float>(min)) / 120.0f)) + " durability");
				}
				current_speed /= 10.0f;
			}
			else if (formula > static_cast<float>(min + 20))
			{
				current_durability -= current_speed;
				if (show)
				{
					network_role->infobox->Push(name + " got off the route, ", name + " lost " + std::to_string(static_cast<int>(current_speed)) + " durability");
				}
				current_speed /= 5.0f;
			}
			else if (formula > static_cast<float>(min + 10))
			{
				current_durability -= current_speed / 2.0f;
				if (show)
				{
					network_role->infobox->Push(name + " fell into a dangerous slip, ", name + " lost " + std::to_string(static_cast<int>(current_speed / 2.0f)) + " durability");
				}
				current_speed /= 2.0f;
			}
			else
			{
				current_speed = static_cast<float>(current_speed) / 1.2f;
				if (show)
				{
					network_role->infobox->Push(name + " slipped, ", name + " lost a little bit of speed");
				}
			}
		}
	}
	if (drift == true)
	{
		drift = false;
		score += 1.5f;
	}
	else
	{
		score += 100.0f / (1.0f + current_speed * 10.0f / 36.0f);
	}
}
float Participant::EvaluateChance(std::string field, const float speed, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}
	field.erase(0, 1);
	float base = (speed / static_cast<float>(atof(field.c_str())) -1) * 100.0f + speed - static_cast<float>(atof(field.c_str()));

	if (base < 0.0f)
	{
		base = 0.0f;
	}
	if (drift == true)
	{
		base /= (static_cast<float>(car_modifiers[CarModifiers::drift_mod]) - 7.5f * attacked)/100.0f;
		if (base > 100.0f)
		{
			base = 100.0f;
		}
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
		base /= (static_cast<float>(car_modifiers[CarModifiers::turn_mod]) - 5.0f * attacked) / 100.0f;
		if (base > 100.0f)
		{
			base = 100.0f;
		}
		return (sqrt(base * -base + 200.0f * base) + 2.0f * base)/3.0f;
	}
}
float Participant::EvaluateSpeed(std::string field, const float chance, const bool drift)
{
	if (static_cast<int>(field.size()) < 2)
	{
		return 0.0f;
	}
	field.erase(0, 1);

	if (drift)
	{
		float result = (2.0f * chance*(static_cast<float>(car_modifiers[CarModifiers::drift_mod]) - (7.5f * attacked)) + 10000.0f + 100.0f * static_cast<float>(atof(field.c_str()))) /
			(10000.0f / static_cast<float>(atof(field.c_str())) + 100.0f + static_cast<float>(car_modifiers[CarModifiers::drift_mod]) - 7.5f*attacked);
		float secondary_result = (3.0f * chance*(static_cast<float>(car_modifiers[CarModifiers::drift_mod]) - (7.5f * attacked)) + 20000.0f + 200.0f * static_cast<float>(atof(field.c_str()))) /
			(20000.0f / static_cast<float>(atof(field.c_str())) + 200.0f + static_cast<float>(car_modifiers[CarModifiers::drift_mod]) - 7.5f*attacked);
	
		if (secondary_result > result)
		{
			result = secondary_result;
		}
		float base = ((result / static_cast<float>(atof(field.c_str())) - 1.0f) * 100.0f + result - static_cast<float>(atof(field.c_str()))) / 
			((static_cast<float>(car_modifiers[CarModifiers::drift_mod]) - (7.5f * attacked)) / 100.0f);

		if (base > 100.0f)
		{
			result = 2.0f * chance - 100.0f;
		}
		else if (base < 0.0f)
		{
			result = 3.0f * chance;
		}
		return result;
	}
	else
	{
		float delta = (200.0f + 12.0f * chance)*(200.0f + 12.0f * chance) - 180.0f * chance*chance;
		float base = (-(200.0f + 12.0f * chance) + sqrt(delta)) / -10.0f;
		float secondary_base = (-(200.0f + 12.0f * chance) - sqrt(delta)) / -10.0f;
		if (base > secondary_base)
		{
			base = secondary_base;
		}
		base = base * ((static_cast<float>(car_modifiers[CarModifiers::turn_mod]) - (5.0f * attacked)) / 100.0f);

		return static_cast<float>(atof(field.c_str())) + base / (100.0f / static_cast<float>(atof(field.c_str())) + 1.0f);
	}
}
float Participant::CalculateBurning(float value)
{
	float raw = value / static_cast<float>(car_modifiers[CarModifiers::max_speed]);
	float result = 0.0f;

	if (value < 0.0f)
	{
		return 0.0f;
	}
	if (raw > 0.25f)
	{
		raw = 0.25f;
		value = static_cast<float>(car_modifiers[CarModifiers::max_speed]) * 0.25f;
	}
	for (float i = 1.0f; i < raw / 0.05 + 3; i += 1.0f)
	{
		result += i * value;
	}
	return result / 10.0f;
}

