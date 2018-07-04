#pragma once
#include "Racer.h"
#include "ToT.h"

class Player : public Racer
{
public:

	Player() {};
	Player(std::string current_name, std::string car, std::string tire);
	void VisionBox(std::vector<std::string> tour, int starting_pos, int color, int color2);
	bool EstimateScore(std::vector<std::string> tour, int starting_pos, int color, int color2);
	virtual bool Action(bool reset, int color, int color2);
	int Ranking(float EnemyScore[], std::string EnemyName[], int rivals, int width, int color, int color2);
	int Attack(float Scores[], std::string Names[], int rivals, int width, int color, int color2);
	void Interface(int color, int color2, int width);
	void SaveHighScore(std::string Path, int Place, int Rivals);
};
