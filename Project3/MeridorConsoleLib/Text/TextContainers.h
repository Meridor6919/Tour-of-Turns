#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <thread>

#include "Color.h"

namespace MeridorConsoleLib
{
	enum class TextAlign : unsigned int
	{
		left,
		center,
		right,
		last
	};
	struct TextInfo
	{
		HANDLE output_handle = INVALID_HANDLE_VALUE;
		Color main_color = Color::white;
		Color secondary_color = Color::light_green;
	};
	struct MultithreadingData
	{
		std::mutex* mutex = nullptr;
		bool* skip_blocking_functions = nullptr;
		std::chrono::milliseconds delay = std::chrono::milliseconds(50);
	};

	namespace Text
	{
		template<class T> using RefVector = std::vector<T>&;
		template<class T> using Vector = std::vector<T>;
		template<class T, size_t size> using Array = std::array<T, size>;
		template<class T, size_t size> using RefArray = std::array<T, size>&;

		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		struct ChooseDesc
		{
			ChooseDesc(TextContainer<std::string, Args...> text) : text(text) {};
			ChooseDesc(std::initializer_list<std::string> init_list) : text(init_list) {};

			TextContainer<std::string, Args...> text;
			size_t starting_index = 0;
			COORD point_of_reference = { 0,0 };
			TextAlign text_align = TextAlign::left;
			short spacing = 1;
			bool clear_after = false;
		};

		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		struct TableTextDesc
		{
			TableTextDesc(TextContainer<std::string, Args...>& text) : text(text) {};
			TableTextDesc(std::initializer_list<std::string> init_list) : text(init_list) {};

			TextContainer<std::string, Args...> text;
			COORD point_of_reference = { 0,0 };
			TextAlign table_alignment = TextAlign::left;
			TextAlign column_alignment = TextAlign::left;
			int number_of_columns = 1;
			int number_of_painted_rows = 0;
			short vertical_spacing = 0;
			short horizontal_spacing = 1;
			bool clear_after = false;
		};
	}
}

