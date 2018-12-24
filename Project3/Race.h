#pragma once
#include "NetworkRole.h"
#include "Participant.h"


class Race {
	std::vector<Participant*> *participants;
	std::vector<float> scores;
	std::string tour_path;
	ToT_Window *main_window;

	void Interface();
	void VisionBox(std::vector<std::string> visible_tour);
	void Attack();
	int Ranking();

public:
	Race(ToT_Window &window, std::vector<Participant*> *participants);
	void Lobby(SinglePlayer *network_role);
	void Game();
	void Ending();
};

	

