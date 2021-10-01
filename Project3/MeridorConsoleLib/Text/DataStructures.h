#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <thread>

#include "Constants.h"
#include "Color.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		template<class T> using RefVector = std::vector<T>&;
		template<class T> using Vector = std::vector<T>;
		template<class T, size_t size> using Array = std::array<T, size>;
		template<class T, size_t size> using RefArray = std::array<T, size>&;

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
			std::chrono::milliseconds delay = default_delay;
		};
		template<class T>
		struct GenericChooseDesc
		{
			T value{};
			T error_value{};
			COORD point_of_reference{};
			Align text_align = Align::left;
			bool clear_after = true;
			bool exit_on_processed_input = false;
			bool dont_show_interface = false;
		};
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		struct HorizontalChooseDesc : GenericChooseDesc<long long>
		{
			explicit HorizontalChooseDesc(TextContainer<std::string, Args...> text) : text(text) {};
			HorizontalChooseDesc(std::initializer_list<std::string> init_list) : text(init_list) {};

			TextContainer<std::string, Args...> text;
			std::string decoration_left = "< ";
			std::string decoration_right = " >";
		};
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		struct VerticalChooseDesc : GenericChooseDesc<long long>
		{
			explicit VerticalChooseDesc(TextContainer<std::string, Args...> text) : text(text) {};
			VerticalChooseDesc(std::initializer_list<std::string> init_list) : text(init_list) {};

			TextContainer<std::string, Args...> text;
			short spacing = default_spacing;
		};
		struct NumericChooseDesc : GenericChooseDesc<long long>
		{
			long long lower_bound = -100;
			long long upper_bound = 100;
		};
		struct TextChooseDesc : GenericChooseDesc<std::string>
		{
			std::string dictionary{ "abcdefghijklmnopqrstuvwyzABCDEFGHIJKLMNOPQRSTUVWXYZ " };
			int maximum_size = 100;
			bool special_space_rules = true;
		};
		template<class T1, class T2>
		struct MyPair
		{
			T1 first;
			T2 second;
		};
		struct InfoboxDesc
		{
			std::vector<MyPair<std::string, std::string>> data{};
			COORD position{};
			COORD box_size{};
			short vertical_spacing = default_spacing;
			Align text_align = Align::left;
			Color fading_color = Color::dark_gray;
			char border_building_character = '-';
		};
		

		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		struct TableTextDesc
		{
			explicit TableTextDesc(TextContainer<std::string, Args...>& text) : text(text) {};
			TableTextDesc(std::initializer_list<std::string> init_list) : text(init_list) {};

			TextContainer<std::string, Args...> text;
			COORD point_of_reference{};
			Align table_alignment = Align::left;
			Align column_alignment = Align::left;
			int number_of_columns = 1;
			int number_of_painted_rows = 1;
			short vertical_spacing = default_spacing;
			short horizontal_spacing = default_spacing;
			bool clear_after = false;
		};
	}
}

