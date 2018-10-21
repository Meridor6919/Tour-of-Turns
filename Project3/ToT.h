#pragma once
#include <fstream>
#include "TextLib.h"
#include "Window.h"
#include "Race.h"

namespace GameMode_
{
	void Game(bool multiplayer, Window &main_window);
	void Credits(Window &main_window);
	void Options(Window &main_window);
	void Ranking(Window &main_window);
}