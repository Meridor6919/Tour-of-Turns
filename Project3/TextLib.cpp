#include "TextLib.h"

int Text::Choose::Horizontal(std::vector<std::string> text, int starting_position, COORD starting_point, Text::TextAlign text_align, bool clear_after, Window &main_window)
{
	HANDLE handle = main_window.GetHandle();
	int color1 = main_window.color1;
	int color2 = main_window.color2;
	char button;

	do
	{
		//Showing text with current index
		SetConsoleTextAttribute(handle, color1);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y });
		std::cout << "< ";
		SetConsoleTextAttribute(handle, color2);
		std::cout << text[starting_position];
		SetConsoleTextAttribute(handle, color1);
		std::cout << " >";

		button = _getch();

		//Clearing text from the screen
		for (int i = 0; i < (int)text[starting_position].size() + 4; i++)
			std::cout << "\b";
		for (int i = 0; i < (int)text[starting_position].size() + 4; i++)
			std::cout << " ";

		//Changing index when left or right arrow button is pressed
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 75 && starting_position > 0)
		{
			starting_position -= 1;
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 77 && starting_position < text.size() - 1)
		{
			starting_position += 1;
		}

	} while (button != 13);

	return starting_position;
}
int Text::Choose::Veritcal(std::vector<std::string> text, int starting_position, COORD starting_point, int spacing, Text::TextAlign text_align, bool clear_after, Window &main_window)
{
	HANDLE handle = main_window.GetHandle();
	int color1 = main_window.color1;
	int color2 = main_window.color2;
	char button;

	//Showing text on the screen
	SetConsoleTextAttribute(handle, color1);
	for (short i = 0; i < text.size(); i++)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[i].size()), starting_point.Y + i * (short)spacing });
		std::cout << text[i];
	}

	do
	{
		//Changing active text's color
		SetConsoleTextAttribute(handle, color2);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y + (short)starting_position * (short)spacing });
		std::cout << text[starting_position];

		button = _getch();

		//Changing color of active text back to normal
		//I assume that user will press the correct button and position will change
		SetConsoleTextAttribute(handle, color1);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y + (short)starting_position * (short)spacing });
		std::cout << text[starting_position];

		//Changing the index when up or down arrow button is pressed
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
		{
			starting_position += 1;
			if (starting_position == text.size())
				starting_position = 0;
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
		{
			starting_position -= 1;
			if (starting_position < 0)
				starting_position = text.size() - 1;
		}

	} while (button != 13);

	//Clearing the text if flag is active
	if (clear_after)
	{
		for (short i = 0; i < text.size(); i++)
		{
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[i].size()), starting_point.Y + i * (short)spacing });
			for (int j = 0; j < text[i].size(); j++)
				std::cout << " ";
		}
	}
	return starting_position;
}
int Text::Choose::Numeric(int max, COORD starting_point, bool zero_allowed, Window &main_window)
{
	char button;
	int number = 0;
	int pos = 0;
	SetConsoleCursorPosition(main_window.GetHandle(), starting_point);
	while (true)
	{
		button = _getch();
		//Adding if user pressed key between 0 and 9 
		if (button >= '0' && button <= '9')
		{
			if (button == '0' && pos == 0)
				continue;
			else if (number * 10 + button - 48 > max)
				continue;

			std::cout << button;
			number = number * 10 + button - 48;
			pos++;
		}
		//erasing last number if user pressed backspace
		if (button == '\b' && pos != 0)
		{
			std::cout << "\b \b";
			number /= 10;
			pos--;
		}
		if (button == 13)
		{
			if (pos == 0 && !zero_allowed)
				continue;
			break;
		}
	}
	//Clearing text from the screen
	SetConsoleCursorPosition(main_window.GetHandle(), starting_point);
	for (int i = 1; i <= number; i *= 10)
		std::cout << " ";
	return number;
}
void Text::OrdinaryText(std::vector<std::string> text, std::vector<Text::OrdinaryText_atributes> atribute, const Text::TextAlign text_align, const short spacing, const short position, Window &MainWindow, bool clearing)
{
	HANDLE handle = MainWindow.GetHandle();
	const int records = text.size();
	short current_position;
	short starting_pos = static_cast<short>(MainWindow.GetWidth() / 2 * text_align);


	SetConsoleCursorPosition(handle, { starting_pos - static_cast<short>((float)text_align / 2 * (float)text[0].size()), position });
	current_position = position;

	//Changing all chars in all members of the text array to ' ' if flag is active
	if (clearing)
	{
		for (int i = 0; i < text.size(); i++)
		{
			for (int j = 0; j < text[i].size(); j++)
			{
				text[i][j] = ' ';
			}
		}
	}

	//Showing the text
	for (int i = 0; i < records; i++)
	{
		if (atribute[i] == color2)
			SetConsoleTextAttribute(handle, MainWindow.color2);
		else
			SetConsoleTextAttribute(handle, MainWindow.color1);

		std::cout << text[i];

		if (atribute[i] == endl && i < records - 1)
		{
			float next_line_size = text[i + 1].size();

			for (int j = 1; atribute[i + j] != endl; j++)
				next_line_size += text[i + j + 1].size();

			current_position += spacing;
			SetConsoleCursorPosition(handle, { starting_pos - static_cast<short>((float)text_align / 2 * next_line_size), current_position });
		}
	}
}
void Text::TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, short vertical_spacing, const COORD starting_point, Window &main_window, bool clearing)
{
	HANDLE handle = main_window.GetHandle();
	std::vector<std::string>::iterator it = text.begin();

	for (short i = 0; i*texts_per_row < text.size(); i++)
	{
		//Setting right color
		if (i < painted_rows)
			SetConsoleTextAttribute(handle, main_window.color2);
		else
			SetConsoleTextAttribute(handle, main_window.color1);

		//Changing all chars in all members of the text array to ' ' if flag is active
		if (clearing)
		{
			for (int j = 0; j < text.size(); j++)
			{
				for (int k = 0; k < text[j].size(); k++)
				{
					text[j][k] = ' ';
				}
			}
		}

		//Showing the text
		for (short j = 0; j < texts_per_row && i*texts_per_row + j < text.size(); j++)
		{
			SetConsoleCursorPosition(handle, { starting_point.X + vertical_spacing * j + vertical_spacing / 2 - static_cast<short>((float)Text::TextAlign::center / 2 * (float)it->size()), starting_point.Y + i * (short)spacing });
			std::cout << *it;

			it++;
		}
	}
}
void Text::Spaces(int i)
{
	for (int j = 0; j < i; j++)
		std::cout << " ";
}