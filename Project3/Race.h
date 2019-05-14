#pragma once
#include "NetworkRole.h"
#include "Participant.h"

class Race {
	std::vector<Participant*> *participants; 
	std::vector<float> scores;
	std::string tour_path;
	ToT_Window *main_window;
	int ais;

	void Interface();
	void VisionBox(std::vector<std::string> visible_tour, int visibility);
	int Ranking(std::vector<std::pair<float, std::string>> &ranking_info, bool clear);

public:
	Race(ToT_Window &window, std::vector<Participant*> *participants);
	void Lobby(SinglePlayer *network_role);		//game states
	std::vector<std::pair<float, std::string>> Game();
	void Ending(std::vector<std::pair<float, std::string>> ranking_info);
};