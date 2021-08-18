#pragma once
#include <string>
#include <vector>
#include <list>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		template<class T> using RefVector = std::vector<T>&;
		template<class T> using Vector = std::vector<T>;
		template<class T, size_t size> using Array = std::array<T, size>;
		template<class T, size_t size> using RefArray = std::array<T, size>&;

		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		struct TextInfo
		{
			TextInfo(TextContainer<std::string, Args...> text) : text(text) {}
			TextInfo(std::initializer_list<std::string> init_list) : text(init_list) {}

			TextContainer<std::string, Args...> text;
			size_t starting_index = 0;
			COORD point_of_reference = { 0,0 };
			TextAlign text_align = TextAlign::left;
			short spacing = 1;
			bool clear_after = false;
		};

		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		struct TableTextInfo
		{
			TableTextInfo(TextContainer<std::string, Args...>& text) : text(text) {}

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

