#include "ChooseNumeric.h"

int MeridorConsoleLib::Text::Internal::ChooseNumeric::CalculateLenght(long long value)
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

COORD MeridorConsoleLib::Text::Internal::ChooseNumeric::CalculatePosition(int lenght)
{
	const short text_align_shift = static_cast<short>(GetTextAlignScalar(choose_desc->text_align) * static_cast<float>(lenght));
	return {
		static_cast<short>(choose_desc->point_of_reference.X - text_align_shift),
		static_cast<short>(choose_desc->point_of_reference.Y)
	};
}

long long MeridorConsoleLib::Text::Internal::ChooseNumeric::InBounds(const long long value)
{
	return value >= choose_desc->lower_bound && value <= choose_desc->upper_bound;
}

void MeridorConsoleLib::Text::Internal::ChooseNumeric::ClearInterface(int lenght)
{
	SetConsoleCursorPosition(window_info->output_handle, CalculatePosition(lenght));
	std::cout << Spaces(lenght);
}

MeridorConsoleLib::Text::Internal::ChooseNumeric::ChooseNumeric(NumericChooseDesc& choose_desc, const TextInfo& window_info) : ChooseInterface(choose_desc, window_info)
{
	this->choose_desc = &choose_desc;
	lenght = CalculateLenght(choose_desc.value);
	prev_lenght = lenght;
}

bool MeridorConsoleLib::Text::Internal::ChooseNumeric::Valid()
{
	return InBounds(choose_desc->value);
}

void MeridorConsoleLib::Text::Internal::ChooseNumeric::ShowInterface()
{
	SetColor(window_info->output_handle, window_info->main_color);
	SetConsoleCursorPosition(window_info->output_handle, CalculatePosition(lenght));
	if (choose_desc->value)
	{
		std::cout << choose_desc->value;
	}
}

bool MeridorConsoleLib::Text::Internal::ChooseNumeric::ProcessInput(int button)
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

void MeridorConsoleLib::Text::Internal::ChooseNumeric::UpdateInterface()
{
	ClearInterface(prev_lenght);
	ShowInterface();
	prev_lenght = lenght;
}

void MeridorConsoleLib::Text::Internal::ChooseNumeric::Clear()
{
	SetConsoleCursorPosition(window_info->output_handle, CalculatePosition(lenght));
	std::cout << Spaces(lenght);
}

long long MeridorConsoleLib::Text::ChooseNumeric(NumericChooseDesc& choose_desc, const TextInfo& window_info)
{
	Internal::ChooseNumeric choose_impl(choose_desc, window_info);
	return Internal::GenericChoose(choose_impl);
}

long long MeridorConsoleLib::Text::ChooseNumeric(NumericChooseDesc& choose_desc, const TextInfo& window_info, const MultithreadingData& multithreading_data)
{
	Internal::ChooseNumeric choose_impl(choose_desc, window_info);
	return Internal::GenericChoose(choose_impl, multithreading_data);
}
