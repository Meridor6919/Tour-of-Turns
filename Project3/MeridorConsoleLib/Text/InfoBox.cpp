#include "Infobox.h"

MeridorConsoleLib::InfoBox::InfoBox(const InfoBoxDesc& info_box_desc, const TextInfo& window_info, const MultithreadingData *multithreading_data)
{
	this->info_box_desc = info_box_desc;
	this->window_info = window_info;
	this->multithreading_data = multithreading_data;

	box_width = static_cast<unsigned short>(info_box_desc.box_size.right - info_box_desc.box_size.left);
	box_height = static_cast<unsigned short>(info_box_desc.box_size.bottom - info_box_desc.box_size.top);
}
void MeridorConsoleLib::InfoBox::SetCursorPosition(short index)
{
	COORD local_position;

	local_position.X = static_cast<short>(info_box_desc.box_size.left + static_cast<short>(static_cast<float>(box_width) * GetTextAlignScalar(info_box_desc.text_align)));
	local_position.Y = static_cast<short>(info_box_desc.box_size.top + upper_border_height + index * info_box_desc.vertical_spacing);
	SetConsoleCursorPosition(window_info.output_handle, local_position);
}
std::string MeridorConsoleLib::InfoBox::RegulateStringSize(const std::string& text, short max_size, bool fill_to_max_size)
{
	std::string ret_value = text;
	if (ret_value.size() > max_size)
	{
		for (short i = max_size - 1; i >= max_size - 3; --i)
		{
			ret_value[i] = '.';
		}
		ret_value.resize(max_size, ' ');
	}
	else
	{
		if (fill_to_max_size)
		{
			ret_value.resize(max_size, ' ');
		}
	}
	return ret_value;
}
void MeridorConsoleLib::InfoBox::ShowLineOfText(unsigned short index, Color primary_color, Color secondary_color)
{
	const unsigned short data_size = static_cast<unsigned short>(data.size());
	unsigned short first_part = static_cast<unsigned short>(data[index].first.size());
	unsigned short second_part = static_cast<unsigned short>(data[index].second.size());

	std::string highlighted_text = RegulateStringSize(data[index].first, box_width);
	std::string ordinary_text = RegulateStringSize(data[index].second, box_width - static_cast<short>(highlighted_text.size()) - 1, true);

	SetCursorPosition(data_size - index - 1);
	SetColor(window_info.output_handle, secondary_color);
	std::cout << highlighted_text << ' ';
	SetColor(window_info.output_handle, primary_color);
	std::cout << ordinary_text;

}
void MeridorConsoleLib::InfoBox::Push(const std::string secondary_color_text, const std::string main_color_text)
{
	if (data.size() == info_box_desc.history_size)
	{
		data.erase(data.begin());
	}
	data.push_back(std::make_pair(secondary_color_text, main_color_text));
}
void MeridorConsoleLib::InfoBox::Draw()
{
	const unsigned short container_size = static_cast<unsigned short>(data.size());

	unsigned short max_records_to_display = (box_height - upper_border_height) / info_box_desc.vertical_spacing;
	Color main_color = window_info.main_color;
	Color secondary_color = window_info.secondary_color;

	if (multithreading_data != nullptr)
	{
		multithreading_data->mutex->lock();
	}
	for (unsigned short index = container_size; index > 0 && index + max_records_to_display > container_size;)
	{
		--index;
		ShowLineOfText(index, main_color, secondary_color);
		main_color = MeridorConsoleLib::Color::dark_gray;
		secondary_color = MeridorConsoleLib::Color::dark_gray;
	}
	if (multithreading_data != nullptr)
	{
		multithreading_data->mutex->unlock();
	}
}
void MeridorConsoleLib::InfoBox::DrawBox(bool clear_instead)
{
	const std::string border = GetMonoCharacterString(box_width, clear_instead ? ' ' : '_');
	COORD local_position = {
		static_cast<short>(info_box_desc.box_size.left) + static_cast<short>(static_cast<float>(box_width) * GetTextAlignScalar(info_box_desc.text_align)),
		static_cast<short>(info_box_desc.box_size.top)
	};

	if (multithreading_data != nullptr)
	{
		multithreading_data->mutex->lock();
	}
	//Top border
	SetColor(window_info.output_handle, window_info.secondary_color);
	SetConsoleCursorPosition(window_info.output_handle, local_position);
	std::cout << border;
	//Bottom border
	local_position.Y += box_height;
	SetConsoleCursorPosition(window_info.output_handle, local_position);
	std::cout << border;
	if (multithreading_data != nullptr)
	{
		multithreading_data->mutex->unlock();
	}
}
void MeridorConsoleLib::InfoBox::Clear()
{
	const std::string empty_text = GetMonoCharacterString(box_width, ' ');
	const short x_pos = static_cast<short>(info_box_desc.box_size.left);

	if (multithreading_data != nullptr)
	{
		multithreading_data->mutex->lock();
	}
	for (short i = static_cast<short>(info_box_desc.box_size.top); i < static_cast<short>(info_box_desc.box_size.bottom); ++i)
	{
		SetConsoleCursorPosition(window_info.output_handle, { x_pos, i });
		std::cout << empty_text;
	}
	if (multithreading_data != nullptr)
	{
		multithreading_data->mutex->unlock();
	}
}
void MeridorConsoleLib::InfoBox::Reset()
{
	data.clear();
}
