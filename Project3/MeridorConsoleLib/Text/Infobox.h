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
			const TextInfo* window_info = nullptr;
			const MultithreadingData* multithreading_data = nullptr;

			COORD GetPositionAbsolute(size_t lenght, short y_value);
			short GetYPositionFromIndex(short index);
			int GetPossibleSlots();

		public:

			Infobox(InfoboxDesc& infobox_desc, const MeridorConsoleLib::Text::TextInfo& window_info, const MultithreadingData& multithreading_data);
			Infobox(InfoboxDesc& infobox_desc, const MeridorConsoleLib::Text::TextInfo& window_info);
			void Push(std::string secondary_color_text, std::string main_color_text);
			void Draw();
			void DrawBox();
			void ClearBox();
			void Reset();
		};
	}
}
