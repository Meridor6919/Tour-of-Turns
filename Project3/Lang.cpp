#pragma once
#include "Constants.h"

namespace VectorOfStrings
{
	std::vector<std::string> car_modifiers = { "max_speed","max_accelerating","max_braking","hand_brake_value","durability","visibility","turn_mod","drift_mod" };
	std::vector<std::string> tire_modifiers = { "asphalt","grass","gravel","sand","mud","ice" };
	std::vector<std::string> main_menu_options = { "New Game ", "Multiplayer ", "Ranking ", "Options ", "Credits ", "Exit" };
	std::vector<std::string> game_lobby_options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	std::vector<std::string> game_mode_options = { "Host game", "Search game", "Back" };
	std::vector<std::string> game_options_options = { "Main Color", "Secondary Color", "Music playing", "Hamachi connection", "Back" };
	std::vector<std::string> game_options_colors = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green", "light cyan","light red","light magenta","yellow","white" };
	std::vector<std::string> race_actions = { "Speed up","Slow down","Hand-Brake","Do nothing","Abaddon Race" };
	std::vector<std::string> race_chances = { "Chance: ", "Estimated time: ", "Durability burning: ", "Estimated speed: " };
	std::vector<std::string> race_ranking = { "PLACE", "RACER", "SCORE" };
	std::vector<std::string> race_interface = { "You can speed up by (0 to " , "You can slow down by (0 to " , "Hand brake value is - ", "You can just wait if you're moving..." ,"Remember only weak ones surrenders" };
	std::vector<std::string> race_attribs = { " You're racing as: ",  " Your current speed is: ", " Your vehice has ", " durability   " };
	std::vector<std::string> race_distance = { "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ", "At a considerable distance: ", "Far ahead: ", "Barely noticeable: " };
	std::vector<std::string> race_boxes = { "Infobox  " , "Attack box" , "Chance of succeeding" };
	std::vector<std::string> race_infobox = { "Turn on asphalt - safe speed is around ", "Straight road, terrain is asphalt", "Turn on grass - safe speed is around ", "Straight grassy road" , "Turn on gravel - safe speed is around " ,
	"Straight road, gravel", "Turn on sand - safe speed is around ", "Linear part of tour, but sandy",  "Turn on mud - safe speed is around ", "Muddy road, without turn" , "Turn on ice - safe speed is around ", "Slippery road with ice" };
	std::vector<std::string> participant_names = { "Paul Harackovy", "Mark Driver", "Max McDonald", "Gordon Goodman", "Miguela Aguela", "Hans Ufner", "Isao Fujimoto", "Igor Belov",
	"John Hill", "Andrew Anderson", "Jane Turning", "Lester King", "Drew McNeil", "Sam Samson", "Gaston Reveneu", "Roman Torbon",
	"Helga Dick", "Mogore the Ogre", "David Black", "Reta Rdest", "Bloodwyn", "Quality Racer", "Sui Cide", "Ivan Atakovic", "Blu Apostrof" };
	std::vector<std::string> participant_infobox = { " have manage to turn, ", " had mistaken, ", " badly crashed !!! ", " crashed !!!, " ," had an dangerous accident, " ," got off the route, " ," fell into a dangerous slip, " ," slipped, " };
	std::vector<std::string> on_off = { "on", "off" };
	std::vector<std::string> credits = {
		"Hello I am Michal Balicki and I am really happy that you are playing my game.",
		"Tour of Turns was my first real project in my programming way and I hope you like it.",
		"I would like to thank people who helped me with this project.",
		"Krystian Grzegorzewicz ",
		"without him the game wouldn't be created.",
		"He was patiently listening what problems do I have with ToT and the most important he gave me a main idea of the game.",
		"Marek Loek ",
		"for making an icon image and inspiring me to hard work.",
		"Marcin Marzec ",
		"for giving me inspiration.",
		"I admire how much work he put into Pan Zniszczenia and Zniwiarz Dusz maps for HoMM3 based on Diablo universe.",
		"Miroslaw Zelent ",
		"for getting me interested in programming"
	};
	std::vector<std::string> title_main = {
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
	std::vector<std::string> title_additional = {
		" ___   ___",
		"|   | |   ",
		"|   | |_  ",
		"|   | |   ",
		"|___| |   ",
	};
	std::vector<std::string> error_msg = { "You can't start new race with current game state, please repair files or reinstall game", "Files not found, please reinstall your game or repair missing files" ,
		"There are no ranking files" , " cannot be read, please repair or delete corrupted files" };
	std::vector<std::string> error_title = { "Unplayable Game",  "Missing Files", "Corupted Files" };
	std::vector<std::string> other_string = { "Racer", " - Do you really want to do this ? (Y/N) ", "RIP, ", " dezintegrated his vehichle...","Don't attack" ," - You can't do this because you aren't moving...",
		"META" , " lost " , "required - ", " highest roll - ", " lowest roll - ", "in result of other racers behaviour", " speed,", "________________________________________________________  " };
}