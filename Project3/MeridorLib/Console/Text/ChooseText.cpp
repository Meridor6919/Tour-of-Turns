#include "ChooseText.h"
namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			COORD ChooseText::GetPosition() const noexcept
			{
				const short text_align_shift = static_cast<short>(GetTextAlignScalar(choose_desc->text_align) * static_cast<float>(choose_desc->value.size()));
				return {
					static_cast<short>(choose_desc->point_of_reference.X - text_align_shift),
					static_cast<short>(choose_desc->point_of_reference.Y)
				};
			}
			ChooseText::ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info) noexcept : ChooseInterface(choose_desc, window_info)
			{
				this->choose_desc = &choose_desc;
				std::sort(choose_desc.dictionary.begin(), choose_desc.dictionary.end());
			}
			bool ChooseText::Valid() noexcept
			{
				return choose_desc->value.size() <= choose_desc->maximum_size;
			}
			void ChooseText::ShowInterface() const noexcept
			{
				SetColor(window_info->output_handle, window_info->main_color);
				SetConsoleCursorPosition(window_info->output_handle, GetPosition());
				std::cout << choose_desc->value + ' ';
			}
			bool ChooseText::ProcessInput(int button) noexcept
			{
				if (choose_desc->value.size() < choose_desc->maximum_size && std::binary_search(choose_desc->dictionary.begin(), choose_desc->dictionary.end(), static_cast<char>(button)))
				{
					if (button == KeyCodes::space && choose_desc->special_space_rules && (choose_desc->value.size() == 0 || *choose_desc->value.rbegin() == KeyCodes::space))
					{
						return false;
					}
					choose_desc->value += static_cast<char>(button);
				}
				else if (button == KeyCodes::backspace && choose_desc->value.size() != 0)
				{
					choose_desc->value.erase(choose_desc->value.end() - 1);
				}
				else
				{
					return false;
				}
				return true;
			}
			void ChooseText::UpdateInterface() noexcept
			{
				ShowInterface();
			}
			void ChooseText::Clear() const noexcept
			{
				SetConsoleCursorPosition(window_info->output_handle, GetPosition());
				std::cout << Spaces(choose_desc->value.size());
			}
		}
		std::string ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info) noexcept
		{
			Internal::ChooseText choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl);
		}
		std::string ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data) noexcept
		{
			Internal::ChooseText choose_impl(choose_desc, window_info);
			return Internal::GenericChoose(choose_impl, multithreading_data);
		}
	}
}