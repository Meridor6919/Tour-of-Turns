#pragma once
#include <string>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	std::string GetMonoCharacterString(const int size, const char character);
	std::string Spaces(const int size);
	float GetTextAlignScalar(TextAlign text_align);
}