#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace MeridorFileOperations
{
	[[nodiscard]] std::string_view GetSeparatedValue(std::string_view text, int index, char separator = '\t') noexcept;
	void SetSeparatedValue(std::string& original_text, std::string_view text_to_put, int index, char separator = '\t') noexcept;
	[[nodiscard]] std::string_view SetFloatingPointPrecision(std::string_view number, unsigned int precision) noexcept;

	[[nodiscard]] std::vector<std::string> ReadFile(std::string path) noexcept;
	[[nodiscard]] std::vector<std::string> GetFilesInDirectory(std::string_view path) noexcept;
	[[nodiscard]] std::string_view GetExtension(std::string_view string) noexcept;
	void RemoveExtension(std::string& string, std::string_view extension) noexcept;
	void AddExtension(std::string& string, std::string_view extension) noexcept;

	template<class T>
	[[nodiscard]] std::istream& GetlineToInteger(std::istream& input_stream, T& var) noexcept
	{
		std::string temp;
		std::istream& ret = std::getline(input_stream, temp);
		var = static_cast<T>(atoi(temp.c_str()));
		return ret;
	}
	template<class T>
	[[nodiscard]] std::istream& GetlineToFloatingPoint(std::istream& input_stream, T& var) noexcept
	{
		std::string temp;
		std::istream& ret = std::getline(input_stream, temp);
		var = static_cast<T>(atof(temp.c_str()));
		return ret;
	}
}