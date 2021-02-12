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
	unsigned short info_pos = 0;
	unsigned short main_menu_position = 0;

	bool playable;

	void ShowRankingDetails(std::string tour, int racer_pos, int classification_type);
	void ClearRankingDetails();

	void SetTheme(const COORD& local_starting_point);
	void SetColor(const COORD& local_starting_point, bool main);
	void SetMusic(const COORD& local_starting_point);
	void SetLanguage(const COORD& local_starting_point);
	void SetHamachiFlag(const COORD& local_starting_point);
	void SetDisplayMode(const COORD& local_starting_point);
	void SetAIModule(const COORD& local_starting_point);

public:
	ToT();
	
	void Credits();
	void Options();
	void Ranking();
	void Info();
	void Game(const bool multiplayer);
	void MainMenu();
};