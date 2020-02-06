#include "Infobox.h"

InfoBox::InfoBox(const int size, const COORD starting_point, const short spacing, Window &main_window)
{
	this->size_of_visible_infobox = size;
	this->starting_point = starting_point;
	this->spacing = spacing;
	this->main_window = &main_window;
}
void InfoBox::Push(const std::string special_text, const std::string text)
{
	const HANDLE handle = main_window->GetHandle();
	
	Clear();	
	infobox.push_back(special_text + "  " + text);	
	//Deleting outdated elements
	if (size_of_visible_infobox < static_cast<int>(infobox.size()))	
	{
		infobox.erase(infobox.begin());
	}
	//Showing recent info
	const short vector_size = static_cast<short>(infobox.size());
	SetConsoleCursorPosition(handle, { starting_point.X, starting_point.Y + (vector_size - 1) * spacing });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << special_text << "  ";
	SetConsoleTextAttribute(handle, main_window->color2);
	std::cout << text << "  ";
	//Showing rest of elements
	SetConsoleTextAttribute(handle, 8);
	for (short i = 0; i < vector_size - 1; ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X, starting_point.Y + i  * spacing });
		std::cout << infobox[vector_size - i - 1];
	}
}
void InfoBox::Clear()
{
	const HANDLE handle = main_window->GetHandle();
	const short vector_size = static_cast<short>(infobox.size());
	for (short i = 0; i < vector_size; ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X, starting_point.Y + i  * spacing });
		Text::Spaces(static_cast<short>(infobox[0].size()));
	}
}

