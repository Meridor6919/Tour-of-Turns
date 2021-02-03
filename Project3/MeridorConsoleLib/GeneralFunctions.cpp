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


std::string MeridorConsoleLib::GetSeparatedValue(const std::string& text, int index, char separator)
{
	int start = 0;
	int count = 0;
	int text_size = static_cast<int>(text.size());
	for (int i = 0; i < text_size; ++i)
	{
		if ((text[i] == separator) || (i + 1 == text_size))
		{
			if (!index)
			{
				count = i - start + (i + 1 == text_size);
				break;
			}
			else
			{
				--index;
				start = i + 1;
			}
		}
	}
	return text.substr(start, count);
}
std::string MeridorConsoleLib::SetSeparatedValue(const std::string& original_text, const std::string& text_to_place, int index, char separator)
{
	int start = 0;
	int count = 0;
	std::string ret;
	int original_text_size = static_cast<int>(original_text.size());
	for (int i = 0; i < original_text_size; ++i)
	{
		if ((original_text[i] == separator) || (i + 1 == original_text_size))
		{
			if (!index)
			{
				count = i - start + (i + 1 == original_text_size);
				return original_text.substr(0, start) + text_to_place + original_text.substr(start + count, original_text_size - start - count);
			}
			else
			{
				--index;
				start = i + 1;
			}
		}
	}
	return original_text;
}

std::string MeridorConsoleLib::SetPrecision(const std::string& floating_point_number, unsigned int precision)
{
	size_t number_size = floating_point_number.size();
	bool remove_decimal_point = !precision;
	for (int i = 0; i < number_size; ++i)
	{
		if (floating_point_number[i] == '.')
		{
			if (number_size - 1 - i < precision)
			{
				precision = number_size - 1 - i;
			}
			return floating_point_number.substr(0, i + precision + 1 - remove_decimal_point);
		}
	}
	return floating_point_number;
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
std::vector<std::string> MeridorConsoleLib::GetFilesInDirectory(const std::string path)
{
	std::vector<std::string> data = {};

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
	{
		if (!entry.is_directory())
		{
			std::string record = entry.path().string();
			record = record.substr(path.size() + 1, record.size() - path.size() - 1);
			data.emplace_back(record);
		}
	}
	return data;
}
void MeridorConsoleLib::RemoveExtension(std::string& string, const std::string &extension)
{
	const size_t extension_size = extension.size();
	string = string.substr(0, string.size() - extension_size);

}
void MeridorConsoleLib::RemoveExtension(std::vector<std::string>& vector, const std::string &extension)
{
	for (size_t i = 0; i < vector.size(); ++i)
	{
		RemoveExtension(vector[i], extension);
	}
}