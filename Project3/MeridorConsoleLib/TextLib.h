#pragma once
#include <iostream>

#include "TextInfo.h"
#include "TableTextInfo.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
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
		void TableText(TableTextInfo& table_text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
	}
}