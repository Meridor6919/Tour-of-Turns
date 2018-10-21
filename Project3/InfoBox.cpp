#include "Infobox.h"

InfoBox::InfoBox(int size, Text::TextAlign text_align, COORD starting_point, int spacing, Window &main_window)
{
	this->size = size;
	this->text_align = text_align;
	this->starting_point = starting_point;
	this->spacing = spacing;
	this->main_window = &main_window;
}
void InfoBox::Push(std::string special_text, std::string text)
{
	HANDLE handle = main_window->GetHandle();
	std::vector<std::string>::iterator it = info.begin();

	info.push_back(special_text + " " + text);		//adding text to infobox

	Clear();									//clearing visible text from infobox to update it

	if (size < info.size())						//deleting elements out of setted size
	{
		info.erase(it);
		it = info.begin();
	}
	SetConsoleTextAttribute(handle, 8);			//painting elder texts in gray 

	for (short i = 0; it != std::prev(info.end(), 1); ++it, i++)	//showing it
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)it->size()), starting_point.Y + i * (short)spacing });
		std::cout << *it;
	}

	//showing new text with special colouring
	SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)it->size()), starting_point.Y + (short)(info.size() - 1) * (short)spacing });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << special_text << " ";
	SetConsoleTextAttribute(handle, main_window->color2);
	std::cout << text << " ";
}
void InfoBox::Clear()
{
	HANDLE handle = main_window->GetHandle();
	std::vector<std::string>::iterator it = info.begin();

	for (short i = 0; it != info.end(); ++it, i++)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)it->size()), starting_point.Y + i * (short)spacing });
		for (int j = 0; j < it->size(); j++)
			std::cout << " ";
	}
}