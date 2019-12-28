#pragma once
#include <string>
#include <vector>

namespace VectorOfStrings
{
	extern std::vector<std::string> car_modifiers;
	extern std::vector<std::string> tire_modifiers;
	extern std::vector<std::string> main_menu_options;
	extern std::vector<std::string> game_lobby_options;
	extern std::vector<std::string> game_mode_options;
	extern std::vector<std::string> game_options_options;
	extern std::vector<std::string> game_options_colors;
	extern std::vector<std::string> race_actions;
	extern std::vector<std::string> race_chances;
	extern std::vector<std::string> race_ranking;
	extern std::vector<std::string> race_interface;
	extern std::vector<std::string> race_attribs;
	extern std::vector<std::string> race_distance;
	extern std::vector<std::string> race_boxes;
	extern std::vector<std::string> race_infobox;
	extern std::vector<std::string> participant_names;
	extern std::vector<std::string> participant_infobox;
	extern std::vector<std::string> on_off;
	extern std::vector<std::string> credits;
	extern std::vector<std::string> title_main;
	extern std::vector<std::string> title_additional;
	extern std::vector<std::string> error_msg;
	extern std::vector<std::string> error_title;
	extern std::vector<std::string> other_string;

}
namespace OtherStrings
{
	enum
	{
		default_name,
		action_confirm,
		infobox_RIP1,
		infobox_RIP2,
		attack,
		unable_to_move,
		meta,
		lost,
		required,
		high_roll,
		low_roll,
		behaviour,
		speed,
		border,
		last
	};
}
namespace ErrorTitles
{
	enum
	{
		unplayable,
		missing_file,
		corrupted_file,
		last
	};
}
namespace ErrorMsgs
{
	enum
	{
		unplayable,
		missing_file,
		ranking_not_present,
		corrupted_file,
		last
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
namespace GameValues
{
	const float drift_value = 2.0f;
	const float friction_scalar = 0.9f;
}
namespace MathFunctions
{
	inline long Factorial(int i) 
	{
		if (i > 1)
		{
			return Factorial(i - 1)*i;
		}
		else
		{
			return 1;
		}
	};
	template<class T>
	inline T PowerInt(T number, int power)
	{
		T value = 1;
		for (int i = 0; i < power; i++)
		{
			value *= number;
		}
		return value;
	}
}