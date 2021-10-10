#include "FileOperations.h"

std::string_view MeridorFileOperations::GetSeparatedValue(std::string_view text, int index, char separator) noexcept
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
void MeridorFileOperations::SetSeparatedValue(std::string& original_text, std::string_view text_to_place, int index, char separator) noexcept
{
	size_t start = 0;
	size_t count = 0;
	const size_t original_text_size = original_text.size();
	
	for (size_t i = 0; i < original_text_size; ++i)
	{
		if ((original_text[i] == separator) || (i + 1 == original_text_size))
		{
			if (!index)
			{
				count = i - start + (i + 1 == original_text_size);
				original_text.erase(start, count);
				original_text.insert(start, text_to_place);
				return;
			}
			else
			{
				--index;
				start = i + 1;
			}
		}
	}
}
std::string_view MeridorFileOperations::SetFloatingPointPrecision(std::string_view number, unsigned int precision) noexcept
{
	return number.substr(0, number.find_last_of('.') + precision + (precision > 0));
}
std::vector<std::string> MeridorFileOperations::ReadFile(const std::string path) noexcept
{
	std::vector<std::string> data{};
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
std::vector<std::string> MeridorFileOperations::GetFilesInDirectory(std::string_view path) noexcept
{
	std::vector<std::string> data{};
	if (std::filesystem::exists(path))
	{
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
		{
			if (!entry.is_directory())
			{
				std::string record = entry.path().string();
				record = record.substr(path.size() + 1, record.size() - path.size() - 1);
				data.push_back(std::move(record));
			}
		}
	}
	return data;
}
void MeridorFileOperations::RemoveExtension(std::string& string, std::string_view extension) noexcept
{
	string = string.substr(0, string.size() - extension.size());
}
void MeridorFileOperations::AddExtension(std::string& string, std::string_view extension) noexcept
{
	string += extension;
}
std::string_view MeridorFileOperations::GetExtension(std::string_view string) noexcept
{
	return string.substr(string.find_last_of('.'));
	
}