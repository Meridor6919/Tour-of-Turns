#pragma once
#include <fstream>
#include "Race.h"
#include "TextLib.h"
#include "Window.h"


namespace GameMode
{
	void Game(bool multiplayer, Window &main_window);
	void Credits(Window &main_window);
	void Options(Window &main_window);
	void Ranking(Window &main_window);
}