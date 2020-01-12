#pragma once
#include "Constants.h"

namespace LanguagePack
{
	std::vector<std::vector<std::string>> vector_of_strings = {
		{ "Max speed","Max Acceleration","Max Braking","Hand Brake Value","Durability","Visibility","Turn Modifier","Drift Modifier" },
		{ "Asphalt","Grass","Gravel","Sand","Mud","Ice" },
		{ "New Game ", "Multiplayer ", "Game Information",  "Ranking ", "Options ", "Credits ", "Exit" },
		{ "Introduction", "Tours", "Cars", "Tires", "Formulas"},
		{ "","Tour of Turns is turn-based racing game where player with the lowest final score wins.",
			"","Before the start of the game, player selects tour on which he will race. Additionally he chooses a car and a tires that greatly impact chances of winning the race.",
			"","The actual race is divided into segments - parts of the tour on which player takes actions.",
			"","Each action consists of two different elements : speed changing phase and attack phase.",
			"","After completing the action, the player moves to the next segment and the new turn begins.",
			"","When player finishes the race his score is added to ranking file."},
		{ "",  "Tour is made out of segments that can be seen on the left side of the screen along with parameters of these types:",
			"Terrain type"," - tires attributes are directly based on this parameter.",
			"Road curviture", " - informs whether the selected segment is a turn or not.",
			"Safe speed", " - if the player surpasses this value he is being tested. Chance of succeeding is showed on the right side of the screen.",
			"", "Straight segments of the road doesnt't have any restrictions about safe speed."},
		{ "", "Selection of car is the single most important decision in Tour of Turns. It provides essencial atributes such as:",
			"Max Speed", " - Player's vehical retains about 90% of speed every turn. This parameter defines maximum speed accumulated over time. Player can surpass this value up to 125% but every point exceeding, costs durability.",
			"Max Acceleration", " - Maximum value by which player can speed up. It's influenced by terrain and according tire parameter.",
			"Max Braking", " - Maximum value by which player can slow down. It's influenced by terrain and according tire parameter.",
			"Hand Brake Value", " - Player can use hand brake, slowing down by a constant value. If player chooses this action on a non-straight segment of turn and he is driving faster than 40km/h then drift is performed.",
			"Durability", " - When the player loses all of durablility then he is out of the race. There are multiple sources that can limit this value. Firstly unsuccessful turning tests, secondly surpassing maximum speed and lastly enemy attacks.",
			"Visibility", " - Sets number of tour's segments that player can see on the left side of the screen.",
			"Turn Modifier", " - Multiplies your chance to succeed in a turn test. Doesn't work when player performs a drift",
			"Drift Modifier", " - Multiplies your chance to succeed in a turn test when performing a drift."},
		{ "", "Tires provide parameters strictly connected to type of the terrain. There are exactly six of them - as many as terrain types",
			"","Each parameter is represented by two numbers with the letter \"x\" between them - number of tests required and number of tests in general. There are two ways in which they impact the gameplay : ",
			"Friction", " - Alterates attributes responsible for speed modulation. Basically better probability of passing the tests - widder the modulation range Specific calculations are available in \"Formulas\" panel",
			"Turning tests", " -In any non-straight segment of tour player is being tested.Number of all tests and required tests are taken directly from tire attributes."},
		{ "Tour of Turns's"," game mechanics are inherently based on math. There are no dexterity involved, pure analysis. Every part of gameplay has its own formulas. Order in which this events take place look like this: ",
			"Attacking segment", " - This part starts with the beggining of the second turn. Players can choose targets that have score, higher up to 6 points and lower up to 4 points. (Player with the lowest score is on the lead)",
			"","When the target is chosen, he is being granted full attacking point. These point are used in later calculations. Attacking other players is double-edged - attacker also get half a point.",
			"Action segment"," - Player chooses how he would like to modify his speed. Then value of that modulation is being multiplied by 0.9 + TireEffectivness.",
			"","TireEffectivness parameter is calculated as common probability to pass serries of tests with local probability of 50%.",
			"","Number of tests required and number of tests in general come from tires parameter responsible for current segment terrain type.",
			"", "After speed is modified, durability burning is being calculated."},
		{ "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" },
		{ "Host game", "Search game", "Back" },
		{ "Main Color", "Secondary Color", "Music volume", "Hamachi connection", "Back" },
		{ "Player Informations", "Tour Informations", "tour_length", "turns", "current_champion", "winrate", "average_place"},
		{ "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green", "light cyan","light red","light magenta","yellow","white" },
		{ "Speed up","Slow down","Hand-Brake","Do nothing","Abaddon Race" },
		{ "Chance: ", "Estimated time: ", "Durability burning: ", "Estimated speed: " },
		{ "PLACE", "RACER", "SCORE" },
		{ "You can speed up by (0 to " , "You can slow down by (0 to " , "Hand brake value is - ", "You can just wait if you're moving..." ,"Remember only weak ones surrenders" },
		{ " You're racing as: ",  " Your current speed is: ", " Your vehice has ", " durability   " },
		{ "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ", "At a considerable distance: ", "Far ahead: ", "Barely noticeable: " },
		{ "Infobox  " , "Attack box" , "Chance of succeeding" },
		{ "Turn on asphalt - safe speed is around ", "Straight road, terrain is asphalt", "Turn on grass - safe speed is around ", "Straight grassy road" , "Turn on gravel - safe speed is around " , "Straight road, gravel",
			"Turn on sand - safe speed is around ", "Linear part of tour, but sandy",  "Turn on mud - safe speed is around ", "Muddy road, without turn" , "Turn on ice - safe speed is around ", "Slippery road with ice" },
		{ "Paul Harackovy", "Mark Driver", "Max McDonald", "Gordon Goodman", "Miguela Aguela", "Hans Ufner", "Isao Fujimoto", "Igor Belov", "John Hill", "Andrew Anderson", "Jane Turning", "Lester King", "Drew McNeil",
			"Sam Samson", "Gaston Reveneu", "Roman Torbon", "Helga Dick", "Mogore the Ogre", "David Black", "Reta Rdest", "Bloodwyn", "Quality Racer", "Sui Cide", "Ivan Atakovic", "Blu Apostrof" },
		{ " have manage to turn, ", " had mistaken, ", " badly crashed !!! ", " crashed !!!, " ," had an dangerous accident, " ," got off the route, " ," fell into a dangerous slip, " ," slipped, " },
		{ "","Hello I am Michal Balicki and I am really happy that you are playing my game.",
			"","Tour of Turns was my first real project in my programming way and I hope you like it.",
			"","I would like to thank people who helped me with this project.",
			"Krystian Grzegorzewicz ", "without him the game wouldn't be created.",
			"","He was patiently listening what problems do I have with ToT and the most important he gave me a main idea of the game.",
			"Marek Loek ", "for making an icon image and inspiring me to hard work.",
			"Marcin Marzec ", "for giving me inspiration.",
			"","I admire how much work he put into Pan Zniszczenia and Zniwiarz Dusz maps for HoMM3, based on Diablo universe.",
			"Miroslaw Zelent ", "for getting me interested in programming"},
		{ "on", "off" },
		{ "  _______  ___         ___     ", "     |    |   | |   | |   |    ", "     |    |   | |   | |___|    ", "     |    |   | |   | |  \\     ", "     |    |___| |___| |   \\    ", "                               ",
			"                               ", "                               ", "                               ", "                               ", "_______        ___    __   __  ", "   |    |   | |   |  |  | |    ",
			"   |    |   | |___|  |  | |__  ", "   |    |   | |  \\   |  |    | ", "   |    |___| |   \\  |  | ___| "},
		{ " ___   ___", "|   | |   ", "|   | |_  ", "|   | |   ", "|___| |   "},
		{ "You can't start new race with current game state, please repair files or reinstall game", "Files not found, please reinstall your game or repair missing files" , "There are no ranking files" , " cannot be read, please repair or delete corrupted files", "Please call a plumber" },
		{ "Unplayable Game",  "Missing Files", "Corupted Files", "Pipe error" },
		{ "Racer", " - Do you really want to do this ? (Y/N) ", "RIP, ", " dezintegrated his vehichle...","Don't attack" ," - You can't do this because you aren't moving...",
			"META" , " lost " , "required - ", " highest roll - ", " lowest roll - ", "in result of other racers behaviour", " speed,", "________________________________________________________  " }
	};
}