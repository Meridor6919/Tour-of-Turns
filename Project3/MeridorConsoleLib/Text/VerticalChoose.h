#pragma once
#include <iostream>
#include <conio.h>

#include "GenericChoose.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			template <template<typename, size_t...> typename TextContainer, size_t ...Args>
			class ChooseVertical : public ChooseInterface<long long>
			{
			protected:
				VerticalChooseDesc<TextContainer, Args...>* choose_desc;
				long long prev_value;

				COORD CalculatePosition(const long long& value)
				{
					const short text_align_shift = static_cast<short>(GetTextAlignScalar(choose_desc->text_align) * static_cast<float>(choose_desc->text[value].size()));
					return {
						static_cast<short>(choose_desc->point_of_reference.X - text_align_shift),
						static_cast<short>(choose_desc->point_of_reference.Y + value * choose_desc->spacing)
					};
				}
				void ClearOption(const long long& value)
				{
					SetConsoleCursorPosition(window_info->output_handle, CalculatePosition(value));
					std::cout << Spaces(choose_desc->text[value].size());
				}
				void ShowOption(const long long& value)
				{
					SetConsoleCursorPosition(window_info->output_handle, CalculatePosition(value));
					std::cout << choose_desc->text[value];
				}

			public:
				ChooseVertical(VerticalChooseDesc<TextContainer, Args...>& choose_desc, const TextInfo& window_info) :
					ChooseInterface(choose_desc, window_info),
					prev_value(choose_desc.value),
					choose_desc(&choose_desc) {}
				bool Valid() override
				{
					return static_cast<bool>(choose_desc->text.size());
				}
				void ShowInterface() override
				{
					SetColor(window_info->output_handle, window_info->main_color);
					for (size_t i = 0; i < choose_desc->value; ++i)
					{
						ShowOption(i);
					}

					SetColor(window_info->output_handle, window_info->secondary_color);
					ShowOption(choose_desc->value);

					SetColor(window_info->output_handle, window_info->main_color);
					for (size_t i = choose_desc->value + 1; i < choose_desc->text.size(); ++i)
					{
						ShowOption(i);
					}

				}
				bool ProcessInput(int button) override
				{
					const size_t vector_size = choose_desc->text.size();
					if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == KeyCodes::down_arrow)
					{
						choose_desc->value = choose_desc->value + 1;
						if (choose_desc->value == vector_size)
						{
							choose_desc->value = 0;
						}
						return true;
					}
					if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == KeyCodes::up_arrow)
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
					SetColor(window_info->output_handle, window_info->main_color);
					ShowOption(prev_value);
					SetColor(window_info->output_handle, window_info->secondary_color);
					ShowOption(choose_desc->value);

					prev_value = choose_desc->value;
				}
				void Clear() override
				{
					for (size_t i = 0; i < choose_desc->text.size(); ++i)
					{
						ClearOption(i);
					}
				}
			};
		}
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		long long ChooseVertical(VerticalChooseDesc<TextContainer, Args...>& choose_desc, const TextInfo& window_info)
		{
			Internal::ChooseVertical<TextContainer, Args...> choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl);
		}
		template <template<typename, size_t...> typename TextContainer, size_t ...Args>
		long long ChooseVertical(VerticalChooseDesc<TextContainer, Args...>& choose_desc, const TextInfo& window_info, const MultithreadingData& multithreading_data)
		{
			Internal::ChooseVertical<TextContainer, Args...> choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl, multithreading_data);
		}
	}
}