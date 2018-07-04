#include "Enemy.h"

Enemy::Enemy(std::string tour)
{
	std::fstream fvar;
	std::string BestCarPath;
	std::string BestTirePath;
	std::string helper;
	AI = rand() % 3;
	name = NumberToString(AI);

	static std::string names[] =
	{ "Paul Harackovy", "Mark Driver", "Max McDonald", "Gordon Goodman", "Miguela Aguela", "Hans Ufner", "Isao Fujimoto", "Igor Belov",
	"John Hill", "Andrew Anderson", "Jane Turning", "Lester King", "Drew McNeil", "Sam Samson","Gaston Reveneu", "Roman Torbon",
	"Helga Dick", "Mogore the Ogre", "David Black", "Reta Rdest", "Bloodwyn", "Quality Racer", "Sui Cide", "Ivan Atakovic" };
	static int used_names = 0;
	int RandomName = rand() % (23 - used_names);

	helper = names[RandomName];
	names[RandomName] = names[23 - used_names];
	name = helper;
	used_names++;


	fvar.open(tour.c_str());
	for(int CurrentBest = 0, helperint;std::getline(fvar, helper);)
	{
		if (helper == "")
			break;

		helperint = CarPoints(helper);

		if (helperint > CurrentBest)
		{
			CurrentBest = helperint;
			BestCarPath = helper;
		}
	}
	fvar.close();

	int Arr[6] = { 0,0,0,0,0,0 };
	fvar.open(tour.c_str());

	while (std::getline(fvar, helper))
	{
		if (helper == "")
			break;
	}
	while (std::getline(fvar, helper))
	{
		Arr[static_cast<int>(helper[0]) - 48] += 1;
	}
	fvar.close();

	fvar.open("Tire.txt");
	for (float CurrentBest = 0, helperfloat; std::getline(fvar, helper);)
	{
		helperfloat = TirePoints(Arr, helper);
		if (helperfloat > CurrentBest)
		{
			CurrentBest = helperfloat;
			BestTirePath = helper;
		}
	}
	LoadParam(BestCarPath, BestTirePath);
	fvar.close();
}
int Enemy::CarPoints(std::string car)
{
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
}
int Enemy::Attack(float Scores[], int rivals)
{
	int *List = new int[rivals + 1];
	int l = 1;
	List[0] = 10;

	for (int i = 0; i < rivals; i++)
	{
		if (Scores[i]<Score + 5 && Scores[i]>Score - 5)
		{
			List[l] = i;
			l++;
		}
	}
	return List[rand() % l];
}
float Enemy::TirePoints(int arr[], std::string tire)
{
	float total_points = 0.0f;
	float x, y;
	std::string helper;
	std::fstream fvar;

	fvar.open(tire.c_str());
	for (int i = 0;std::getline(fvar, helper);i++)
	{
		for (int i = 0; i < static_cast<int>(helper.size()); i++)
		{
			if (helper[i] == 'x')
			{
				tire = helper;
				x = static_cast<float>(atof(helper.erase(i, helper.size() - i).c_str()));
				y = static_cast<float>(atof(tire.erase(0,i+1).c_str()));
				break;
			}
		}
		for (int i2 = 0; i2 <= y - x; x++)
		{
			total_points += (factorial(y)/(factorial(y-x)*factorial(x)) * Power(0.5f, x) * Power(0.5f, y-x)) * arr[i];
		}
	}
	return total_points;
}
bool Enemy::Action(std::string Path, int position, int color, int color2)
{
	std::fstream fvar;
	std::string helper;
	std::string field;
	bool flag = true;
	int safe_speed = static_cast<int>(Atrib[MaxSpeed]);

	fvar.open(Path.c_str());

	while(std::getline(fvar, helper))
	{
		if (helper == "")
			break;
	}
	for (int i = 0; i < position && getline(fvar, helper); i++);
	for (int i = 0; i < Atrib[Visibility] && getline(fvar, helper); i++)
	{
		if(i==0)
			field = helper;
		if (helper.size() > 1)
		{
			helper.erase(0, 1);
			if (atoi(helper.c_str())+(i*Atrib[MaxBraking]) < safe_speed)
				safe_speed = atoi(helper.c_str()) + (i*static_cast<int>(Atrib[MaxBraking]));
		}
		
	}
	fvar.close();
	
	switch (AI)
	{
		case 2://speedy guy
		{
			if (Atrib[Durability] < current_speed * 2 && current_speed>safe_speed*2/3)
			{
				current_speed -= Atrib[MaxBraking];
				if (current_speed == 0)
					current_speed = safe_speed * 0.7 + rand() % 5 - 2;
			}
			else if (field.size() > 1 && current_speed>40 && rand()%2 == 1)
			{
				drift = true;
				current_speed -= Atrib[HandBrake];
			}
			else
			{
				current_speed += Atrib[Acceleration];
				if (current_speed > safe_speed*1.4)
					current_speed = safe_speed * 1.4 + rand() % 5 - 2;

			}
				

			break;
		}
		case 1://drifter
		{
			if (field.size() > 1 && current_speed>40)
			{
				drift = true;
				current_speed -= Atrib[HandBrake];
			}
			else if (Atrib[Durability] < current_speed * 2 && current_speed>safe_speed * 0.7)
			{
				current_speed -= Atrib[MaxBraking];

				if (current_speed < safe_speed * 0.7)
					current_speed = safe_speed * 0.7+ rand() % 5 - 2;
			}
			else if (current_speed > 150)
			{
				current_speed -= Atrib[MaxBraking];

				if (current_speed < 150)
					current_speed = 150 + rand() % 5 - 2;
			}
			else
				current_speed += Atrib[Acceleration];

			break;
		}
		case 0://safe guy
		{
			if (field.size() > 1 && current_speed>40 && current_speed<120 && rand()%2 == 1)
			{
				drift = true;
				current_speed -= Atrib[HandBrake];
			}
			else if (Atrib[Durability] < current_speed * 2 || current_speed>safe_speed)
			{
				current_speed -= Atrib[MaxBraking];

				if (current_speed < safe_speed)
					current_speed = safe_speed + rand()%5-2;
			}
			else
			{
				current_speed += Atrib[Acceleration];
				if (current_speed > safe_speed)
					current_speed = safe_speed + rand()%5-2;
			}

			break;
		}
	}
	if (current_speed > Atrib[MaxSpeed])
		current_speed = Atrib[MaxSpeed];
	else if (current_speed < 0)
		current_speed = 0;
	
	if (position>0)
		flag = Test(field, false, color, color2);
	
	current_speed = static_cast<int>(static_cast<float>(current_speed)*0.9f);

	if (drift == true)
	{
		drift = false;
		Score += 1.5;
	}
	else
		Score += 100.0f / (1.0f + current_speed * 10.0f / 36.0f);

	return flag;
}