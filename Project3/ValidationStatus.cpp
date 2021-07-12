#include "ValidationStatus.h"

void Validation::Status::SetFlag(Flags flag)
{
	if (!(status & static_cast<char>(flag)))
	{
		status = status | static_cast<char>(flag);
	}
}
void Validation::Status::UnsetFlag(Flags flag)
{
	if (status & static_cast<char>(flag))
	{
		status = status - static_cast<char>(flag);
	}
}
bool Validation::Status::IsFlagActive(Flags flag)
{
	return status & static_cast<char>(flag);
}
