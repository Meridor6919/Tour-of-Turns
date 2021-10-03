#pragma once
#include <iostream>
#include <conio.h>

#include "ChooseGeneric.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			template <template<typename, size_t...> typename TextContainer, size_t ...Args>
			class ChooseHorizontal : public ChooseInterface<long long>
			{
			protected:
				HorizontalChooseDesc<TextContainer, Args...>* choose_desc;
				COORD position{};
				size_t lenght;

				void Update(const long long& value)
				{
					lenght = choose_desc->text[value].size() + choose_desc->decoration_left.size() + choose_desc->decoration_right.size();

					const short text_align_shift = static_cast<short>(GetTextAlignScalar(choose_desc->text_align) * static_cast<float>(lenght));
					position = {
						static_cast<short>(choose_desc->point_of_reference.X - text_align_shift),
						static_cast<short>(choose_desc->point_of_reference.Y)
					};
				}

			public:
				ChooseHorizontal(HorizontalChooseDesc<TextContainer, Args...>& choose_desc, const WindowInfo& window_info) :
					ChooseInterface(choose_desc, window_info),
					choose_desc(&choose_desc) {}
				bool Valid() override
				{
					if (choose_desc->text.size() < choose_desc->value + 1)
					{
						return false;
					}
					else
					{
						Update(choose_desc->value);
						return true;
					}
				}
				void ShowInterface() override
				{
					SetConsoleCursorPosition(window_info->output_handle, position);
					SetColor(window_info->output_handle, window_info->secondary_color);
					std::cout << choose_desc->decoration_left;

					SetColor(window_info->output_handle, window_info->main_color);
					std::cout << choose_desc->text[choose_desc->value];

					SetColor(window_info->output_handle, window_info->secondary_color);
					std::cout << choose_desc->decoration_right;
				}
				bool ProcessInput(int button) override
				{
					const size_t vector_size = choose_desc->text.size();
					if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == KeyCodes::right_arrow)
					{
						choose_desc->value = choose_desc->value + 1;
						if (choose_desc->value == vector_size)
						{
							choose_desc->value = 0;
						}
						return true;
					}
					else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == KeyCodes::left_arrow)
					{
						if (choose_desc->value == 0)
						{
							choose_desc->value = vector_size;
						}
						choose_desc->value = choose_desc->value - 1;
						return true;
					}
					return false;
				}
				void UpdateInterface() override
				{
					Clear();
					Update(choose_desc->value);
					ShowInterface();
				}
				void Clear() override
				{
					SetConsoleCursorPosition(window_info->output_handle, position);
					std::cout << Spaces(lenght);
				}
			};
		}
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		long long ChooseHorizontal(HorizontalChooseDesc<TextContainer, Args...>& choose_desc, const WindowInfo& window_info)
		{
			Internal::ChooseHorizontal<TextContainer, Args...> choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl);
		}
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		long long ChooseHorizontal(HorizontalChooseDesc<TextContainer, Args...>& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
		{
			Internal::ChooseHorizontal<TextContainer, Args...> choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl, multithreading_data);
		}
	}
}