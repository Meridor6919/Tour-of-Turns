#include "GeneralFunctions.h"

std::string MeridorConsoleLib::GetMonoCharacterString(const int size, const char character)
{
	std::string ret = "";
	ret.resize(size, character);
	return ret;
}
std::string MeridorConsoleLib::Spaces(const int size)
{
	return GetMonoCharacterString(size, ' ');
}

float MeridorConsoleLib::GetTextAlignScalar(TextAlign text_align)
{
	return static_cast<float>(text_align) / 2.0f;;
}

std::vector<std::string> MeridorConsoleLib::ReadFile(const std::string path)
{
	std::vector<std::string> data;
	std::ifstream fvar;
	std::string helper;
	fvar.open(path);
	while (getline(fvar, helper) && helper != "")
	{
		data.push_back(std::move(helper));
	}
	fvar.close();
	return data;
}
