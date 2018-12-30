#include "Participant.h"
#include "NetworkRole.h"


Participant::Participant(std::string name, std::string car_path, std::string tire_path, SinglePlayer &network_role)
{
	ai_type = 0;
	this->name = name;
	this->car_path = car_path;
	this->network_role = &network_role;
	this->car_modifiers = network_role.GetCarParameters(car_path);
	this->tire_modifiers = network_role.GetTireParameters(tire_path);
	this->current_speed = 0.0f;
	this->score = 0.0f;
	this->current_durability = static_cast<float>(car_modifiers[CarModifiers::durability]);
	this->attacked = 0;
}

Participant::Participant(SinglePlayer * network_role, std::string tour_path)
{

	this->network_role = network_role;
	this->current_speed = 0.0f;
	this->score = 0.0f;
	this->attacked = 0;

	static std::string names[] =
	{ "Paul Harackovy", "Mark Driver", "Max McDonald", "Gordon Goodman", "Miguela Aguela", "Hans Ufner", "Isao Fujimoto", "Igor Belov",
	"John Hill", "Andrew Anderson", "Jane Turning", "Lester King", "Drew McNeil", "Sam Samson","Gaston Reveneu", "Roman Torbon",
	"Helga Dick", "Mogore the Ogre", "David Black", "Reta Rdest", "Bloodwyn", "Quality Racer", "Sui Cide", "Ivan Atakovic", "Blu Apostrof" };
	std::string BestCarPath;
	std::string BestTirePath;
	std::string helper;

	ai_type = rand() % 3+1;

	static int used_names = 0;
	int RandomName = rand() % (24 - used_names);

	helper = names[RandomName];
	names[RandomName] = names[24 - used_names];
	name = helper;
	used_names++;
	
	std::vector<std::string> cars;
	network_role->GetCarNames(cars, tour_path);

	int current_best = 0;
	int best_points = 0;
	for (int i = 0; i < cars.size(); i++)
	{
		int j = CarPoints(cars[i]);
		if (j > best_points)
		{
			best_points = j;
			current_best = i;
		}
	}
	this->car_path = cars[current_best];
	this->car_modifiers = network_role->GetCarParameters(cars[current_best]);
	this->current_durability = static_cast<float>(car_modifiers[CarModifiers::durability]);

	std::vector<std::string> tires;
	network_role->GetTireNames(tires);
	
	current_best = 0;
	best_points = 0;
	for (int i = 0; i < tires.size(); i++)
	{
		int j = TiresPoints(tires[i]);
		if (j > best_points)
		{
			best_points = j;
			current_best = i;
		}
	}
	this->tire_modifiers = network_role->GetTireParameters(tires[current_best]);
}
int Participant::TiresPoints(std::string tires_path)
{
	std::function <int(int)> factorial;
	factorial = [&factorial](int i) -> int {
		if (i > 1)
			return i * factorial(i - 1);
		else
			return 1;
	};
	auto power = [](float number, int power) {
		int ret = 1;

		for (int i = 0; i < power; i++)
			ret *= number;

		return ret;
	};

	float total_points = 0.0f;
	int x, y;
	std::string helper;

	std::vector<std::string>tires_atrib = network_role->GetTireParameters(tires_path);
	for (int i = 0; i < tires_atrib.size(); i++)
	{

		for (int j = 0; j < tires_atrib[i].size(); j++)
		{
			if (tires_atrib[i][j] == 'x')
			{
				x = static_cast<int>(atoi(tires_atrib[i].substr(0, j).c_str()));
				y = static_cast<int>(atoi(tires_atrib[i].substr(j, tires_atrib[i].size() - j).c_str()));
			}
		}
		for (int i2 = 0; i2 <= y - x; x++)
		{
			total_points += (factorial(y) / (factorial(y - x)*factorial(x)) * power(0.5f, x) * power(0.5f, y - x));
		}
	}
	return total_points;
}
int Participant::CarPoints(std::string cars_path)
{
	std::vector<int> car_params;
	int total_points = 0;
	car_params = network_role->GetCarParameters(cars_path);
	for (int i = 0; i < car_params.size(); i++)
	{
		switch (i)
		{
			case CarModifiers::max_accelerating:
			{
				if (ai_type == 3)
					total_points += car_params[i] * 10;
				else
					total_points += car_params[i] * 5;
				break;
			}
			case CarModifiers::max_speed:
			{
				if (ai_type == 1 && car_params[i] > 200)
					total_points += 200 + car_params[i] - 200 / 5;
				else if (ai_type == 2 && car_params[i] > 150)
					total_points += 150 + car_params[i] - 150 / 5;
				else
					total_points += car_params[i];
				break;
			}
			case CarModifiers::max_braking:
			{
				if (ai_type == 1 && car_params[i] > 50)
					total_points += 50;
				else if (ai_type == 2 && car_params[i] > 30)
					total_points += 30;
				else if (ai_type != 3)
					total_points += car_params[i];
				break;
			}
			case CarModifiers::hand_brake_value:
			{
				if (ai_type == 1 && car_params[i] > 50)
					total_points += 50;
				else if (ai_type == 2)
					total_points -= car_params[i] * 5;
				else if (ai_type != 3)
					total_points += car_params[i];
				break;
			}
			case CarModifiers::durability:
			{
				if (car_params[i] < 100)
					total_points += car_params[i];
				else if (car_params[i] < 200)
					total_points += 100 + (car_params[i] - 100) / 2;
				else if (car_params[i] < 400)
					total_points += 150 + (car_params[i] - 200) / 10;
				else
					total_points += 175;

				break;
			}
			case CarModifiers::visibility:
			{
				total_points += car_params[i] * 10;
				break;
			}
			case CarModifiers::turn_mod:
			{
				if (ai_type == 1 || ai_type == 3)
					total_points += (car_params[i] - 100) * 3;
				else
					total_points += (car_params[i] - 100) / 2;
				break;
			}
			case CarModifiers::drift_mod:
			{
				if (ai_type == 2)
					total_points += (car_params[i] - 100) * 5;
				else
					total_points += (car_params[i] - 100);
				break;
			}
		}
	}
	return total_points;
	return 0;
}
void Participant::TakeAction(int safe_speed, bool turn) {

	switch (ai_type)
	{
	case 2://speedy guy
	{
		if (current_durability < current_speed * 2 && current_speed>safe_speed * 2 / 3)
		{
			current_speed -= car_modifiers[CarModifiers::hand_brake_value];
			if (current_speed == 0)
				current_speed = safe_speed * 0.7 + rand() % 5 - 2;
		}
		else if (turn && current_speed > 40 && rand() % 2 == 1)
		{
			drift = true;
			current_speed -= car_modifiers[CarModifiers::hand_brake_value];
		}
		else
		{
			current_speed += car_modifiers[CarModifiers::max_accelerating];
			if (current_speed > safe_speed*1.4)
				current_speed = safe_speed * 1.4 + rand() % 5 - 2;

		}
		break;
	}
	case 1://drifter
	{
		if (turn && current_speed > 40)
		{
			drift = true;
			current_speed -= car_modifiers[CarModifiers::hand_brake_value];
		}
		else if (current_durability < current_speed * 2 && current_speed>safe_speed * 0.7)
		{
			current_speed -= car_modifiers[CarModifiers::max_braking];

			if (current_speed < safe_speed * 0.7)
				current_speed = safe_speed * 0.7 + rand() % 5 - 2;
		}
		else if (current_speed > safe_speed*1.4)
		{
			current_speed -= car_modifiers[CarModifiers::max_braking];
			if (current_speed < safe_speed * 1.4)
				current_speed = safe_speed * 1.4 + rand() % 5 - 2;
		}
		else
			current_speed += car_modifiers[CarModifiers::max_accelerating];

		break;
	}
	case 0://safe guy
	{
		if (turn && current_speed > 40 && current_speed < 150 && rand() % 2 == 1)
		{
			drift = true;
			current_speed -= car_modifiers[CarModifiers::hand_brake_value];
		}
		else if (current_durability < current_speed * 2 || current_speed>safe_speed*1.1)
		{
			current_speed -= car_modifiers[CarModifiers::max_braking];

			if (current_speed < safe_speed)
				current_speed = safe_speed + rand() % 5 - 2;
		}
		else
		{
			current_speed += car_modifiers[CarModifiers::max_accelerating];
			if (current_speed > safe_speed)
				current_speed = safe_speed + rand() % 5 - 2;
		}

		break;
	}
	}
	if (current_speed > car_modifiers[CarModifiers::max_speed])
		current_speed = car_modifiers[CarModifiers::max_speed];

	else if (current_speed < 0)
		current_speed = 0;

	current_speed = static_cast<int>(static_cast<float>(current_speed)*0.9f);
}

void Participant::Test(std::string field)
{
	if (field.size() == 1)
	{
		int r = rand() % 8;
		int lost = (attacked - r)*rand() % 5 * 5;
		if (attacked > r)
		{
			current_speed /= attacked - r;
			current_durability -= lost;
			//if (show)
				//infobox(name + " lost " + NumberToString(lost) + " durability", ", because of enemies attacks", color, color2);
		}
		attacked = 0;
		return;
	}



	const char Chelper = field[0];
	field.erase(0, 1);
	std::string tire = tire_modifiers[atoi(&Chelper)];
	std::string helper = tire;
	int find = tire.find("x");
	int reqired_tests = atoi(helper.erase(find, helper.size() - find).c_str());
	int number_of_tests = atoi(tire.erase(0, find + 1).c_str());
	int passed_tests = 0;
	int max = 0, min = 100;
	float local_score;
	float formula;
	float base = static_cast<float>(current_speed) - static_cast<float>(atof(field.c_str()));


	if (base < 0)
		base = 0;



	if (drift == true)
	{
		base *= 10000.0f / static_cast<float>(car_modifiers[CarModifiers::drift_mod]) + static_cast<float>(5 * attacked);
		if (base > 100.0f)
			base = 100.0f;
		formula = (current_speed + base) / 2;
	}
	else
	{
		base *= 10000.0f / static_cast<float>(car_modifiers[CarModifiers::turn_mod]) + static_cast<float>(0.15f * attacked);
		if (base > 100.0f)
			base = 100.0f;
		formula = 1.0f / 3.0f*sqrt(10000.0f - (100.0f - base)*(100.0f - base)) + 2.0f / 3.0f*base;
	}

	attacked = 0;

	for (int i = 0; i < number_of_tests; i++)
	{
		local_score = static_cast<float>(rand() % 100) + static_cast<float>(rand() % 100 + 1) / 100.0f;

		if (local_score > formula)
		{
			if (local_score > max)
				max = static_cast<int>(local_score);

			passed_tests++;
		}
		else if (local_score < min)
		{
			min = static_cast<int>(local_score);
		}
	}

	if (passed_tests >= reqired_tests)
	{
		//if (show)
			//infobox(name + " have manage to turn, ", "required - " + NumberToString(static_cast<int>(formula)) + " highest roll - " + NumberToString(max), color, color2);
	}
	else
	{
		//if (show)
			//infobox(name + " had mistaken, ", "required - " + NumberToString(static_cast<int>(formula)) + " lowest roll - " + NumberToString(min), color, color2);

		if (formula > static_cast<float>(min + 50))
		{
			current_durability -= current_speed * (100.0f + formula - static_cast<float>(min)) / 50.0f;
			//if (show)
				//infobox(name + " badly crashed !!! ", name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 50) + " durability", color, color2);
			current_speed = 0;
		}
		else if (formula > static_cast<float>(min + 40))
		{
			current_durability -= current_speed * (100.0f + formula - static_cast<float>(min)) / 75.0f;
			//if (show)
				//infobox(name + " crashed !!!, ", name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 75) + " durability", color, color2);
			current_speed = 0;
		}
		else if (formula > static_cast<float>(min + 30))
		{

			current_durability -= current_speed * (100.0f + formula - static_cast<float>(min)) / 120.0f;
			//if (show)
				//infobox(name + " had an dangerous accident, ", name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 120) + " durability", color, color2);
			current_speed /= 10;
		}
		else if (formula > static_cast<float>(min + 20))
		{
			current_durability -= current_speed;
			//if (show)
				//infobox(name + " got off the route, ", name + " lost " + NumberToString(current_speed) + " durability", color, color2);
			current_speed /= 5;
		}
		else if (formula > static_cast<float>(min + 10))
		{
			current_durability -= current_speed / 2;
			//if (show)
				//infobox(name + " fell into a dangerous slip, ", name + " lost " + NumberToString(current_speed / 2) + " durability", color, color2);
			current_speed /= 2;
		}
		else
		{
			current_speed = static_cast<int>(static_cast<int>(current_speed) / 1.2f);
			//if (show)
				//infobox(name + " slipped, ", name + " lost a little bit of speed", color, color2);
		}

		if (current_durability <= 0)
		{
			//infobox("RIP, " + name + " dezintegrated his vehichle...", "", color, color2);
			
		}
	}
}

