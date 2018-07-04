#pragma once
#include "Racer.h"
#include "ToT.h"

class Enemy : public Racer
{
	int AI;
	int CarPoints(std::string car);
	float TirePoints(int arr[], std::string tire);
public:
	Enemy(std::string tour);
	bool Action(std::string Path, int position, int color, int color2);
	int Attack(float Scores[], int rivals);
};

