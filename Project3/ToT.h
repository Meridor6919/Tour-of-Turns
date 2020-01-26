#pragma once
#include <fstream>
#include "NetworkRole.h"
#include "ToT_Window.h"
#include "Constants.h"

class ToT {
	std::shared_ptr<ToT_Window> main_window;
	HANDLE handle;
	short game_window_center;
	short info_pos = 0;
	short main_menu_position = 0;

	std::vector<std::string> GetRankingNames(std::string tour);
	void ShowRankingDetails(std::string tour, int racer_pos, int classification_type, bool clearing = false);

public:
	ToT();
	void Credits();
	void Options();
	void Ranking();
	void Info();
	void Game(const bool multiplayer);
	void MainMenu();
};