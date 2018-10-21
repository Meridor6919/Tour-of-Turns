#include "Participant.h"

Participant::Participant(std::vector<int>car_modifiers, std::vector<std::string> tire_modifiers)
{
	this->car_modifiers = car_modifiers;
	this->tire_modifiers = tire_modifiers;
	this->current_speed = 0.0f;
	this->score = 0.0f;
	this->current_durability = car_modifiers[CarModifiers::durability];
}