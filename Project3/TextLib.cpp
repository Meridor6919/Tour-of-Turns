#include "TextLib.h"

int Text::Choose::Horizontal(std::vector<std::string> text, int starting_position, COORD starting_point, Text::TextAlign text_align, bool clear_after, Window &main_window)
{
	HANDLE handle = main_window.GetHandle();
	int color1 = main_window.color1;
	int color2 = main_window.color2;
	char button;

	SetConsoleTextAttribute(handle, color1);


	SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y });
	std::cout << "< ";
	SetConsoleTextAttribute(handle, color2);
	std::cout << text[starting_position];
	SetConsoleTextAttribute(handle, color1);
	std::cout << " >";

	do
	{
		button = _getch();

		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 75 && starting_position > 0)
		{
			for (int i = 0; i < (int)text[starting_position].size() + 4; i++)
				std::cout << "\b";
			for (int i = 0; i < (int)text[starting_position].size() + 4; i++)
				std::cout << " ";

			starting_position -= 1;
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y });

			std::cout << "< ";
			SetConsoleTextAttribute(handle, color2);
			std::cout << text[starting_position];
			SetConsoleTextAttribute(handle, color1);
			std::cout << " >";
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 77 && starting_position < text.size() - 1)
		{
			for (int i = 0; i < (int)text[starting_position].size(); i++)
				std::cout << "\b";
			for (int i = 0; i < (int)text[starting_position].size(); i++)
				std::cout << " ";

			starting_position += 1;
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y });

			std::cout << "< ";
			SetConsoleTextAttribute(handle, color2);
			std::cout << text[starting_position];
			SetConsoleTextAttribute(handle, color1);
			std::cout << " >";
		}

	} while (button != 13);

	if (clear_after)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y });
		for (int i = 0; i < text[starting_position].size() + 4; i++)
			std::cout << " ";
	}
	return starting_position;
}
int Text::Choose::Veritcal(std::vector<std::string> text, int starting_position, COORD starting_point, int spacing, Text::TextAlign text_align, bool clear_after, Window &main_window)
{
	HANDLE handle = main_window.GetHandle();
	int color1 = main_window.color1;
	int color2 = main_window.color2;
	char button;

	SetConsoleTextAttribute(handle, color1);

	for (short i = 0; i < text.size(); i++)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[i].size()), starting_point.Y + i * (short)spacing });
		std::cout << text[i];
	}
	SetConsoleTextAttribute(handle, color2);
	SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y + (short)starting_position * (short)spacing });
	std::cout << text[starting_position];

	do
	{
		button = _getch();

		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
		{
			SetConsoleTextAttribute(handle, color1);
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y + (short)starting_position * (short)spacing });
			std::cout << text[starting_position];

			starting_position += 1;
			if (starting_position == text.size())
				starting_position = 0;

			SetConsoleTextAttribute(handle, color2);
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y + (short)starting_position * (short)spacing });
			std::cout << text[starting_position];

		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
		{
			SetConsoleTextAttribute(handle, color1);
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y + (short)starting_position * (short)spacing });
			std::cout << text[starting_position];

			starting_position -= 1;
			if (starting_position < 0)
				starting_position = text.size() - 1;

			SetConsoleTextAttribute(handle, color2);
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)text_align / 2 * (float)text[starting_position].size()), starting_point.Y + (short)starting_position * (short)spacing });
			std::cout << text[starting_position];
		}

	} while (button != 13);

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
	while (true)//while player presses enter
	{
		button = _getch();
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
	return number;
}

void Text::OrdinaryText(std::vector<std::string> text, std::vector<Text::Atributes> atribute, const Text::TextAlign text_align, const short spacing, const short position, Window &MainWindow, bool clearing)
{
#ifdef DEBUG
	if (text.size() != atribute.size())
	{
		MessageBox(0, "Diffrent vector sizes", "Error", 0);
		return;
	}
#endif // DEBUG

	HANDLE handle = MainWindow.GetHandle();
	const int records = text.size();
	short current_position;
	short starting_pos;

	switch (text_align)
	{
		case Text::TextAlign::left:
		{
			starting_pos = 0;
			break;
		}
		case Text::TextAlign::right:
		{
			starting_pos = static_cast<short>(MainWindow.GetWidth());
			break;
		}
		case Text::TextAlign::center:
		{
			starting_pos = static_cast<short>(MainWindow.GetWidth()/2);
			break;
		}
	}

	if (!clearing)
	{
		SetConsoleCursorPosition(handle, { starting_pos - static_cast<short>((float)text_align / 2 * (float)text[0].size()), position });

		current_position = position;
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

				SetConsoleCursorPosition(handle, { starting_pos  - static_cast<short>((float)text_align / 2 * next_line_size), current_position });
			}
		}
	}
	else
	{
		SetConsoleCursorPosition(handle, { starting_pos - static_cast<short>((float)text_align / 2 * (float)text[0].size()), position });

		current_position = position;
		for (int i = 0; i < records; i++)
		{
			for (int j = 0; j < text[i].size(); j++)
				std::cout << " ";

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
}
void Text::TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short starting_line, Window &main_window, bool clearing)
{
#ifdef DEBUG
	if (texts_per_row == 0)
		MessageBox(0, "parametr in function TableText texts_per_row cannot be 0", "Error", 0);
	return;
#endif 

	HANDLE handle = main_window.GetHandle();
	short vertical_spacing = main_window.GetWidth() / texts_per_row;
	std::vector<std::string>::iterator it = text.begin();

	for (short i = 0; i*texts_per_row < text.size(); i++)
	{
		if (i < painted_rows)
			SetConsoleTextAttribute(handle, main_window.color2);
		else
			SetConsoleTextAttribute(handle, main_window.color1);

		for (short j = 0; j < texts_per_row && i*texts_per_row + j < text.size(); j++)
		{

			SetConsoleCursorPosition(handle, { vertical_spacing* j + vertical_spacing / 2 - static_cast<short>((float)Text::TextAlign::center / 2 * (float)it->size()), starting_line + i * (short)spacing });
			if (clearing)
			{
				for (int k = 0; k < it->size(); k++)
					std::cout << " ";
			}
			else
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