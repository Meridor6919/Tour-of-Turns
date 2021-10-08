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

			[[nodiscard]] COORD GetPositionAbsolute(size_t lenght, short y_value) const noexcept;
			[[nodiscard]] short GetYPositionFromIndex(short index) const noexcept;
			[[nodiscard]] int GetMaxRecords() const noexcept;

			void CorrectContainerSize() noexcept;
			void CorrectTextSize(std::string& text, short& remaining_space) const noexcept;
			void ShowDataRecord(Color main_color, Color secondary_color, short index) const noexcept;

		public:
			Infobox(InfoboxDesc& infobox_desc, const MeridorConsoleLib::Text::WindowInfo& window_info) noexcept;
			
			void Push(std::string secondary_color_text, std::string main_color_text) noexcept;
			void Reset() noexcept;
			
			void Draw() const noexcept;
			void DrawBox() const noexcept;
			void ClearBox() const noexcept;
			void Draw(const MultithreadingData& multithreading_data) const noexcept;
			void DrawBox(const MultithreadingData& multithreading_data) const noexcept;
			void ClearBox(const MultithreadingData& multithreading_data) const noexcept;
		};
	}
}
