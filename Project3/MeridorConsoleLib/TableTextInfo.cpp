#include "TableTextInfo.h"

MeridorConsoleLib::Text::TableTextInfo::TableTextInfo(const std::vector<std::string>& text, COORD point_of_reference, TextAlign text_align, int number_of_columns,
	int number_of_painted_rows, short vertical_spacing, short horizontal_spacing, bool clear_after) : text_ref{text}
{
	this->point_of_reference = point_of_reference;
	this->text_align = text_align;
	this->number_of_columns = number_of_columns;
	this->number_of_painted_rows = number_of_painted_rows;
	this->vertical_spacing = vertical_spacing;
	this->horizontal_spacing = horizontal_spacing;
	this->clear_after = clear_after;
	
}
MeridorConsoleLib::Text::TableTextInfo::TableTextInfo(std::vector<std::string>&& text, COORD point_of_reference, TextAlign text_align, int number_of_columns,
	int number_of_painted_rows, short vertical_spacing, short horizontal_spacing, bool clear_after) : text_ref{ text_val }
{
	this->point_of_reference = point_of_reference;
	this->text_align = text_align;
	this->number_of_columns = number_of_columns;
	this->number_of_painted_rows = number_of_painted_rows;
	this->vertical_spacing = vertical_spacing;
	this->horizontal_spacing = horizontal_spacing;
	this->clear_after = clear_after;
}

const std::vector<std::string>& MeridorConsoleLib::Text::TableTextInfo::GetText()
{
	return text_ref;
}

COORD MeridorConsoleLib::Text::TableTextInfo::GetPointOfReference()
{
	return point_of_reference;
}

MeridorConsoleLib::TextAlign MeridorConsoleLib::Text::TableTextInfo::GetTextAlign()
{
	return text_align;
}

short MeridorConsoleLib::Text::TableTextInfo::GetVerticalSpacing()
{
	return vertical_spacing;
}

short MeridorConsoleLib::Text::TableTextInfo::GetHorizontalSpacing()
{
	return horizontal_spacing;
}

int MeridorConsoleLib::Text::TableTextInfo::GetNumberOfPaintedRows()
{
	return number_of_painted_rows;
}

int MeridorConsoleLib::Text::TableTextInfo::GetNumberOfColumns()
{
	return number_of_columns;
}

bool MeridorConsoleLib::Text::TableTextInfo::GetClearFlag()
{
	return clear_after;
}

void MeridorConsoleLib::Text::TableTextInfo::SetClearFlag(bool clear_after)
{
	this->clear_after = clear_after;
}

