#include "Infobox.h"

InfoBox::InfoBox(const int size, const Text::TextAlign text_align, const COORD starting_point, const int spacing, Window &main_window)
{
	this->size = size;
	this->text_align = text_align;
	this->starting_point = starting_point;
	this->spacing = spacing;
	this->main_window = &main_window;
}
void InfoBox::Push(const std::string special_text, const std::string text)
{
	//Pushing new text into infobox
	info.push_back(special_text + "  " + text);	

	HANDLE handle = main_window->GetHandle();
	std::vector<std::string>::iterator it = info.begin();

	//Clearing visible text from infobox to update it
	Clear();	

	//Deleting outdated elements
	if (size < info.size())	
	{
		info.erase(it);
		it = info.begin();
	}
	//Showing recent info
	SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * static_cast<float>(it->size())), starting_point.Y + static_cast<short>((info.size() - 1) * spacing) });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << special_text << "  ";
	SetConsoleTextAttribute(handle, main_window->color2);
	std::cout << text << "  ";

	//Showing rest of elements
	SetConsoleTextAttribute(handle, 8);
	for (int i = 0; it != std::prev(info.end(), 1); ++it, ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * static_cast<float>(it->size())), starting_point.Y + static_cast<short>(i  * spacing)});
		std::cout << *it;
	}
}
void InfoBox::Clear()
{
	HANDLE handle = main_window->GetHandle();
	std::vector<std::string>::iterator it = info.begin();

	for (short i = 0; it != info.end(); ++it, ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * static_cast<float>(it->size())), starting_point.Y + static_cast<short>(i  * spacing) });
		for (int j = 0; j < it->size(); ++j)
		{
			std::cout << " ";
		}
	}
}

