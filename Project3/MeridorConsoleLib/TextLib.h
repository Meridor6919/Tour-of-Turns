#pragma once
#include <iostream>
#include <conio.h>

#include "TextInfo.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		inline char Button(const bool* loop, std::chrono::milliseconds delay)
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

		namespace Choose
		{
			template <template<typename, size_t...> typename TextContainer, size_t ...Args>
			int Horizontal(TextInfo<TextContainer, Args...>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {})
			{
				char button;
				size_t index = text_info.starting_index;
				size_t vector_size = text_info.text.size();

				if (!vector_size)
				{
					return -1;
				}
				do
				{
					const short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[index].size()));
					const COORD position = { static_cast<short>(text_info.point_of_reference.X - text_align_shift),
						static_cast<short>(text_info.point_of_reference.Y) };

					if (multithreading_data.mutex != nullptr)
					{
						multithreading_data.mutex->lock();
					}
					SetColor(window_info.output_handle, window_info.main_color);
					SetConsoleCursorPosition(window_info.output_handle, position);
					std::cout << "< ";
					SetColor(window_info.output_handle, window_info.secondary_color);
					std::cout << text_info.text[index];
					SetColor(window_info.output_handle, window_info.main_color);
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
						SetConsoleCursorPosition(window_info.output_handle, position);
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
					else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 77 && index < vector_size - 1)
					{
						++index;
					}

				} while (button != 13);

				return static_cast<int>(index);
			}

			template <template<typename, size_t...> typename TextContainer, size_t ...Args>
			void VerticalShowGUI(TextInfo<TextContainer, Args...>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {})
			{
				if (multithreading_data.mutex != nullptr)
				{
					multithreading_data.mutex->lock();
				}
				SetColor(window_info.output_handle, window_info.main_color);
				for (short i = 0; i < static_cast<short>(text_info.text.size()); ++i)
				{
					short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[i].size()));
					const COORD position = { static_cast<short>(text_info.point_of_reference.X - text_align_shift),
						static_cast<short>(text_info.point_of_reference.Y + i * text_info.spacing) };

					SetConsoleCursorPosition(window_info.output_handle, position);
					std::cout << text_info.text[i];
				}
				if (multithreading_data.mutex != nullptr)
				{
					multithreading_data.mutex->unlock();
				}
			}

			template <template<typename, size_t...> typename TextContainer, size_t ...Args>
			void VerticalClearGUI(TextInfo<TextContainer, Args...>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {})
			{
				if (multithreading_data.mutex != nullptr)
				{
					multithreading_data.mutex->lock();
				}
				for (short i = 0; i < static_cast<short>(text_info.text.size()); ++i)
				{
					short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[i].size()));
					const COORD position = { static_cast<short>(text_info.point_of_reference.X - text_align_shift),
						static_cast<short>(text_info.point_of_reference.Y + i * text_info.spacing) };

					SetConsoleCursorPosition(window_info.output_handle, position);
					std::cout << Spaces(static_cast<int>(text_info.text[i].size()));
				}
				if (multithreading_data.mutex != nullptr)
				{
					multithreading_data.mutex->unlock();
				}
			}

			template <template<typename, size_t...> typename TextContainer, size_t ...Args>
			int Vertical(TextInfo<TextContainer, Args...>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {})
			{
				char button;

				size_t index = text_info.starting_index;
				size_t vector_size = text_info.text.size();
				if (!vector_size)
				{
					return -1;
				}
				VerticalShowGUI(text_info, window_info, multithreading_data);
				do
				{
					const short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[index].size()));
					const COORD position = { static_cast<short>(text_info.point_of_reference.X - text_align_shift),
						static_cast<short>(text_info.point_of_reference.Y + index * text_info.spacing) };

					if (multithreading_data.mutex != nullptr)
					{
						multithreading_data.mutex->lock();
					}
					SetColor(window_info.output_handle, window_info.secondary_color);
					SetConsoleCursorPosition(window_info.output_handle, position);
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
					SetColor(window_info.output_handle, window_info.main_color);
					SetConsoleCursorPosition(window_info.output_handle, position);
					std::cout << text_info.text[index];
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
				if (text_info.clear_after)
				{
					VerticalClearGUI(text_info, window_info, multithreading_data);
				}
				else
				{
					short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * static_cast<float>(text_info.text[index].size()));
					const COORD position = { static_cast<short>(text_info.point_of_reference.X - text_align_shift),
						static_cast<short>(text_info.point_of_reference.Y + index * text_info.spacing) };

					SetColor(window_info.output_handle, window_info.secondary_color);
					SetConsoleCursorPosition(window_info.output_handle, position);
					std::cout << text_info.text[index];
				}
				return static_cast<int>(index);
			}

			inline void Numeric(int* number_return_value, const int max, COORD starting_point, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
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
					SetConsoleCursorPosition(window_info.output_handle, { static_cast<short>(starting_point.X + pos), starting_point.Y });
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
				SetConsoleCursorPosition(window_info.output_handle, starting_point);
				std::cout << Spaces(pos);
				if (multithreading_data.mutex != nullptr)
				{
					multithreading_data.mutex->unlock();
				}
			}
		}
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		void OrdinaryText(TextInfo<TextContainer, Args...>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {})
		{
			size_t vector_size = text_info.text.size();
			if (multithreading_data.mutex != nullptr)
			{
				multithreading_data.mutex->lock();
			}
			for (short i = 0; i < vector_size; i += 2)
			{
				size_t line_size = text_info.text[i].size();
				if (i + 1 < vector_size)
				{
					line_size += text_info.text[i + 1].size();
				}

				short text_align_shift = static_cast<short>(GetTextAlignScalar(text_info.text_align) * line_size);
				const COORD position = { static_cast<short>(text_info.point_of_reference.X - text_align_shift),
					static_cast<short>(text_info.point_of_reference.Y + i / 2 * text_info.spacing) };


				SetConsoleCursorPosition(window_info.output_handle, position);
				SetColor(window_info.output_handle, window_info.secondary_color);
				std::cout << (text_info.clear_after ? Spaces(text_info.text[i].size()) : text_info.text[i]);
				SetColor(window_info.output_handle, window_info.main_color);
				if (i + 1 < vector_size)
				{
					std::cout << (text_info.clear_after ? Spaces(text_info.text[i + 1].size()) : text_info.text[i + 1]);
				}
			}
			if (multithreading_data.mutex != nullptr)
			{
				multithreading_data.mutex->unlock();
			}
		}

		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		void TableText(TableTextInfo<TextContainer, Args...>& table_text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {})
		{

		}
	}
}