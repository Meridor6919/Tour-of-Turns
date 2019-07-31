#pragma once
#include <fstream>
#include "NetworkRole.h"
#include "ToT_Window.h"

void Game(bool multiplayer, ToT_Window &main_window);
void Credits(ToT_Window &main_window);
void Options(ToT_Window &main_window);
void Ranking(ToT_Window &main_window);
void GameLobby(ToT_Window &window, std::shared_ptr<SinglePlayer> network_role);
