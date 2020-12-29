#pragma once
#include <string>

namespace ErrorTitle
{
	const std::string unplayable = "Unplayable Game";
	const std::string missing_file = "Missing Files";
	const std::string corrupted_file = "Corupted Files";
	const std::string pipe_error = "Pipe error";
	const std::string language_error = "Language pack critical error";
	const std::string placeholder_language = "Used different language";
	const std::string cheating_attempt = "Cheating detected";
	const std::string ai_connection = "AI module error";
}
namespace ErrorMsg
{
	const std::string unplayable = "You can't start new race with current game state, please repair files or reinstall game";
	const std::string missing_file = "Files not found, please reinstall your game or repair missing files";
	const std::string ranking_missing = "There are no ranking files";
	const std::string corrupted_file = " cannot be read, please repair or delete corrupted files";
	const std::string pipe_error = "Please call a plumber";
	const std::string language_error = "Game cannot find any valid language packs, please repair files or reinstall game";
	const std::string placeholder_language = " is current language. \nPreviously selected language pack has been corrupted";
	const std::string available_cars = " has no cars available";
	const std::string cheating_attempt = " has made an attempt to cheat. He is now eliminated :)";
	const std::string ai_connection = "There is an issue with your AI module, please repair files or reinstall the game";
}