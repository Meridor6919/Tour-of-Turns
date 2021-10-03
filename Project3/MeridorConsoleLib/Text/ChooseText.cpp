#include "ChooseText.h"

COORD MeridorConsoleLib::Text::Internal::ChooseText::GetPosition()
{
	const short text_align_shift = static_cast<short>(GetTextAlignScalar(choose_desc->text_align) * static_cast<float>(choose_desc->value.size()));
	return {
		static_cast<short>(choose_desc->point_of_reference.X - text_align_shift),
		static_cast<short>(choose_desc->point_of_reference.Y)
	};
}
MeridorConsoleLib::Text::Internal::ChooseText::ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info) : ChooseInterface(choose_desc, window_info)
{
	this->choose_desc = &choose_desc;
	std::sort(choose_desc.dictionary.begin(), choose_desc.dictionary.end());
}
bool MeridorConsoleLib::Text::Internal::ChooseText::Valid()
{
	return choose_desc->value.size() <= choose_desc->maximum_size;
}
void MeridorConsoleLib::Text::Internal::ChooseText::ShowInterface()
{
	SetColor(window_info->output_handle, window_info->main_color);
	SetConsoleCursorPosition(window_info->output_handle, GetPosition());
	std::cout << choose_desc->value + ' ';
}
bool MeridorConsoleLib::Text::Internal::ChooseText::ProcessInput(int button)
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
void MeridorConsoleLib::Text::Internal::ChooseText::UpdateInterface()
{
	ShowInterface();
}
void MeridorConsoleLib::Text::Internal::ChooseText::Clear()
{
	SetConsoleCursorPosition(window_info->output_handle, GetPosition());
	std::cout << Spaces(choose_desc->value.size());
}
std::string MeridorConsoleLib::Text::ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info)
{
	Internal::ChooseText choose_impl(choose_desc, window_info);
	return Internal::GenericChoose(choose_impl);
}
std::string MeridorConsoleLib::Text::ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data)
{
	Internal::ChooseText choose_impl(choose_desc, window_info);
	return Internal::GenericChoose(choose_impl, multithreading_data);
}
