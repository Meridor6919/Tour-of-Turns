#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace MeridorFileOperations
{
	std::string GetSeparatedValue(const std::string& text, int index, char separator = '\t');
	std::string SetSeparatedValue(const std::string& original_text, const std::string& text_to_put, int index, char separator = '\t');
	std::string SetPrecision(const std::string& floating_point_number, unsigned int precision);

	std::vector<std::string> ReadFile(std::string path);
	std::vector<std::string> GetFilesInDirectory(std::string path);
	void RemoveExtension(std::string& string, const std::string& extension);
	void AddExtension(std::string& string, const std::string& extension);
	std::string GetExtension(const std::string& string);

	template<class T>
	std::istream& GetlineToInteger(std::istream& input_stream, T& var)
	{
		std::string temp;
		std::istream& ret = std::getline(input_stream, temp);
		var = static_cast<T>(atoi(temp.c_str()));
		return ret;
	}
	template<class T>
	std::istream& GetlineToFloatingPoint(std::istream& input_stream, T& var)
	{
		std::string temp;
		std::istream& ret = std::getline(input_stream, temp);
		var = static_cast<T>(atof(temp.c_str()));
		return ret;
	}
}