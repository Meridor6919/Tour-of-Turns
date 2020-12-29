#pragma once
#include <fstream>

#include "NetworkRole.h"
#include "NetworkRoleHost.h"
#include "NetworkRoleClient.h"
#include "ToT_Window.h"
#include "ErrorConstants.h"
#include "LanguagePack.h"

using namespace MeridorConsoleLib;

class ToT {
	std::shared_ptr<ToT_Window> main_window;
	HANDLE handle;
	short game_window_center;
	short info_pos = 0;
	short main_menu_position = 0;

	bool playable;
	bool ranking_enabled;

	void ShowRankingDetails(std::string tour, int racer_pos, int classification_type, bool clearing = false);
	void Title();

public:
	ToT();
	void CheckPlayablity();
	void CheckRankingValid();
	void CheckLangPackValid();
	
	void Credits();
	void Options();
	void Ranking();
	void Info();
	void Game(const bool multiplayer);
	void MainMenu();
};