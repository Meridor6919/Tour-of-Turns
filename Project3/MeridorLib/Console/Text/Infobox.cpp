#include "Infobox.h"

namespace MeridorConsoleLib
{
	namespace Text
	{

		COORD Infobox::GetPositionAbsolute(size_t lenght, short y_value)
		{
			const short text_align_shift = static_cast<short>(GetTextAlignScalar(infobox_desc->text_align) * static_cast<float>(lenght));
			return {
				static_cast<short>(infobox_desc->position.X - text_align_shift),
				static_cast<short>(infobox_desc->position.Y + y_value)
			};
		}
		short Infobox::GetYPositionFromIndex(short index)
		{
			return static_cast<short>(infobox_desc->box_size.Y - 1 - infobox_desc->vertical_spacing * index);
		}
		int Infobox::GetMaxRecords()
		{
			return (infobox_desc->box_size.Y - 1) / infobox_desc->vertical_spacing;
		}
		void Infobox::CorrectContainerSize()
		{
			if (infobox_desc->data.size() + 1 >= GetMaxRecords())
			{
				infobox_desc->data.erase(infobox_desc->data.begin());
			}
		}
		void Infobox::CorrectTextSize(std::string& text, short& remaining_space)
		{
			const size_t text_to_long_size = infobox_desc->text_to_long_sign.size();
			if (text.size() > remaining_space)
			{
				if (remaining_space > text_to_long_size)
				{
					text.resize(remaining_space - text_to_long_size);
					text += infobox_desc->text_to_long_sign;
				}
				else
				{
					text.resize(remaining_space);
				}
			}
			remaining_space -= static_cast<short>(text.size() + infobox_desc->horizontal_spacing);
		}
		void Infobox::ShowDataRecord(Color main_color, Color secondary_color, short index)
		{
			COORD pos = GetPositionAbsolute(
				infobox_desc->data[index].first.size() + infobox_desc->data[index].second.size() + infobox_desc->horizontal_spacing,
				GetYPositionFromIndex(static_cast<short>(index - infobox_desc->data.size() + GetMaxRecords()))
			);
			SetConsoleCursorPosition(window_info->output_handle, { infobox_desc->box_size.X, pos.Y });
			std::cout << GetMonoCharacterString(infobox_desc->box_size.X, ' ');

			SetConsoleCursorPosition(window_info->output_handle, pos);
			SetColor(window_info->output_handle, secondary_color);
			std::cout << infobox_desc->data[index].first;
			SetColor(window_info->output_handle, main_color);
			std::cout << Spaces(infobox_desc->horizontal_spacing) + infobox_desc->data[index].second;
		}
		Infobox::Infobox(InfoboxDesc& infobox_desc, const MeridorConsoleLib::Text::WindowInfo& window_info)
		{
			this->infobox_desc = &infobox_desc;
			this->window_info = &window_info;
		}
		void Infobox::Push(std::string secondary_color_text, std::string main_color_text)
		{
			CorrectContainerSize();

			short remaining_space = infobox_desc->box_size.X;
			CorrectTextSize(secondary_color_text, remaining_space);
			CorrectTextSize(main_color_text, remaining_space);

			Text::MyPair<std::string, std::string> text = { secondary_color_text, main_color_text };
			infobox_desc->data.emplace_back(std::move(text));
		}
		void Infobox::Draw()
		{
			Color current_main = window_info->main_color;
			Color current_secondary = window_info->secondary_color;

			for (int i = static_cast<short>(infobox_desc->data.size() - 1); i >= 0; --i)
			{
				ShowDataRecord(current_main, current_secondary, i);
				current_main = infobox_desc->fading_color;
				current_secondary = infobox_desc->fading_color;
			}
		}
		void Infobox::DrawBox()
		{
			const std::string border = GetMonoCharacterString(infobox_desc->box_size.X, infobox_desc->border_building_character);

			SetConsoleCursorPosition(window_info->output_handle, GetPositionAbsolute(infobox_desc->box_size.X, 0));
			SetColor(window_info->output_handle, window_info->secondary_color);
			std::cout << border;
			SetConsoleCursorPosition(window_info->output_handle, GetPositionAbsolute(infobox_desc->box_size.X, infobox_desc->box_size.Y - 1));
			std::cout << border;
			Draw();
		}
		void Infobox::ClearBox()
		{
			const std::string spaces = Spaces(infobox_desc->box_size.X);
			COORD position{};

			for (int i = 0; i < infobox_desc->box_size.Y; ++i)
			{
				position = GetPositionAbsolute(infobox_desc->box_size.X, i);
				SetConsoleCursorPosition(window_info->output_handle, position);
				std::cout << spaces;
			}
			++position.Y;
			std::cout << spaces;
		}
		void Infobox::Draw(const MultithreadingData& multithreading_data)
		{
			multithreading_data.mutex->lock();
			Draw();
			multithreading_data.mutex->unlock();
		}
		void Infobox::DrawBox(const MultithreadingData& multithreading_data)
		{
			multithreading_data.mutex->lock();
			DrawBox();
			multithreading_data.mutex->unlock();
		}
		void Infobox::ClearBox(const MultithreadingData& multithreading_data)
		{
			multithreading_data.mutex->lock();
			ClearBox();
			multithreading_data.mutex->unlock();
		}
		void Infobox::Reset()
		{
			infobox_desc->data.clear();
		}
	}
}