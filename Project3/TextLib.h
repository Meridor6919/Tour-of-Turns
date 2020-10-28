#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Window.h"
#include <mutex>

namespace Text
{
	enum TextAlign
	{
		left,
		center,
		right
	};
	struct MultithreadingData
	{
		std::mutex *mutex = nullptr;
		bool* loop = nullptr;
		std::chrono::milliseconds delay;
	};
	struct WindowInfo
	{
		HANDLE handle;
		int main_color;
		int secondary_color;
	};
	struct TextInfo
	{
		const std::vector<std::string> &text;
		size_t starting_index;
		COORD point_of_reference;
		TextAlign text_align;
		const short spacing;
		bool clear_after;
	};

	char Button(const bool* loop, std::chrono::milliseconds delay);
	namespace Choose
	{
		int Horizontal(const TextInfo &text_info, const WindowInfo &window_info, const MultithreadingData &multithreading_data = {});
		int Veritcal(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
		void VerticalShowGUI(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
		void VerticalClearGUI(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
		int Numeric(const int max, COORD starting_point, const bool zero_allowed, Window &main_window);
	}
	void OrdinaryText(std::vector<std::string> text, const TextAlign text_align, const short spacing, const COORD position, Window &main_window, std::mutex *mutex, const bool clearing = false);
	void OrdinaryText(std::vector<std::string> text, const TextAlign text_align, const short spacing, const COORD position, Window &main_window, const bool clearing = false);
	void TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short vertical_spacing, const COORD starting_point, Window &main_window, std::mutex *mutex, const bool clearing = false);
	void TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short vertical_spacing, const COORD starting_point, Window &main_window, const bool clearing = false);
	void Spaces(const int i, std::mutex *mutex);
	void Spaces(const int i);
}