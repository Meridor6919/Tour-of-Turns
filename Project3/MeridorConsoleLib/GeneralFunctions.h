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
	void SetColor(HANDLE handle, Color color);

	std::string GetSeparatedValue(const std::string& text, int index, char separator = '\t');
	std::string SetSeparatedValue(const std::string& original_text, const std::string& text_to_put, int index, char separator = '\t');
	std::string SetPrecision(const std::string& floating_point_number, unsigned int precision);

	std::vector<std::string> ReadFile(const std::string path);
	std::vector<std::string> GetFilesInDirectory(const std::string path);
	void RemoveExtension(std::string& string, const std::string& extension);
	void RemoveExtension(std::vector<std::string>& vector, const std::string& extension);
	void AddExtension(std::string& string, const std::string& extension);
	void AddExtension(std::vector<std::string>& vector, const std::string& extension);
	std::string GetExtension(const std::string& string);

	std::istream& StringGetline(std::istream& input_stream, std::string& var);
	template<class T>
	std::istream& IntegerGetline(std::istream& input_stream, T& var)
	{
		std::string temp;
		std::istream& ret = std::getline(input_stream, temp);
		var = static_cast<T>(atoi(temp.c_str()));
		return ret;
	}
	template<class T>
	std::istream& FloatGetline(std::istream& input_stream, T& var)
	{
		std::string temp;
		std::istream& ret = std::getline(input_stream, temp);
		var = static_cast<T>(atof(temp.c_str()));
		return ret;
	}
	template<class T>
	bool Between(T lower_bound, T higher_bound, T value)
	{
		return (value >= lower_bound && value <= higher_bound);
	}
	template<class T>
	T Power(T number, int power)
	{
		T ret = 1;
		while (power > 0)
		{
			ret *= number;
			--power;
		}
		return ret;
	}
}