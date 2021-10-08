#pragma once
#include <utility>
#include "DataStructures.h"
namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			template <template<typename, size_t...> typename TextContainer, size_t ...Args>
			class TableClass
			{
				TableDesc<TextContainer, Args...>* table_desc;
				const WindowInfo* window_info;
				short column_width;

				[[nodiscard]] short GetColumnWidth() const noexcept
				{
					return static_cast<short>((table_desc->box_size.X - (table_desc->text.size() - 1) * table_desc->horizontal_spacing) / table_desc->text.size());
				}
				[[nodiscard]] short GetAlignShift(short lenght) const noexcept
				{
					return static_cast<short>(static_cast<float>(column_width - lenght) * GetTextAlignScalar(table_desc->text_align));
				}
				void SelectCorrectColor(int index) const noexcept
				{
					if (table_desc->number_of_painted_rows > index)
					{
						SetColor(window_info->output_handle, window_info->secondary_color);
					}
					else
					{
						SetColor(window_info->output_handle, window_info->main_color);
					}
				}

			public:
				TableClass(TableDesc<TextContainer, Args...>& table_desc, const WindowInfo& window_info) :
					table_desc(&table_desc),
					window_info(&window_info) 
				{
					column_width = GetColumnWidth();
				}
				void Draw()
				{
					COORD position = { table_desc->position.X, table_desc->position.Y };
					bool iterate = true;
					for (int j = 0; iterate; ++j)
					{
						iterate = false;
						SelectCorrectColor(j);

						for (int i = 0; i < table_desc->text.size(); ++i)
						{
							if (table_desc->text[i].size() > j)
							{
								iterate = true;
								SetConsoleCursorPosition(window_info->output_handle, { static_cast<short>(position.X + GetAlignShift(table_desc->text[i][j].size())), position.Y });
								std::cout << table_desc->text[i][j];
								position.X += column_width;
							}
						}
						position.X = table_desc->position.X;
						position.Y += table_desc->vertical_spacing;
					}
				}
				void Clear()
				{
					std::string spaces = Spaces(table_desc->box_size.X);
					for (short i = table_desc->position.Y; i < table_desc->position.Y + table_desc->box_size.Y; ++i)
					{
						SetConsoleCursorPosition(window_info->output_handle, { table_desc->position.X,i });
						std::cout << spaces;
					}
				}
				bool ClearInstead()
				{
					return table_desc->clear_instead;
				}
			};

			template <class T>
			void Table(T& table_class)
			{
				if (table_class.ClearInstead())
				{
					table_class.Clear();
				}
				else
				{
					table_class.Draw();
				}
			}
			template <class T>
			void Table(T& table_class, const MultithreadingData& multithreading_data)
			{
				multithreading_data.mutex->lock();
				if (table_class.ClearInstead())
				{
					table_class.Clear();
				}
				else
				{
					table_class.Draw();
				}
				multithreading_data.mutex->unlock();
			}
		}
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		void Table(TableDesc<TextContainer, Args...>& choose_desc, const WindowInfo& window_info)
		{
			Internal::TableClass<TextContainer, Args...> table_impl(choose_desc, window_info);
			return Internal::Table(table_impl);
		}
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		void Table(TableDesc<TextContainer, Args...>& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
		{
			Internal::TableClass<TextContainer, Args...> table_impl(choose_desc, window_info);
			return Internal::Table(table_impl, multithreading_data);
		}
	}
}