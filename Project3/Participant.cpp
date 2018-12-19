#include "Participant.h"
#include "NetworkRole.h"


Participant::Participant(std::string name, std::string car_path, std::string tire_path, SinglePlayer &network_role)
{
	this->name = name;
	this->car_path = car_path;
	this->network_role = &network_role;
	this->car_modifiers = network_role.GetCarParameters(car_path);
	this->tire_modifiers = network_role.GetTireParameters(tire_path);
	this->current_speed = 0.0f;
	this->score = 0.0f;
	this->current_durability = static_cast<float>(car_modifiers[CarModifiers::durability]);
}

Participant::Participant(SinglePlayer * network_role, std::string tour_path)
{

	this->network_role = network_role;
	this->current_speed = 0.0f;
	this->score = 0.0f;

	static std::string names[] =
	{ "Paul Harackovy", "Mark Driver", "Max McDonald", "Gordon Goodman", "Miguela Aguela", "Hans Ufner", "Isao Fujimoto", "Igor Belov",
	"John Hill", "Andrew Anderson", "Jane Turning", "Lester King", "Drew McNeil", "Sam Samson","Gaston Reveneu", "Roman Torbon",
	"Helga Dick", "Mogore the Ogre", "David Black", "Reta Rdest", "Bloodwyn", "Quality Racer", "Sui Cide", "Ivan Atakovic" };
	std::string BestCarPath;
	std::string BestTirePath;
	std::string helper;

	ai_type = rand() % 3;

	static int used_names = 0;
	int RandomName = rand() % (23 - used_names);

	helper = names[RandomName];
	names[RandomName] = names[23 - used_names];
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
				if (ai_type == 2)
					total_points += car_params[i] * 10;
				else
					total_points += car_params[i] * 5;
				break;
			}
			case CarModifiers::max_speed:
			{
				if (ai_type == 0 && car_params[i] > 200)
					total_points += 200 + car_params[i] - 200 / 5;
				else if (ai_type == 1 && car_params[i] > 150)
					total_points += 150 + car_params[i] - 150 / 5;
				else
					total_points += car_params[i];
				break;
			}
			case CarModifiers::max_braking:
			{
				if (ai_type == 0 && car_params[i] > 50)
					total_points += 50;
				else if (ai_type == 1 && car_params[i] > 30)
					total_points += 30;
				else if (ai_type != 2)
					total_points += car_params[i];
				break;
			}
			case CarModifiers::hand_brake_value:
			{
				if (ai_type == 0 && car_params[i] > 50)
					total_points += 50;
				else if (ai_type == 1)
					total_points -= car_params[i] * 5;
				else if (ai_type != 2)
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
				if (ai_type == 0 || ai_type == 2)
					total_points += (car_params[i] - 100) * 3;
				else
					total_points += (car_params[i] - 100) / 2;
				break;
			}
			case CarModifiers::drift_mod:
			{
				if (ai_type == 1)
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
