#pragma once
#include <string>
#include <vector>
#include <fstream>

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
	const int maximum_timer = 60;
	const int maximum_name_length = 20;
	const int minimum_drift_speed = 40;
	const int attack_forward_distance = 4;
	const int attack_backward_distance = 6;
	const float drift_value = 2.0f;
	const float friction_scalar = 0.9f;
}