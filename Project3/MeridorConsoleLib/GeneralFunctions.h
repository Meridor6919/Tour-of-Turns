#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	std::string GetMonoCharacterString(const int size, const char character);
	std::string Spaces(const int size);
	float GetTextAlignScalar(TextAlign text_align);
	std::vector<std::string> ReadFile(const std::string path);
	std::vector<std::string> GetFilesInDirectory(const std::string path);
}