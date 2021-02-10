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
	constexpr int max_ais = 7;
	constexpr float meters_per_second = 1.0f / 3.6f;

	constexpr int minimum_drift_speed = 40;
	constexpr float drift_value = 2.0f;
	constexpr float friction_scalar = 0.9f;
	constexpr float tire_effectivness_multiplier = 0.2f;

	constexpr int attack_forward_distance = 4;
	constexpr int attack_backward_distance = 6;

	constexpr float maximum_speed_multiplier = 1.25f;
	constexpr float burning_levels = 5.0f;
	constexpr int base_burning_levels = 10;

	constexpr int badly_crashed = 50;
	constexpr int crashed = 40;
	constexpr int dangerous_accident = 30;
	constexpr int off_route = 20;
	constexpr int dangerous_slip = 10;

	//per attack point
	constexpr float speed_reduction = 0.125f;
	//per attack point
	constexpr float chance_reduction = 5.0f;
}