#include "Participant.h"
#include "NetworkRole.h"

Participant::Participant(std::string car_path, std::string tire_path, SinglePlayer &network_role)
{
	this->car_path = car_path;
	this->network_role = &network_role;
	this->car_modifiers = network_role.GetCarParameters(car_path);
	this->tire_modifiers = network_role.GetTireParameters(tire_path);
	this->current_speed = 0.0f;
	this->score = 0.0f;
	this->current_durability = car_modifiers[CarModifiers::durability];
}