#pragma once
#include "NetworkRole.h"
#include "Participant.h"


class Race {
	std::vector<Participant*> participants;
	std::vector<float> scores;
	std::string tour_path;
	Window *main_window;

public:
	Race(Window &window);
	void Lobby(SinglePlayer *network_role);
	void Game();
	void Ending();
};

	

