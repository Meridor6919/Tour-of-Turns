#pragma once
#include <fstream>
#include "NetworkRole.h"
#include "ToT_Window.h"
#include "Constants.h"


class ToT {
	ToT_Window* main_window;
	HANDLE handle;
	short game_window_center;
	short info_pos = 0;

public:

	ToT(ToT_Window*main_window);
	void Credits();
	void Options();
	void Ranking();
	void Info();
	void Game(const bool multiplayer);



};