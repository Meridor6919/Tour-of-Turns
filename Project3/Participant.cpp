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
	this->current_durability = static_cast<float>(car_modifiers[CarModifiers::durability]);

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
	for (int best_points = 0, int i = 0; i < cars.size(); i++)
	{
		int j = CarPoints(cars[i]);
		if (j > best_points)
		{
			best_points = j;
			current_best = i;
		}
	}
	this->car_modifiers = network_role->GetCarParameters(cars[current_best]);

	std::vector<std::string> tires;
	network_role->GetTireNames(tires);
	
	current_best = 0;
	for (int i = 0, int best_points = 0; i < tires.size(); i++)
	{
		int j = TiresPoints(tires[i]);
		if (j > best_points)
		{
			best_points = j;
			current_best = i;
		}
	}
	this->car_modifiers = network_role->GetCarParameters(cars[current_best]);
}
int Participant::TiresPoints(std::string tires_path)
{
	float total_points = 0.0f;
	float x, y;
	std::string helper;

	std::vector<std::string>tires_atrib = network_role->GetTireParameters(tires_path);
	for (int i = 0; i < tires_atrib.size(); i++)
	{

		for (int j = 0; j < tires_atrib[i].size(); j++)
		{
			if (tires_atrib[i][j] == 'x')
			{
				x = static_cast<float>(atof(tires_atrib[i].substr(0, j).c_str()));
				y = static_cast<float>(atof(tires_atrib[i].substr(j, tires_atrib[i].size() - j).c_str()));
			}
		}
	}
	//need to validate

	/*
		for (int i2 = 0; i2 <= y - x; x++)
		{
			total_points += (factorial(y)/(factorial(y-x)*factorial(x)) * Power(0.5f, x) * Power(0.5f, y-x)) * arr[i];
		}
	}
	return total_points;
	*/
	return 0;
}
int Participant::CarPoints(std::string cars_path)
{
	/*
	std::fstream fvar;
	std::string helper;
	int total_points = 0;
	int i;

	fvar.open(car.c_str());
	for (i = 0; std::getline(fvar, helper); i++)
	{
		switch (i)
		{
			case Acceleration:
			{
				if (AI == 2)
					total_points += atoi(helper.c_str()) * 10;
				else
					total_points += atoi(helper.c_str()) * 5;
				break;
			}
			case MaxSpeed:
			{
				if (AI == 0 && atoi(helper.c_str()) > 200)
					total_points += 200 + (atoi(helper.c_str()) - 200) / 5;
				else if (AI == 1 && atoi(helper.c_str()) > 150)
					total_points += 150 + (atoi(helper.c_str()) - 150) / 5;
				else
					total_points += atoi(helper.c_str());
				break;
			}
			case MaxBraking:
			{
				if (AI == 0 && atoi(helper.c_str()) > 50)
					total_points += 50;
				else if (AI == 1 && atoi(helper.c_str()) > 30)
					total_points += 30;
				else if (AI != 2)
					total_points += atoi(helper.c_str());
				break;
			}
			case HandBrake:
			{
				if (AI == 0 && atoi(helper.c_str()) > 50)
					total_points += 50;
				else if (AI == 1)
					total_points -= atoi(helper.c_str()) * 5;
				else if (AI != 2)
					total_points += atoi(helper.c_str());
				break;
			}
			case Durability:
			{
				if (atoi(helper.c_str()) < 100)
					total_points += atoi(helper.c_str());
				else if (atoi(helper.c_str()) < 200)
					total_points += 100 + (atoi(helper.c_str()) - 100) / 2;
				else if (atoi(helper.c_str()) < 400)
					total_points += 150 + (atoi(helper.c_str()) - 200) / 10;
				else
					total_points += 175;
				break;
			}
			case Visibility:
			{
				total_points += atoi(helper.c_str()) * 10;
				break;
			}
			case TurnEffectivnes:
			{
				if (AI == 0 || AI == 2)
					total_points += (atoi(helper.c_str()) - 100) * 3;
				else
					total_points += (atoi(helper.c_str()) - 100) / 2;
				break;
			}
			case DriftEffectivnes:
			{
				if (AI == 1)
					total_points += (atoi(helper.c_str()) - 100) * 5;
				else
					total_points += (atoi(helper.c_str()) - 100);
				break;
			}
		}
	}
	if (i < 7)
	{
		helper = "Car file " + car + " is corrupted. Please repair file before you will launch a game again";
		MessageBox(0, helper.c_str(), ERROR, 0);
		exit(0);
	}
	fvar.close();
	return total_points;
	*/
	return 0;
}
