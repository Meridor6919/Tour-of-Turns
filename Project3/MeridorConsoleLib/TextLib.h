#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <conio.h>

#include "TextInfo.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
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
		namespace Choose
		{
			int Horizontal(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
			int Veritcal(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
			void VerticalShowGUI(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
			void VerticalClearGUI(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
			void Numeric(int* number_return_value, const int max, COORD starting_point, const WindowInfo& window_info, const MultithreadingData& multithreading_data);
		}
		void OrdinaryText(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
		void TableText(const TableTextInfo& table_text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
	}
}