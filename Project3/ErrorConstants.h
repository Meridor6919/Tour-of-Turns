#pragma once
#include <string>

namespace ErrorTitle
{
	constexpr const char* unplayable = "Unplayable Game";
	constexpr const char* missing_file = "Missing Files";
	constexpr const char* corrupted_file = "Corupted Files";
	constexpr const char* pipe_error = "Pipe error";
	constexpr const char* language_error = "Language pack critical error";
	constexpr const char* placeholder_language = "Used different language";
	constexpr const char* cheating_attempt = "Cheating detected";
	constexpr const char* ai_connection = "AI module error";
	constexpr const char* comming_soon_tm = "Feature missing";
}
namespace ErrorMsg
{
	constexpr const char* comming_soon_tm = "This feature is not yet supported";
	constexpr const char* unplayable = "You can't start new race with current game state, please repair files or reinstall game";
	constexpr const char* missing_file = "Files not found, please reinstall your game or repair missing files";
	constexpr const char* corrupted_file = " cannot be read, please repair or delete corrupted files";
	constexpr const char* pipe_error = "Please call a plumber";
	constexpr const char* language_error = "Game cannot find any valid language packs, please repair files or reinstall game";
	constexpr const char* placeholder_language = " is current language. \nPreviously selected language pack has been corrupted";
	constexpr const char* available_cars = " has no cars available";
	constexpr const char* cheating_attempt = " has made an attempt to cheat. He is now eliminated :)";
	constexpr const char* ai_connection = "There is an issue with your AI module, please repair files or reinstall the game";
}