#pragma once
#include <string>

namespace Modifiers {
	const std::vector<std::string> car_modifiers = { "max_speed","max_accelerating","max_braking","hand_brake_value","durability","visibility","turn_mod","drift_mod" };
	const std::vector<std::string> tire_modifiers = { "asphalt","grass","gravel","sand","mud","ice" };
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
	const std::string missing_file = "files not found, please reinstall your game or repair missing files";
	const std::string ranking_not_present = "There are no ranking files";
	const std::string corrupted_file = " cannot be read, please repair or delete corrupted files";
}
namespace Error_Title
{
	const std::string missing_file = "Missing Files";
	const std::string corrupted_file = "Corupted Files";

}