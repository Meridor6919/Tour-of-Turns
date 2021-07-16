#pragma once
#include <string>
#include <vector>
#include <conio.h>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		class TextInfo
		{
			const std::vector<std::string>& text_ref;
			std::vector<std::string> text_val;
			bool is_ref;

			size_t starting_index;
			COORD point_of_reference;
			TextAlign text_align;
			short spacing;
			bool clear_after;

		public:

			TextInfo(const std::vector<std::string>& text, size_t starting_index, COORD point_of_reference, TextAlign text_align, short spacing, bool clear_after);
			TextInfo(std::vector<std::string>&& text, size_t starting_index, COORD point_of_reference, TextAlign text_align, short spacing, bool clear_after);

			const std::vector<std::string>& GetText();
			size_t GetStartingIndex();
			COORD GetPointOfReference();
			TextAlign GetTextAlign();
			short GetSpacing();
			bool GetClearFlag();

			void SetStartingIndex(size_t starting_index);
			void SetClearFlag(bool clear_after);
		};
	}
}

