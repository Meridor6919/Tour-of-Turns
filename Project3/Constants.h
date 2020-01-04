#pragma once
#include <string>
#include <vector>

namespace LanguagePack
{
	extern std::vector<std::vector<std::string>> vector_of_strings;
	enum
	{
		car_modifiers,
		tire_modifiers,
		main_menu,
		game_information,
		introduction,
		game_lobby,
		multiplayer,
		game_options,
		information_box_titles,
		colors,
		race_actions,
		race_chances,
		race_ranking,
		race_interface,
		race_attribs,
		race_distance,
		race_boxes,
		race_infobox,
		participant_names,
		participant_infobox,
		credits,
		on_off,
		title_main,
		title_additional,
		error_msg,
		error_title,
		other_string,
		last
	};
}
namespace GameInformation
{
	enum
	{
		player_info,
		tour_info,
		tour_length,
		turns,
		champion,
		win_rate,
		avg_place,
		last

	};
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
	const std::string tour = ".tour";
	const std::string car = ".car";
	const std::string tire = ".tire";
	const std::string ranking = ".rank";
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