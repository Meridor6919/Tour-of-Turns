#pragma once
#include <string>
#include <vector>
#include <conio.h>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		class TableTextInfo
		{
			const std::vector<std::string>& text_ref;
			std::vector<std::string> text_val;
			COORD point_of_reference;
			TextAlign text_align;
			int number_of_columns;
			int number_of_painted_rows;
			short vertical_spacing;
			short horizontal_spacing;
			bool clear_after;

		public:

			TableTextInfo(const std::vector<std::string>& text, COORD point_of_reference, TextAlign text_align, int number_of_columns,
				int number_of_painted_rows, short vertical_spacing, short horizontal_spacing, bool clear_after);
			TableTextInfo(std::vector<std::string>&& text, COORD point_of_reference, TextAlign text_align, int number_of_columns,
				int number_of_painted_rows, short vertical_spacing, short horizontal_spacing, bool clear_after);

			const std::vector<std::string>& GetText();
			COORD GetPointOfReference();
			TextAlign GetTextAlign();
			short GetVerticalSpacing();
			short GetHorizontalSpacing();
			int GetNumberOfPaintedRows();
			int GetNumberOfColumns();
			bool GetClearFlag();

			void SetClearFlag(bool clear_after);
		};
	}
}