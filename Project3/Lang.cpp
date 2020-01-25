#pragma once
#include "Constants.h"

namespace LanguagePack
{
	std::vector<std::vector<std::string>> vector_of_strings;
	bool LoadVector(std::string path)
	{
		LanguagePack::vector_of_strings.push_back({});
		std::ifstream fvar;
		std::string line;
		int position = 0;
		fvar.open(path.c_str());
		while (std::getline(fvar, line))
		{
			if (line == "&&&")
			{
				LanguagePack::vector_of_strings.push_back({});
				++position;
			}
			else
			{
				LanguagePack::vector_of_strings[position].push_back(line);
			}
		}
		return true;
	}
}