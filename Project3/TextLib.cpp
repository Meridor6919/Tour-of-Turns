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
float Text::GetTextAlignScalar(TextAlign text_align)
{
	return static_cast<float>(text_align) / 2.0f;;
}
int Text::Choose::Horizontal(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	char button;
	size_t index = text_info.starting_index;
	do
	{
		const short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[index].size()));
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
		button = Button(multithreading_data.skip_blocking_functions, multithreading_data.delay);
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		if (button != 13 || text_info.clear_after)
		{
			SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y });
			std::cout << Spaces(static_cast<int>(text_info.text[index].size() + 4));
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

	return static_cast<int>(index);
}
int Text::Choose::Veritcal(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	char button;
	
	size_t index = text_info.starting_index;
	VerticalShowGUI(text_info, window_info, multithreading_data);
	do
	{
		const short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[index].size()));
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + static_cast<short>(index) * text_info.spacing });
		std::cout << text_info.text[index];
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		button = Button(multithreading_data.skip_blocking_functions, multithreading_data.delay);
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetConsoleTextAttribute(window_info.handle, window_info.main_color);
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + static_cast<short>(index) * text_info.spacing });
		std::cout << text_info.text[index];
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
		{
			index += 1;
			if (index == text_info.text.size())
			{
				index = 0;
			}
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
		{
			
			if (index == 0)
			{
				index = text_info.text.size() - 1;
			}
			else
			{
				--index;
			}
		}

	} while (button != 13);
	if (text_info.clear_after)
	{
		VerticalClearGUI(text_info, window_info, multithreading_data);
	}
	else
	{
		SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
		short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[index].size()));
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + static_cast<short>(index) * text_info.spacing });
		std::cout << text_info.text[index];
	}
	return static_cast<int>(index);
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
		short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[i].size()));
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
	for (short i = 0; i < static_cast<short>(text_info.text.size()); ++i)
	{
		short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[i].size()));
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - text_align_shift, text_info.point_of_reference.Y + i * text_info.spacing });
		std::cout << Spaces(static_cast<int>(text_info.text[i].size()));
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void Text::Choose::Numeric(int* number_return_value, const int max, COORD starting_point, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	short pos = 0;
	char button;
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	do
	{
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		button = Button(multithreading_data.skip_blocking_functions, multithreading_data.delay);
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetConsoleCursorPosition(window_info.handle, { starting_point.X + pos, starting_point.Y });
		if (button >= '0' && button <= '9')
		{
			if (button == '0' && pos == 0)
			{
				continue;
			}
			else if ((*number_return_value) * 10 + button - 48 > max)
			{
				continue;
			}
			std::cout << button;
			(*number_return_value) = (*number_return_value) * 10 + button - 48;
			++pos;
		}
		if (button == '\b' && pos != 0)
		{
			std::cout << "\b \b";
			(*number_return_value) /= 10;
			--pos;
		}
	} while (button != 13);
	SetConsoleCursorPosition(window_info.handle, starting_point);
	std::cout << Spaces(pos);
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void Text::OrdinaryText(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	const int text_size = static_cast<int>(text_info.text.size());
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	for (short i = 0; i < text_size; i += 2)
	{
		short line_size = static_cast<short>(text_info.text[i].size());
		if (i + 1 < text_size)
		{
			line_size += static_cast<short>(text_info.text[i + 1].size());
		}
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - static_cast<short>(GetTextAlignScalar(text_info.text_align) * line_size),
			text_info.point_of_reference.Y + i / 2 * text_info.spacing });
		SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
		std::cout << text_info.text[i];
		SetConsoleTextAttribute(window_info.handle, window_info.main_color);
		if (i + 1 < text_size)
		{
			std::cout << text_info.text[i + 1];
		}
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void Text::ClearOrdinaryText(const TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	const int text_size = static_cast<int>(text_info.text.size());
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	for (short i = 0; i < text_size; i += 2)
	{
		const short line_size = static_cast<short>(text_info.text[i].size()) + static_cast<short>(text_info.text[i + 1].size());
		SetConsoleCursorPosition(window_info.handle, { text_info.point_of_reference.X - static_cast<short>(GetTextAlignScalar(text_info.text_align) * line_size),
			text_info.point_of_reference.Y + i / 2 * text_info.spacing });
		std::cout << Spaces(static_cast<int>(text_info.text[i].size()));
		if (i + 1 < text_size)
		{
			std::cout << Spaces(static_cast<int>(text_info.text[i + 1].size()));
		}
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void Text::TableText(const TableTextInfo& table_text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	const int text_size = static_cast<int>(table_text_info.text.size());
	int column_number = 0;
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	for (short i = 0; i < text_size; ++i)
	{
		int current_column = i % table_text_info.number_of_columns;
		if (!current_column)
		{
			if (column_number < table_text_info.painted_rows)
			{
				SetConsoleTextAttribute(window_info.handle, window_info.secondary_color);
			}
			else
			{
				SetConsoleTextAttribute(window_info.handle, window_info.main_color);
			}
			++column_number;
		}
		short general_spacing_value = table_text_info.vertical_spacing * current_column + table_text_info.vertical_spacing / 2;
		COORD position = { table_text_info.point_of_reference.X + general_spacing_value - static_cast<short>(GetTextAlignScalar(table_text_info.text_align) * static_cast<float>(table_text_info.text[i].size())),
								table_text_info.point_of_reference.Y + i * table_text_info.horizontal_spacing };
		SetConsoleCursorPosition(window_info.handle, position);
		if (table_text_info.clear)
		{
			std::cout << Spaces(static_cast<int>(table_text_info.text[i].size()));
		}
		else
		{
			std::cout << table_text_info.text[i];
		}
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
std::string Text::GetMonoCharacterString(const int size, const char character)
{
	std::string ret = "";
	ret.resize(size, character);
	return ret;
}
std::string Text::Spaces(const int size)
{
	return GetMonoCharacterString(size, ' ');
}