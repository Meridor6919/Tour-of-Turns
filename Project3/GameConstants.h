#pragma once
#include <string>
#include <vector>
#include <fstream>

namespace RankingInfo
{
	enum 
	{
		Name,
		GamesPlayed,
		GamesWon,
		Place,
		Score,
		BestScore,
		FavCar,
		FavTire,
		Crashes,
		Attacks,
		Drifts,
		Burning,
		last
	};
}
namespace RankingClassification
{
	enum
	{
		all_games = 1,
		ai8 = 2,
		multiplayer = 4
	};
}
namespace ConnectionCodes
{
	enum
	{
		Start = 10,
		GetInit,
		NewTurn,
		GetCarNames,
		GetTireNames,
		GetCarParams,
		GetTireParams,
		GetAllAttributes,
		GetTour,
		SetName,
		SetCar,
		SetTires,
		SetAction,
		SetAttack,
		last
	};
}
namespace CarAttributes
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
namespace TerrainTypes
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
namespace GameConstants
{
	constexpr int minimum_drift_speed = 40;
	constexpr int attack_forward_distance = 4;
	constexpr int attack_backward_distance = 6;
	constexpr float drift_value = 2.0f;
	constexpr float friction_scalar = 0.9f;
	constexpr int box_width = 84;
	constexpr int max_ais = 7;
}