#pragma once
#include <string>

namespace ErrorTitle
{
	constexpr const char* corrupted_file = "Corupted file";
	constexpr const char* missing_file = "Missing file";
	constexpr const char* repaired = "Repaired broken files";
	constexpr const char* placeholder_language = "Used different language";
	constexpr const char* ai_error = "AI module error";
	constexpr const char* pipe_error = "Pipe error";
	constexpr const char* cheating_attempt = "Cheating detected";
	constexpr const char* comming_soon_tm = "Feature missing";
	constexpr const char* unplayable = "Unplayable game";
	constexpr const char* ranking_absent = "Ranking not available";
	constexpr const char* music_unavailable = "Ranking not available";
}
namespace ErrorMsg
{
	constexpr const char* corrupted_file = " cannot be read, do you want to remove it?";
	constexpr const char* missing_file = "Files not found, please reinstall your game or repair missing files";
	constexpr const char* repaired = "There was a problem with one of your game files, but application was able to fix it";
	constexpr const char* no_music = "Music file is missing, application won't be able to play any sounds";
	constexpr const char* no_title_theme = "Title theme file is missing, application created new \"empty\" file to fix the problem.\nUnfortunately there won't be any fancy title themes.";
	constexpr const char* no_game_config = "No game config was found. Application has created a new, default config";
	constexpr const char* no_window_config = "No window config was found. Application has created an emergency placeholder.\nUnfortunately you won't be able to launch a game, until window config file is restored";
	constexpr const char* no_game_files = "Some crucial game files are missing. You won't be able to launch the game.";
	constexpr const char* no_lang_pack = "Application cannot find any valid language packs, please repair files or reinstall game";
	constexpr const char* no_ranking = "No ranking files were found. You won't be able to access ranking panel.";
	constexpr const char* placeholder_language = " is current language. \nPreviously selected language pack has been corrupted";
	constexpr const char* ai_connection = "There is an issue with your AI module, please repair files or reinstall the game";
	constexpr const char* pipe_error = "Please call a plumber";
	constexpr const char* cheating_attempt = " has made an attempt to cheat. He is now eliminated :)";
	constexpr const char* comming_soon_tm = "This feature is not yet supported";
	constexpr const char* unplayable = "You cannot start new race with the current game state, please repair files or reinstall game.";
	constexpr const char* ranking_absent = "You cannot open ranking tab with the current game state, please repair files or reinstall game.";
	constexpr const char* music_unavailable = "You cannot access music options with the current game state, please repair files or reinstall game.";
}