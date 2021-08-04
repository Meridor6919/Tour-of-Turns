#include "TextLib.h"

char MeridorConsoleLib::Text::Button(const bool* loop, std::chrono::milliseconds delay)
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
int MeridorConsoleLib::Text::Choose::Horizontal(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	char button;
	size_t index = text_info.GetStartingIndex();
	size_t vector_size = text_info.GetText().size();

	if (!vector_size)
	{
		return -1;
	}
	do
	{
		const short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.GetTextAlign()) * static_cast<float>(text_info.GetText()[index].size()));
		const COORD position = { static_cast<short>(text_info.GetPointOfReference().X - text_align_shift),
			static_cast<short>(text_info.GetPointOfReference().Y) };

		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetColor(window_info.handle, window_info.main_color);
		SetConsoleCursorPosition(window_info.handle, position);
		std::cout << "< ";
		SetColor(window_info.handle, window_info.secondary_color);
		std::cout << text_info.GetText()[index];
		SetColor(window_info.handle, window_info.main_color);
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
		if (button != 13 || text_info.GetClearFlag())
		{
			SetConsoleCursorPosition(window_info.handle, position);
			std::cout << Spaces(static_cast<int>(text_info.GetText()[index].size() + 4));
		}
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 75 && index > 0)
		{
			--index;
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 77 && index < vector_size - 1)
		{
			++index;
		}

	} while (button != 13);

	return static_cast<int>(index);
}
int MeridorConsoleLib::Text::Choose::Veritcal(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	char button;

	size_t index = text_info.GetStartingIndex();
	size_t vector_size = text_info.GetText().size();
	if (!vector_size)
	{
		return -1;
	}
	VerticalShowGUI(text_info, window_info, multithreading_data);
	do
	{
		const short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.GetTextAlign()) * static_cast<float>(text_info.GetText()[index].size()));
		const COORD position = { static_cast<short>(text_info.GetPointOfReference().X - text_align_shift),
			static_cast<short>(text_info.GetPointOfReference().Y + index * text_info.GetSpacing()) };

		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetColor(window_info.handle, window_info.secondary_color);
		SetConsoleCursorPosition(window_info.handle, position);
		std::cout << text_info.GetText()[index];
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		button = Button(multithreading_data.skip_blocking_functions, multithreading_data.delay);
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->lock();
		}
		SetColor(window_info.handle, window_info.main_color);
		SetConsoleCursorPosition(window_info.handle, position);
		std::cout << text_info.GetText()[index];
		if (multithreading_data.mutex != nullptr)
		{
			multithreading_data.mutex->unlock();
		}
		if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
		{
			index += 1;
			if (index == vector_size)
			{
				index = 0;
			}
		}
		else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
		{

			if (index == 0)
			{
				index = vector_size - 1;
			}
			else
			{
				--index;
			}
		}

	} while (button != 13);
	if (text_info.GetClearFlag())
	{
		VerticalClearGUI(text_info, window_info, multithreading_data);
	}
	else
	{
		short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.GetTextAlign()) * static_cast<float>(text_info.GetText()[index].size()));
		const COORD position = { static_cast<short>(text_info.GetPointOfReference().X - text_align_shift),
			static_cast<short>(text_info.GetPointOfReference().Y + index * text_info.GetSpacing()) };

		SetColor(window_info.handle, window_info.secondary_color);
		SetConsoleCursorPosition(window_info.handle, position);
		std::cout << text_info.GetText()[index];
	}
	return static_cast<int>(index);
}
void MeridorConsoleLib::Text::Choose::VerticalShowGUI(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	SetColor(window_info.handle, window_info.main_color);
	for (short i = 0; i < static_cast<short>(text_info.GetText().size()); ++i)
	{
		short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.GetTextAlign()) * static_cast<float>(text_info.GetText()[i].size()));
		const COORD position = { static_cast<short>(text_info.GetPointOfReference().X - text_align_shift),
			static_cast<short>(text_info.GetPointOfReference().Y + i * text_info.GetSpacing()) };

		SetConsoleCursorPosition(window_info.handle, position);
		std::cout << text_info.GetText()[i];
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void MeridorConsoleLib::Text::Choose::VerticalClearGUI(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	for (short i = 0; i < static_cast<short>(text_info.GetText().size()); ++i)
	{
		short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.GetTextAlign()) * static_cast<float>(text_info.GetText()[i].size()));
		const COORD position = { static_cast<short>(text_info.GetPointOfReference().X - text_align_shift),
			static_cast<short>(text_info.GetPointOfReference().Y + i * text_info.GetSpacing()) };

		SetConsoleCursorPosition(window_info.handle, position);
		std::cout << Spaces(static_cast<int>(text_info.GetText()[i].size()));
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void MeridorConsoleLib::Text::Choose::Numeric(int* number_return_value, const int max, COORD starting_point, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
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
		SetConsoleCursorPosition(window_info.handle, { static_cast<short>(starting_point.X + pos), starting_point.Y });
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
void MeridorConsoleLib::Text::OrdinaryText(TextInfo& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	size_t vector_size = text_info.GetText().size();
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	for (short i = 0; i < vector_size; i += 2)
	{
		size_t line_size = text_info.GetText()[i].size();
		if (i + 1 < vector_size)
		{
			line_size += text_info.GetText()[i + 1].size();
		}

		short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.GetTextAlign()) * line_size);
		const COORD position = { static_cast<short>(text_info.GetPointOfReference().X - text_align_shift),
			static_cast<short>(text_info.GetPointOfReference().Y + i / 2 * text_info.GetSpacing()) };

		
		SetConsoleCursorPosition(window_info.handle, position);
		SetColor(window_info.handle, window_info.secondary_color);
		std::cout << (text_info.GetClearFlag() ? Spaces(text_info.GetText()[i].size()) : text_info.GetText()[i]);
		SetColor(window_info.handle, window_info.main_color);
		if (i + 1 < vector_size)
		{
			std::cout << (text_info.GetClearFlag() ? Spaces(text_info.GetText()[i + 1].size()) : text_info.GetText()[i + 1]);
		}
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
void MeridorConsoleLib::Text::TableText(TableTextInfo& table_text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	int column_number = 0;
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->lock();
	}
	for (short i = 0; i < table_text_info.GetText().size(); ++i)
	{
		int current_column = i % table_text_info.GetNumberOfColumns();
		if (!current_column)
		{
			if (column_number < table_text_info.GetNumberOfPaintedRows())
			{
				SetColor(window_info.handle, window_info.secondary_color);
			}
			else
			{
				SetColor(window_info.handle, window_info.main_color);
			}
			++column_number;
		}
		short general_spacing_value = table_text_info.GetVerticalSpacing() * current_column + table_text_info.GetVerticalSpacing() / 2;
		COORD position = { table_text_info.GetPointOfReference().X + general_spacing_value - static_cast<short>(GetTextAlignScalar(table_text_info.GetTextAlign()) * static_cast<float>(table_text_info.GetText()[i].size())),
								table_text_info.GetPointOfReference().Y + i * table_text_info.GetHorizontalSpacing()};
		SetConsoleCursorPosition(window_info.handle, position);

		std::cout << (table_text_info.GetClearFlag() ? Spaces(table_text_info.GetText()[i].size()) : table_text_info.GetText()[i]);
	}
	if (multithreading_data.mutex != nullptr)
	{
		multithreading_data.mutex->unlock();
	}
}
