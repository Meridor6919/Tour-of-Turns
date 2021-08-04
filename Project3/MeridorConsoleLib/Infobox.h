#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "GeneralFunctions.h"
#include "DataStructures.h"

namespace MeridorConsoleLib
{
	struct InfoBoxDesc
	{
		size_t history_size;
		RECT box_size;
		short vertical_spacing;
		TextAlign text_align;
	};

	class InfoBox
	{
		const short upper_border_height = 2;
		unsigned short box_width;
		unsigned short box_height;

		InfoBoxDesc info_box_desc;
		WindowInfo window_info;
		const MultithreadingData* multithreading_data;
		std::vector<std::pair<std::string, std::string>> data;

		void SetCursorPosition(short index);
		std::string RegulateStringSize(const std::string& text, short max_size, bool fill_to_max_size = false);
		void ShowLineOfText(unsigned short index, Color primary_color, Color secondary_color);

	public:

		InfoBox(const InfoBoxDesc& info_box_desc, const MeridorConsoleLib::WindowInfo& window_info, const MultithreadingData* multithreading_data = nullptr);
		void Push(const std::string secondary_color_text, const std::string main_color_text);
		void Draw();
		void DrawBox(bool clear_instead = false);
		void Clear();
		void Reset();
	};
}