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
	enum class TextAlign
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
		short spacing;
		bool clear_after;
	};
	struct TableTextInfo
	{
		const std::vector<std::string>& text;
		COORD point_of_reference;
		TextAlign text_align;
		int number_of_columns;
		int painted_rows;
		short vertical_spacing;
		short horizontal_spacing;
		bool clear;
	};

	char Button(const bool* loop, std::chrono::milliseconds delay);
	float GetTextAlignScalar(TextAlign text_align);
	namespace Choose
	{
		int Horizontal(const TextInfo &text_info, const WindowInfo &window_info, const MultithreadingData &multithreading_data = {});
		int Veritcal(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
		void VerticalShowGUI(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
		void VerticalClearGUI(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
		int Numeric(const int max, COORD starting_point, const bool zero_allowed, Window &main_window);
	}
	void OrdinaryText(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
	void ClearOrdinaryText(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
	void TableText(const TableTextInfo& table_text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
	std::string Spaces(const int i);
}