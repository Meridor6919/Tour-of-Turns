#pragma once
#include <string>

namespace VectorOfStrings {
	const std::vector<std::string> car_modifiers = { "max_speed","max_accelerating","max_braking","hand_brake_value","durability","visibility","turn_mod","drift_mod" };
	const std::vector<std::string> tire_modifiers = { "asphalt","grass","gravel","sand","mud","ice" };
	const std::vector<std::string> main_menu_options = { "New Game ", "Multiplayer ", "Ranking ", "Options ", "Credits ", "Exit" };
	const std::vector<std::string> game_lobby_options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	const std::vector<std::string> game_mode_options = { "Host game", "Search game", "Back" };
	const std::vector<std::string> game_options_options = { "Main Color", "Secondary Color", "Music playing", "Hamachi connection", "Back" };
	const std::vector<std::string> game_options_colors = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green", "light cyan","light red","light magenta","yellow","white" };
	const std::vector<std::string> on_off = { "on", "off" };
	const std::vector<std::string> credits = {
		"Hello I am Michal Balicki and I am really happy that you are playing my game.",
		"Tour of Turns was my first real project in my programming way and I hope you like it.",
		"I would like to thank people who helped me with this project.",
		"Krystian Grzegorzewicz ",
		"without him the game wouldn't be created.",
		"He was patiently listening what problems do I have with ToT and the most important he gave me a main idea of the game.",
		"Marek Loek ",
		"for making an icon image and inspiring me to hard work.",
		"Marcel Marzec ",
		"for giving me inspiration.",
		"I admire how much work he put in Pan Zniszczenia map for HoMM3 based on Diablo 2 universum.",
		"Miroslaw Zelent ",
		"for getting me interested in programming"
	};
}
namespace CarModifiers
{
	enum
	{
		max_speed,
		max_accelerating,
		max_braking,
		hand_brake_value,
		durability,
		visibility,
		turn_mod,
		drift_mod,
		last
	};
}
namespace TireModifiers
{
	enum
	{
		asphalt,
		grass,
		gravel,
		sand,
		mud,
		ice,
		last
	};
}
namespace FolderName
{
	const std::string tour = "Maps";
	const std::string car = "Cars";
	const std::string tire = "Tires";
	const std::string main = "Game_Files";
}
namespace FileName
{
	const std::string tire = "Tire.txt";
	const std::string tour = "Tour.txt";
	const std::string ranking = "Ranking.txt";
	const std::string config = "config.txt";
	const std::string music = "test.wav";
}
namespace ExtName
{
	const std::string tour = "tour";
	const std::string car = "car";
	const std::string tire = "tire";
	const std::string ranking = "rank";
}
namespace Error_Msg
{
	const std::string unplayable = "You can't start new race with current game state, please repair files or reinstall game";
	const std::string missing_file = "Files not found, please reinstall your game or repair missing files";
	const std::string ranking_not_present = "There are no ranking files";
	const std::string corrupted_file = " cannot be read, please repair or delete corrupted files";
}
namespace Error_Title
{
	const std::string unplayable = "Unplayable Game";
	const std::string missing_file = "Missing Files";
	const std::string corrupted_file = "Corupted Files";

}