#include "TextInfo.h"

MeridorConsoleLib::Text::TextInfo::TextInfo(const std::vector<std::string>& text, size_t starting_index, COORD point_of_reference, TextAlign text_align, short spacing, bool clear_after) : text_ref{ text }
{
	this->is_ref = true;
	this->starting_index = starting_index;
	this->point_of_reference = point_of_reference;
	this->text_align = text_align;
	this->spacing = spacing;
	this->clear_after = clear_after;
}
MeridorConsoleLib::Text::TextInfo::TextInfo(std::vector<std::string>&& text, size_t starting_index, COORD point_of_reference, TextAlign text_align, short spacing, bool clear_after) : text_ref{ text_val }
{
	this->is_ref = false;
	this->text_val = text;
	this->starting_index = starting_index;
	this->point_of_reference = point_of_reference;
	this->text_align = text_align;
	this->spacing = spacing;
	this->clear_after = clear_after;
}
const std::vector<std::string>& MeridorConsoleLib::Text::TextInfo::GetText()
{
	if (this->is_ref)
	{
		return this->text_ref;
	}
	else
	{
		return this->text_val;
	}
}
size_t MeridorConsoleLib::Text::TextInfo::GetStartingIndex()
{
	return starting_index;
}
COORD MeridorConsoleLib::Text::TextInfo::GetPointOfReference()
{
	return point_of_reference;
}
MeridorConsoleLib::TextAlign MeridorConsoleLib::Text::TextInfo::GetTextAlign()
{
	return text_align;
}
short MeridorConsoleLib::Text::TextInfo::GetSpacing()
{
	return spacing;
}
bool MeridorConsoleLib::Text::TextInfo::GetClearFlag()
{
	return clear_after;
}
void MeridorConsoleLib::Text::TextInfo::SetStartingIndex(size_t starting_index)
{
	this->starting_index = starting_index;
}
void MeridorConsoleLib::Text::TextInfo::SetClearFlag(bool clear_after)
{
	this->clear_after = clear_after;
}
