#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "GeneralFunctions.h"
#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		class Infobox
		{
			InfoboxDesc* infobox_desc = nullptr;
			const WindowInfo* window_info = nullptr;

			COORD GetPositionAbsolute(size_t lenght, short y_value);
			short GetYPositionFromIndex(short index);
			int GetMaxRecords();

			void CorrectContainerSize();
			void CorrectTextSize(std::string& text, short& remaining_space);
			void ShowDataRecord(Color main_color, Color secondary_color, short index);

		public:
			Infobox(InfoboxDesc& infobox_desc, const MeridorConsoleLib::Text::WindowInfo& window_info);
			
			void Push(std::string secondary_color_text, std::string main_color_text);
			void Reset();
			
			void Draw();
			void DrawBox();
			void ClearBox();
			void Draw(const MultithreadingData& multithreading_data);
			void DrawBox(const MultithreadingData& multithreading_data);
			void ClearBox(const MultithreadingData& multithreading_data);
		};
	}
}
