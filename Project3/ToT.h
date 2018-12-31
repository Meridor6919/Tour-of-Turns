#pragma once
#include <fstream>
#include "Race.h"
#include "TextLib.h"
#include "ToT_Window.h"


namespace GameMode
{
	void Game(bool multiplayer, ToT_Window_ &main_window);
	void Credits(ToT_Window_ &main_window);
	void Options(ToT_Window_ &main_window);
	void Ranking(ToT_Window_ &main_window);
}