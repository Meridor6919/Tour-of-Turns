#include "FileOperations.h"

std::string MeridorFileOperations::GetSeparatedValue(const std::string& text, int index, char separator)
{
	size_t start = 0;
	size_t count = 0;
	size_t text_size = text.size();
	for (size_t i = 0; i < text_size; ++i)
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
std::string MeridorFileOperations::SetSeparatedValue(const std::string& original_text, const std::string& text_to_place, int index, char separator)
{
	size_t start = 0;
	size_t count = 0;
	size_t original_text_size = original_text.size();
	std::string ret;
	for (size_t i = 0; i < original_text_size; ++i)
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
std::string MeridorFileOperations::SetFloatingPointPrecision(const std::string& number, unsigned int precision)
{
	return std::string(number.begin(), std::find(number.begin(), number.end(), '.') + precision);
}
std::vector<std::string> MeridorFileOperations::ReadFile(const std::string path)
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
std::vector<std::string> MeridorFileOperations::GetFilesInDirectory(const std::string path)
{
	std::vector<std::string> data = {};
	if (std::filesystem::exists(path))
	{
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
		{
			if (!entry.is_directory())
			{
				std::string record = entry.path().string();
				record = record.substr(path.size() + 1, record.size() - path.size() - 1);
				data.emplace_back(record);
			}
		}
	}
	return data;
}
void MeridorFileOperations::RemoveExtension(std::string& string, const std::string& extension)
{
	string = string.substr(0, string.size() - extension.size());
}
void MeridorFileOperations::AddExtension(std::string& string, const std::string& extension)
{
	string += extension;
}
std::string MeridorFileOperations::GetExtension(const std::string& string)
{
	for (int i = static_cast<int>(string.size()) - 1; i >= 0; --i)
	{
		if (string[i] == '.')
		{
			return string.substr(i);
		}
	}
	return "";
}