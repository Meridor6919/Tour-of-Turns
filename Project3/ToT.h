#pragma once
#include <fstream>
#include "NetworkRole.h"
#include "ToT_Window.h"

namespace GameMode_ {
	void Game(bool multiplayer, ToT_Window &main_window);
	void Credits(ToT_Window &main_window);
	void Options(ToT_Window &main_window);
	void Ranking(ToT_Window &main_window);
}