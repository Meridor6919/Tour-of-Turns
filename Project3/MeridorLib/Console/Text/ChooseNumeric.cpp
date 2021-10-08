#include "ChooseNumeric.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			int ChooseNumeric::CalculateLenght(long long value) const noexcept
			{
				int lenght = 0;
				if (value < 0)
				{
					++lenght;
				}
				for (; value != 0; ++lenght)
				{
					value /= 10;
				}
				return lenght;
			}
			COORD ChooseNumeric::GetPosition(int lenght) const noexcept
			{
				const short text_align_shift = static_cast<short>(GetTextAlignScalar(choose_desc->text_align) * static_cast<float>(lenght));
				return {
					static_cast<short>(choose_desc->point_of_reference.X - text_align_shift),
					static_cast<short>(choose_desc->point_of_reference.Y)
				};
			}
			bool ChooseNumeric::InBounds(const long long value) const noexcept
			{
				return value >= choose_desc->lower_bound && value <= choose_desc->upper_bound;
			}
			void ChooseNumeric::ClearInterface(int lenght) const noexcept
			{
				SetConsoleCursorPosition(window_info->output_handle, GetPosition(lenght));
				std::cout << Spaces(lenght);
			}
			ChooseNumeric::ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info) noexcept : ChooseInterface(choose_desc, window_info)
			{
				this->choose_desc = &choose_desc;
				lenght = CalculateLenght(choose_desc.value);
				prev_lenght = lenght;
			}
			bool ChooseNumeric::Valid() noexcept
			{
				return InBounds(choose_desc->value);
			}
			void ChooseNumeric::ShowInterface() const noexcept
			{
				SetColor(window_info->output_handle, window_info->main_color);
				SetConsoleCursorPosition(window_info->output_handle, GetPosition(lenght));
				if (choose_desc->value)
				{
					std::cout << choose_desc->value;
				}
			}
			bool ChooseNumeric::ProcessInput(int button) noexcept
			{
				if (button >= '0' && button <= '9')
				{
					int digit = (button - '0') * (choose_desc->value < 0 ? -1 : 1);
					auto new_value = choose_desc->value * 10 + digit;
					if (InBounds(new_value) && new_value != choose_desc->value)
					{
						choose_desc->value = new_value;
						++lenght;
						return true;
					}
				}
				else if (button == '-')
				{
					if (InBounds(-choose_desc->value) && choose_desc->value)
					{
						lenght += choose_desc->value < 0 ? -1 : 1;
						choose_desc->value = choose_desc->value * -1;
						return true;
					}
				}
				else if (button == '\b')
				{
					auto new_value = choose_desc->value / 10;
					if (InBounds(new_value))
					{
						choose_desc->value = new_value;
						--lenght;
						if (!new_value)
						{
							lenght = 0;
						}
						return true;
					}
				}
				return false;
			}
			void ChooseNumeric::UpdateInterface() noexcept
			{
				ClearInterface(prev_lenght);
				ShowInterface();
				prev_lenght = lenght;
			}
			void ChooseNumeric::Clear() const noexcept
			{
				SetConsoleCursorPosition(window_info->output_handle, GetPosition(lenght));
				std::cout << Spaces(lenght);
			}
		}
		long long ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info) noexcept
		{
			Internal::ChooseNumeric choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl);
		}
		long long ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data) noexcept
		{
			Internal::ChooseNumeric choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl, multithreading_data);
		}
	}
}
