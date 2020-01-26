#include "TextLib.h"

int Text::Choose::Horizontal(const std::vector<std::string> text, int starting_position, const COORD starting_point, const TextAlign text_align, const bool clear_after, Window &main_window, std::mutex *mutex)
{
	if (!static_cast<short>(text.size()))
	{
		return -1;
	}
	const HANDLE handle = main_window.GetHandle();
	const int color1 = main_window.color1;
	const int color2 = main_window.color2;
	char button;
	do
	{
		const float line_size = static_cast<float>(text[starting_position].size());
		//Showing text with current index
		if (mutex != nullptr)
		{
			mutex->lock();
		}
		SetConsoleTextAttribute(handle, color1);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * line_size), starting_point.Y });
		std::cout << "< ";
		SetConsoleTextAttribute(handle, color2);
		std::cout << text[starting_position];
		SetConsoleTextAttribute(handle, color1);
		std::cout << " >";
		if (mutex != nullptr)
		{
			mutex->unlock();
		}
		button = _getch();
		if (mutex != nullptr)
		{
			mutex->lock();
		}
		//Clearing text from the screen
		if (button != 13 || clear_after)
		{
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * line_size), starting_point.Y });
			for (float i = 0; i < line_size + 4; ++i)
			{
				std::cout << " ";
			}
		}
		if (mutex != nullptr)
		{
			mutex->unlock();
		}
		//Changing index when left or right arrow button is pressed
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 75 && starting_position > 0)
		{
			starting_position -= 1;
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 77 && starting_position < static_cast<int>(text.size()) - 1)
		{
			starting_position += 1;
		}

	} while (button != 13);

	return starting_position;
}
int Text::Choose::Veritcal(const std::vector<std::string> text, short starting_position, const COORD starting_point, const short spacing, const TextAlign text_align, const bool clear_after, Window &main_window, std::mutex *mutex)
{
	const HANDLE handle = main_window.GetHandle();
	const int color1 = main_window.color1;
	const int color2 = main_window.color2;
	const int text_size = static_cast<int>(text.size());
	const float text_align_float = static_cast<float>(text_align);
	char button;

	//Showing text on the screen
	if (mutex != nullptr)
	{
		mutex->lock();
	}
	SetConsoleTextAttribute(handle, color1);
	for (short i = 0; i < text_size; ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(text_align_float / 2.0f * static_cast<float>(text[i].size())), starting_point.Y + i * spacing });
		std::cout << text[i];
	}
	if (mutex != nullptr)
	{
		mutex->unlock();
	}
	do
	{
		//Changing active text's color
		if (mutex != nullptr)
		{
			mutex->lock();
		}
		SetConsoleTextAttribute(handle, color2);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(text_align_float / 2.0f * static_cast<float>(text[starting_position].size())), starting_point.Y + starting_position * spacing });
		std::cout << text[starting_position];
		if (mutex != nullptr)
		{
			mutex->unlock();
		}
		button = _getch();
		if (mutex != nullptr)
		{
			mutex->lock();
		}
		//Changing color of active text back to normal
		//I assume that user will press the correct button and position will change
		SetConsoleTextAttribute(handle, color1);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(text_align_float / 2.0f * static_cast<float>(text[starting_position].size())), starting_point.Y + starting_position * spacing });
		std::cout << text[starting_position];
		if (mutex != nullptr)
		{
			mutex->unlock();
		}
		//Changing the index when up or down arrow button is pressed
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
		{
			starting_position += 1;
			if (starting_position == text_size)
				starting_position = 0;
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
		{
			starting_position -= 1;
			if (starting_position < 0)
				starting_position = text_size - 1;
		}

	} while (button != 13);

	//Clearing the text if flag is active
	if (mutex != nullptr)
	{
		mutex->lock();
	}
	if (clear_after)
	{
		for (short i = 0; i < text_size; ++i)
		{
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(text_align_float / 2.0f * static_cast<float>(text[i].size())), starting_point.Y + i * spacing });
			for (int j = 0; j < static_cast<int>(text[i].size()); ++j)
			{
				std::cout << " ";
			}
		}
	}
	else
	{
		SetConsoleTextAttribute(handle, color2);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(text_align_float / 2.0f * static_cast<float>(text[starting_position].size())), starting_point.Y + starting_position * spacing });
		std::cout << text[starting_position];
	}
	if (mutex != nullptr)
	{
		mutex->unlock();
	}
	return starting_position;
}
int Text::Choose::Numeric(const int max, COORD starting_point, const bool zero_allowed, Window &main_window, std::mutex *mutex)
{
	char button;
	int number = 0;
	short pos = 0;
	while (true)
	{
		SetConsoleCursorPosition(main_window.GetHandle(), { starting_point.X + pos, starting_point.Y });
		button = _getch();
		if (mutex != nullptr)
		{
			mutex->lock();
		}
		//Adding if user pressed key between 0 and 9 
		if (button >= '0' && button <= '9')
		{
			if (button == '0' && pos == 0)
			{
				if (mutex != nullptr)
				{
					mutex->unlock();
				}
				continue;
			}
			else if (number * 10 + button - 48 > max)
			{
				if (mutex != nullptr)
				{
					mutex->unlock();
				}
				continue;
			}
			std::cout << button;
			number = number * 10 + button - 48;
			++pos;
		}
		//erasing last number if user pressed backspace
		if (button == '\b' && pos != 0)
		{
			std::cout << "\b \b";
			number /= 10;
			--pos;
		}
		if (mutex != nullptr)
		{
			mutex->unlock();
		}
		if (button == 13)
		{
			if (pos == 0 && !zero_allowed)
			{
				continue;
			}
			break;
		}
	}
	//Clearing text from the screen
	if (mutex != nullptr)
	{
		mutex->lock();
	}
	SetConsoleCursorPosition(main_window.GetHandle(), starting_point);
	for (int i = 1; i <= number; i *= 10)
	{
		std::cout << " ";
	}
	if (mutex != nullptr)
	{
		mutex->unlock();
	}
	return number;
}
void Text::OrdinaryText(std::vector<std::string> text, const TextAlign text_align, const short spacing, const COORD position, Window &main_window, const bool clearing, std::mutex *mutex)
{
	HANDLE handle = main_window.GetHandle();
	const int text_size = static_cast<int>(text.size());

	//Changing all chars in all members of the text array to ' ' if flag is active
	if (clearing)
	{
		for (int i = 0; i <text_size; ++i)
		{
			for (int j = 0; j < static_cast<int>(text[i].size()); ++j)
			{
				text[i][j] = ' ';
			}
		}
	}
	if (text_size % 2)
	{
		text.push_back("");
	}
	//Showing the text
	if (mutex != nullptr)
	{
		mutex->lock();
	}
	for (short i = 0; i < text_size; i+=2)
	{
		const short line_size = static_cast<short>(text[i].size()) + static_cast<short>(text[i + 1].size());
		SetConsoleCursorPosition(handle, { position.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * line_size), position.Y+i/2*spacing });
		SetConsoleTextAttribute(handle, main_window.color2);
		std::cout << text[i];
		SetConsoleTextAttribute(handle, main_window.color1);
		std::cout << text[i + 1];
	}
	if (mutex != nullptr)
	{
		mutex->unlock();
	}
}
void Text::TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short vertical_spacing, const COORD starting_point, Window &main_window, const bool clearing, std::mutex *mutex)
{
	HANDLE handle = main_window.GetHandle();
	std::vector<std::string>::iterator it = text.begin();
	const int text_size = static_cast<int>(text.size());
	if (mutex != nullptr)
	{
		mutex->lock();
	}
	for (int i = 0; i*texts_per_row < text_size; ++i)
	{
		//Setting right color
		if (i < painted_rows)
		{
			SetConsoleTextAttribute(handle, main_window.color2);
		}
		else
		{
			SetConsoleTextAttribute(handle, main_window.color1);
		}
		//Changing all chars in all members of the text array to ' ' if flag is active
		if (clearing)
		{
			for (int j = 0; j < text_size; j++)
			{
				for (int k = 0; k < static_cast<int>(text[j].size()); ++k)
				{
					text[j][k] = ' ';
				}
			}
		}
		//Showing the text
		for (int j = 0; j < texts_per_row && i*texts_per_row + j < text_size; ++j)
		{
			SetConsoleCursorPosition(handle, { starting_point.X + vertical_spacing * static_cast<short>(j) + vertical_spacing / 2 - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(it->size())),
				starting_point.Y + static_cast<short>(i * spacing) });
			std::cout << *it;
			++it;
		}
	}
	if (mutex != nullptr)
	{
		mutex->unlock();
	}
}
void Text::Spaces(const int i, std::mutex *mutex)
{
	if (mutex != nullptr)
	{
		mutex->lock();
	}
	for (int j = 0; j < i; ++j)
	{
		std::cout << " ";
	}
	if (mutex != nullptr)
	{
		mutex->unlock();
	}
}