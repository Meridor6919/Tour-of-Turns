#include "TextLib.h"

char Text::Button(const bool *loop, std::chrono::milliseconds delay)
{
	if (loop == nullptr)
	{
		return _getch();
	}
	while (*loop)
	{
		if (_kbhit())
		{
			return _getch();
		}
		std::this_thread::sleep_for(delay);
	}
	return 13;
}
int Text::Choose::Horizontal(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	char button;
	size_t index = text_info.starting_index;
	do
	{
		const short text_align_shift = static_cast<short>(static_cast<float>(text_info.text_align) / 2.0f * static_cast<float>(text_info.text[index].size()));
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetConsoleTextAttribute(window_info.handle, window_info.main_color);
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y });
		std::cout << "< ";
		SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
		std::cout << text_info.text[index];
		SetConsoleTextAttribute(window_info.handle, window_info.main_color);
		std::cout << " >";
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		button = Button(multithreading_data.loop, multithreading_data.delay);
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		if (button != 13 || text_info.clear_after)
		{
			SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y });
			Text::Spaces(static_cast<int>(text_info.text[index].size() + 4));
		}
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 75 && index > 0)
		{
			--index;
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 77 && index < text_info.text.size() - 1)
		{
			++index;
		}

	} while (button != 13);

	return index;
}
int Text::Choose::Veritcal(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	char button;
	
	size_t index = text_info.starting_index;
	VerticalShowGUI(text_info, window_info, multithreading_data);
	do
	{
		const short text_align_shift = static_cast<short>(static_cast<float>(text_info.text_align) / 2.0f * static_cast<float>(text_info.text[index].size()));
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + index * text_info.spacing });
		std::cout << text_info.text[index];
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		button = Button(multithreading_data.loop, multithreading_data.delay);
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetConsoleTextAttribute(window_info.handle, window_info.main_color);
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + index * text_info.spacing });
		std::cout << text_info.text[index];
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
		{
			index += 1;
			if (index == text_info.text.size())
				index = 0;
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
		{
			index -= 1;
			if (index < 0)
				index = text_info.text.size() - 1;
		}

	} while (button != 13);
	if (text_info.clear_after)
	{
		VerticalClearGUI(text_info, window_info, multithreading_data);
	}
	else
	{
		SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
		short text_align_shift = static_cast<short>(static_cast<float>(text_info.text_align) / 2.0f * static_cast<float>(text_info.text[index].size()));
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + index * text_info.spacing });
		std::cout << text_info.text[index];
	}
	return index;
}
void Text::Choose::VerticalShowGUI(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	SetConsoleTextAttribute(window_info.handle, window_info.main_color);
	for (short i = 0; i < static_cast<short>(text_info.text.size()); ++i)
	{
		short text_align_shift = static_cast<short>(static_cast<float>(text_info.text_align) / 2.0f * static_cast<float>(text_info.text[i].size()));
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + i * text_info.spacing });
		std::cout << text_info.text[i];
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void Text::Choose::VerticalClearGUI(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	SetConsoleTextAttribute(window_info.handle, window_info.main_color);
	for (short i = 0; i < static_cast<short>(text_info.text.size()); ++i)
	{
		short text_align_shift = static_cast<short>(static_cast<float>(text_info.text_align) / 2.0f * static_cast<float>(text_info.text[i].size()));
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + i * text_info.spacing });
		Spaces(static_cast<int>(text_info.text[i].size()));
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
int Text::Choose::Numeric(const int max, COORD starting_point, const bool zero_allowed, Window &main_window)
{
	char button;
	int number = 0;
	short pos = 0;
	while (true)
	{
		SetConsoleCursorPosition(main_window.GetHandle(), { starting_point.X + pos, starting_point.Y });
		button = _getch();
		//Adding if user pressed key between 0 and 9 
		if (button >= '0' && button <= '9')
		{
			if (button == '0' && pos == 0)
			{
				continue;
			}
			else if (number * 10 + button - 48 > max)
			{
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
	SetConsoleCursorPosition(main_window.GetHandle(), starting_point);
	Text::Spaces(pos);
	return number;
}
void Text::OrdinaryText(std::vector<std::string> text, const TextAlign text_align, const short spacing, const COORD position, Window & main_window, std::mutex *mutex, const bool clearing)
{
	HANDLE handle = main_window.GetHandle();
	const int text_size = static_cast<int>(text.size());

	//Changing all chars in all members of the text array to ' ' if flag is active
	if (clearing)
	{
		for (int i = 0; i < text_size; ++i)
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
	for (short i = 0; i < text_size; i += 2)
	{
		const short line_size = static_cast<short>(text[i].size()) + static_cast<short>(text[i + 1].size());
		SetConsoleCursorPosition(handle, { position.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * line_size), position.Y + i / 2 * spacing });
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
void Text::OrdinaryText(std::vector<std::string> text, const TextAlign text_align, const short spacing, const COORD position, Window & main_window, const bool clearing)
{
	HANDLE handle = main_window.GetHandle();
	const int text_size = static_cast<int>(text.size());

	//Changing all chars in all members of the text array to ' ' if flag is active
	if (clearing)
	{
		for (int i = 0; i < text_size; ++i)
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
	for (short i = 0; i < text_size; i += 2)
	{
		const short line_size = static_cast<short>(text[i].size()) + static_cast<short>(text[i + 1].size());
		SetConsoleCursorPosition(handle, { position.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * line_size), position.Y + i / 2 * spacing });
		SetConsoleTextAttribute(handle, main_window.color2);
		std::cout << text[i];
		SetConsoleTextAttribute(handle, main_window.color1);
		std::cout << text[i + 1];
	}
}
void Text::TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short vertical_spacing, const COORD starting_point, Window &main_window, std::mutex *mutex, const bool clearing)
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
void Text::TableText(std::vector<std::string> text, const int painted_rows, const int texts_per_row, const short spacing, const short vertical_spacing, const COORD starting_point, Window &main_window, const bool clearing)
{
	HANDLE handle = main_window.GetHandle();
	std::vector<std::string>::iterator it = text.begin();
	const int text_size = static_cast<int>(text.size());
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
}
void Text::Spaces(const int i, std::mutex *mutex)
{
	if (mutex != nullptr)
	{
		mutex->lock();
	}
	Text::Spaces(i);
	if (mutex != nullptr)
	{
		mutex->unlock();
	}
}
void Text::Spaces(const int i)
{
	for (int j = 0; j < i; ++j)
	{
		std::cout << ' ';
	}
}