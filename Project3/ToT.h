#pragma once
#include <fstream>
#include "NetworkRole.h"
#include "ToT_Window.h"
#include "Constants.h"

namespace GameMode 
{
	void Credits(ToT_Window &main_window);
	void Options(ToT_Window &main_window);
	void Ranking(ToT_Window &main_window);
	void Info(ToT_Window &main_window);
	void Game(const bool multiplayer, ToT_Window &main_window);
}