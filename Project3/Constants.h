#pragma once
#include <string>

namespace VectorOfStrings 
{
	const std::vector<std::string> car_modifiers = { "max_speed","max_accelerating","max_braking","hand_brake_value","durability","visibility","turn_mod","drift_mod" };
	const std::vector<std::string> tire_modifiers = { "asphalt","grass","gravel","sand","mud","ice" };
	const std::vector<std::string> main_menu_options = { "New Game ", "Multiplayer ", "Ranking ", "Options ", "Credits ", "Exit" };
	const std::vector<std::string> game_lobby_options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	const std::vector<std::string> game_mode_options = { "Host game", "Search game", "Back" };
	const std::vector<std::string> game_options_options = { "Main Color", "Secondary Color", "Music playing", "Hamachi connection", "Back" };
	const std::vector<std::string> game_options_colors = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green", "light cyan","light red","light magenta","yellow","white" };
	const std::vector<std::string> race_actions = { "Speed up","Slow down","Hand-Brake","Do nothing","Abaddon Race" };
	const std::vector<std::string> race_chances = { "Chance: ", "Estimated time: ", "Durability burning: " };
	const std::vector<std::string> race_ranking = { "PLACE", "RACER", "SCORE" };
	const std::vector<std::string> race_interface = { "You can speed up by (0 to " , "You can slow down by (0 to " , "Hand brake value is - ", "You can just wait if you're moving..." ,"Remember only weak ones surrenders" };
	const std::vector<std::string> race_attribs = { " You're racing as: ",  " Your current speed is: ", " Your vehice has ", " durability   " };
	const std::vector<std::string> race_distance = { "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ", "At a considerable distance: ", "Far ahead: ", "Barely noticeable: " };
	const std::vector<std::string> race_boxes = { "Infobox  " , "Attack box" , "Chance of succeeding" };
	const std::vector<std::string> race_infobox = { "Turn on asphalt - safe speed is around ", "Straight road, terrain is asphalt", "Turn on grass - safe speed is around ", "Straight grassy road" , "Turn on gravel - safe speed is around " ,
	"Straight road, gravel", "Turn on sand - safe speed is around ", "Linear part of tour, but sandy",  "Turn on mud - safe speed is around ", "Muddy road, without turn" , "Turn on ice - safe speed is around ", "Slippery road with ice" };
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
	const std::vector<std::string> title_main = {
		"  _______  ___         ___     ",
		"     |    |   | |   | |   |    ",
		"     |    |   | |   | |___|    ",
		"     |    |   | |   | |  \\     ",
		"     |    |___| |___| |   \\    ",
		"                               ",
		"                               ",
		"                               ",
		"                               ",
		"                               ",
		"_______        ___    __   __  ",
		"   |    |   | |   |  |  | |    ",
		"   |    |   | |___|  |  | |__  ",
		"   |    |   | |  \\   |  |    | ",
		"   |    |___| |   \\  |  | ___| ",
	};
	const std::vector<std::string> title_additional = {
		" ___   ___",
		"|   | |   ",
		"|   | |_  ",
		"|   | |   ",
		"|___| |   ",
	};
}
namespace String 
{
	const std::string default_name = "Racer";
	const std::string action_confirm = " - Do you really want to do this ? (Y/N) ";
	const std::string infobox_RIP1 = "RIP, ";
	const std::string infobox_RIP2 = " dezintegrated his vehichle...";
	const std::string attack = "Don't attack";
	const std::string unable_to_move = " - You can't do this because you aren't moving...";
	const std::string meta = "META";

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