#include "Infobox.h"

COORD MeridorConsoleLib::Text::Infobox::GetPositionAbsolute(size_t lenght, short y_value)
{
	const short text_align_shift = static_cast<short>(GetTextAlignScalar(infobox_desc->text_align) * static_cast<float>(lenght));
	return {
		static_cast<short>(infobox_desc->position.X - text_align_shift),
		static_cast<short>(infobox_desc->position.Y + y_value)
	};
}

short MeridorConsoleLib::Text::Infobox::GetYPositionFromIndex(short index)
{
	return static_cast<short>(infobox_desc->box_size.Y - 1 - infobox_desc->vertical_spacing * index);
}

int MeridorConsoleLib::Text::Infobox::GetPossibleSlots()
{
	return (infobox_desc->box_size.Y - 1) / infobox_desc->vertical_spacing;
}

MeridorConsoleLib::Text::Infobox::Infobox(InfoboxDesc& infobox_desc, const MeridorConsoleLib::Text::TextInfo& window_info, const MultithreadingData& multithreading_data)
{
	this->infobox_desc = &infobox_desc;
	this->window_info = &window_info;
	this->multithreading_data = &multithreading_data;
}

MeridorConsoleLib::Text::Infobox::Infobox(InfoboxDesc& infobox_desc, const MeridorConsoleLib::Text::TextInfo& window_info)
{
	this->infobox_desc = &infobox_desc;
	this->window_info = &window_info;
}

void MeridorConsoleLib::Text::Infobox::Push(std::string secondary_color_text, std::string main_color_text)
{
	constexpr const char* dots = "...";
	constexpr size_t dots_size = 3;

	if (infobox_desc->data.size() + 1 >= GetPossibleSlots())
	{
		infobox_desc->data.erase(infobox_desc->data.begin());
	}
	auto size = infobox_desc->box_size.X;
	if (secondary_color_text.size() > size)
	{
		if (size > dots_size)
		{
			secondary_color_text.resize(size - dots_size);
			secondary_color_text += dots;
		}
		else
		{
			secondary_color_text.resize(size);
		}
	}
	size -= secondary_color_text.size() + default_spacing;
	if (main_color_text.size() > size)
	{
		if (size > dots_size)
		{
			main_color_text.resize(size - dots_size);
			main_color_text += dots;
		}
	}
	Text::MyPair<std::string, std::string> text = { secondary_color_text, main_color_text };
	infobox_desc->data.emplace_back(std::move(text));
}

void MeridorConsoleLib::Text::Infobox::Draw()
{
	Color current_main = window_info->main_color;
	Color current_secondary = window_info->secondary_color;
	for (int i = infobox_desc->data.size() - 1; i >= 0; --i)
	{
		auto y_pos_from_index = GetYPositionFromIndex(i - infobox_desc->data.size() + GetPossibleSlots());
		SetConsoleCursorPosition(window_info->output_handle, GetPositionAbsolute(infobox_desc->box_size.X, y_pos_from_index));
		std::cout << GetMonoCharacterString(infobox_desc->box_size.X, ' ');
		SetConsoleCursorPosition(window_info->output_handle, GetPositionAbsolute(infobox_desc->data[i].first.size() + infobox_desc->data[i].second.size() + 1, y_pos_from_index));
		SetColor(window_info->output_handle, current_secondary);
		std::cout << infobox_desc->data[i].first;
		SetColor(window_info->output_handle, current_main);
		std::cout << ' ' + infobox_desc->data[i].second;

		current_main = infobox_desc->fading_color;
		current_secondary = infobox_desc->fading_color;
	}
}

void MeridorConsoleLib::Text::Infobox::DrawBox()
{
	const std::string border = GetMonoCharacterString(infobox_desc->box_size.X, infobox_desc->border_building_character);

	if (multithreading_data)
	{
		multithreading_data->mutex->lock();
	}
	SetConsoleCursorPosition(window_info->output_handle, GetPositionAbsolute(infobox_desc->box_size.X, 0));
	SetColor(window_info->output_handle, window_info->secondary_color);
	std::cout << border;
	SetConsoleCursorPosition(window_info->output_handle, GetPositionAbsolute(infobox_desc->box_size.X, infobox_desc->box_size.Y - 1));
	std::cout << border;
	Draw();
	if (multithreading_data)
	{
		multithreading_data->mutex->unlock();
	}
}

void MeridorConsoleLib::Text::Infobox::ClearBox()
{
	const std::string border = GetMonoCharacterString(infobox_desc->box_size.X, infobox_desc->border_building_character);
	const std::string spaces = Spaces(infobox_desc->box_size.X);
	COORD position{};
	
	multithreading_data->mutex->lock();
	for (int i = 0; i < infobox_desc->box_size.Y; ++i)
	{
		position = GetPositionAbsolute(infobox_desc->box_size.X, i);
		SetConsoleCursorPosition(window_info->output_handle, position);
		std::cout << spaces;
	}
	++position.Y;
	std::cout << spaces;
	multithreading_data->mutex->unlock();
}

void MeridorConsoleLib::Text::Infobox::Reset()
{
	infobox_desc->data.clear();
}
