#pragma once
#include "ToT.h"

//Enums for Attrib and Terrain arrays
enum AttribType
{
	Acceleration, MaxSpeed, MaxBraking, HandBrake, Durability, Visibility, TurnEffectivnes, DriftEffectivnes
};
enum TerrainType
{
	Asphalt, Grass, Gravel, Sand, Mud, Ice
};
//abstract class
class Racer
{
public:
	std::string name;
	int attacks;
	float Score;
	
protected:
	
	std::string Terrain[6];
	float Atrib[8];
	bool drift;
	int current_speed;
	
	//FUNCTIONS
	
	virtual bool Test(std::string field, bool show, int color, int color2)
	{
		if (field.size() == 1)
		{
			int r = rand() % 8;
			int lost = (attacks - r)*rand() % 5 * 5;
			if (attacks > r)
			{
				current_speed /= attacks-r;
				Atrib[Durability] -= lost;
				if(show)
					infobox(name + " lost " + NumberToString(lost) + " durability", ", because of enemies attacks", color, color2);
			}
			attacks = 0;
			return true;
		}
			

		
		const char Chelper = field[0];
		field.erase(0, 1);
		std::string tire = Terrain[atoi(&Chelper)];
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
			base *= 100.0f / static_cast<float>(Atrib[DriftEffectivnes]) + static_cast<float>(5*attacks);
			if (base > 100.0f)
				base = 100.0f;
			formula = (current_speed+base)/2;
		}
		else
		{
			base *= 100.0f / static_cast<float>(Atrib[TurnEffectivnes]) + static_cast<float>(0.15f * attacks);
			if (base > 100.0f)
				base = 100.0f;
			formula = 1.0f / 3.0f*sqrt(10000.0f - Power((100.0f - base), 2.0f)) + 2.0f / 3.0f*base;
		}

		attacks = 0;
		
		for (int i = 0; i < number_of_tests; i++)
		{
			local_score = static_cast<float>(rand() % 100) + static_cast<float>(rand() % 100+1) / 100.0f;

				if (local_score > formula)
				{
					if (local_score > max)
						max = static_cast<int>(local_score);

					passed_tests++;
				}
				else if(local_score < min)
				{
					min = static_cast<int>(local_score);
				}
		}
		
		if (passed_tests >= reqired_tests)
		{
			if (show)
				infobox(name + " have manage to turn, ", "required - " + NumberToString(static_cast<int>(formula)) + " highest roll - " + NumberToString(max), color, color2);
			return true;
		}
		else
		{
			if(show)
				infobox(name + " had mistaken, ", "required - " + NumberToString(static_cast<int>(formula)) + " lowest roll - " + NumberToString(min), color, color2);
			
			if (formula > static_cast<float>(min + 50))
			{
				Atrib[Durability] -= current_speed * (100.0f + formula - static_cast<float>(min)) / 50.0f;
				if (show)
					infobox(name + " badly crashed !!! ", name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 50) + " durability", color, color2);
				current_speed = 0;
			}
			else if (formula > static_cast<float>(min + 40))
			{
				Atrib[Durability] -= current_speed * (100.0f + formula - static_cast<float>(min)) / 75.0f;
				if (show)
					infobox(name + " crashed !!!, ", name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 75) + " durability", color, color2);
				current_speed = 0;
			}
			else if (formula > static_cast<float>(min + 30))
			{
				
				Atrib[Durability] -= current_speed * (100.0f + formula - static_cast<float>(min)) / 120.0f;
				if (show)
					infobox(name + " had an dangerous accident, ", name + " lost " + NumberToString(current_speed * (100 + static_cast<int>(formula) - min) / 120) + " durability", color, color2);
				current_speed /= 10;
			}
			else if (formula > static_cast<float>(min + 20))
			{
				Atrib[Durability] -= current_speed;
				if (show)
					infobox(name + " got off the route, ", name + " lost " + NumberToString(current_speed) + " durability", color, color2);
				current_speed /= 5;
			}
			else if (formula > static_cast<float>(min + 10))
			{
				Atrib[Durability] -= current_speed/2;
				if (show)
					infobox(name + " fell into a dangerous slip, ", name + " lost " + NumberToString(current_speed / 2) + " durability", color, color2);
				current_speed /= 2;
			}
			else 
			{
				current_speed = static_cast<int>(static_cast<int>(current_speed) / 1.2f);
				if (show)
					infobox(name + " slipped, ", name + " lost a little bit of speed", color, color2);
			}

			if (Atrib[Durability] <= 0)
			{
				infobox("RIP, "+name + " dezintegrated his vehichle...","", color, color2);
				return false;
			}
			else
				return true;
		}
	}
	void LoadParam(std::string p_Auto, std::string p_Tire)
	{
		std::string line;
		std::fstream file;
		int i;
		Score = 0.0f;
		current_speed = 0;
		attacks = 0;
		
		file.open(p_Auto.c_str(), std::ios::in);
		if (!file.good())
		{
			MessageBox(0, "Error can't find car file", "ERROR", 0);
			_getch();
			exit(0);
		}
		for (i = 0; std::getline(file, line); i++)
		{
			if (i > 7)
			{
				MessageBox(0, "Corrupted car file", "ERROR1", 0);
				_getch();
				exit(0);
			}
			Atrib[i] = static_cast<float>(atof(line.c_str()));
		}
		if (i < 7)
		{
			MessageBox(0, "Corrupted car file", "ERROR2", 0);
			_getch();
			exit(0);
		}

		file.close();

		file.open(p_Tire.c_str(), std::ios::in);
		if (!file.good())
		{
			MessageBox(0, "Error can't find tire file", "ERROR1", 0);
			_getch();
			exit(0);
		}

		for (i = 0; std::getline(file, line); i++)
		{
			if (i > 5)
			{
				MessageBox(0, "Corrupted tire file", "ERROR2", 0);
				_getch();
				exit(0);
			}
			Terrain[i] = line;
		}
		if (i < 5)
		{
			MessageBox(0, "Corrupted tire file", "ERROR", 0);
			_getch();
			exit(0);
		}
		file.close();

		
	}

};

