#include "Title.h"
#include "ToT_Window.h"

void Title::Init(ToT_Window* main_window)
{
	this->main_window = main_window;
}

void Title::SetTheme(std::string name)
{
	theme = LoadTitleTheme(name);
	SanitizeTitleTheme(theme);
}

const TitleTheme& Title::GetTheme()
{
	return theme;
}

void Title::ShowMainPart()
{
	const COORD starting_point = { static_cast<short>(main_window->GetCharactersPerRow()) / 2, 1 };
	const COORD orientation_point = { starting_point.X - static_cast<short>(static_cast<float>(TextAlign::center) / 2.0f * LanguagePack::text[LanguagePack::title_main][0].size()), starting_point.Y };
	const short main_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main].size());
	const short additional_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional].size());
	const HANDLE handle = main_window->GetHandle();

	//Main text
	SetConsoleTextAttribute(handle, *main_window->main_color);
	for (short i = 0; i < main_title_size; ++i)
	{
		SetConsoleCursorPosition(handle, { orientation_point.X, orientation_point.Y + i });
		std::cout << LanguagePack::text[LanguagePack::title_main][i];
	}
	SetConsoleTextAttribute(handle, *main_window->secondary_color);
	for (short i = 0; i < additional_title_size; ++i)
	{
		const short main_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		const short additional_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional][i].size());
		SetConsoleCursorPosition(handle, { orientation_point.X + main_line_size / 2 - additional_line_size / 2, orientation_point.Y + i + main_title_size / 3 });
		std::cout << LanguagePack::text[LanguagePack::title_additional][i];
	}
}

void Title::ShowDecoration(bool clear)
{
	//Decoration constants definition
	const short main_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main].size());
	const COORD starting_point = { static_cast<short>(main_window->GetCharactersPerRow()) / 2, 1 };
	const COORD orientation_point = { starting_point.X - static_cast<short>(static_cast<float>(TextAlign::center) / 2.0f * LanguagePack::text[LanguagePack::title_main][0].size()), starting_point.Y };
	const short main_decoration_left_size = static_cast<short>(theme.main_left.size());
	const short secondary_decoration_left_size = static_cast<short>(theme.secondary_left.size());
	const short decoration_left_size = theme.decoration_distance + max(theme.secondary_pos_left + secondary_decoration_left_size - 1, main_decoration_left_size);
	const HANDLE handle = main_window->GetHandle();

	const std::string main_left = clear ? Spaces(theme.main_left.size()) : theme.main_left;
	const std::string main_right = clear ? Spaces(theme.main_right.size()) : theme.main_right;
	const std::string secondary_left = clear ? Spaces(theme.secondary_left.size()) : theme.secondary_left;
	const std::string secondary_right = clear ? Spaces(theme.secondary_right.size()) : theme.secondary_right;

	//Decoration algorithm
	for (short i = 0; i < main_title_size; ++i)
	{
		const short wobbling = (theme.decoration_wobble ? i % 2 : 0);
		const short line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		SetConsoleTextAttribute(handle, *main_window->main_color);
		SetConsoleCursorPosition(handle, { orientation_point.X - decoration_left_size - wobbling, orientation_point.Y + i });
		std::cout << main_left;
		SetConsoleCursorPosition(handle, { orientation_point.X + line_size + theme.decoration_distance + wobbling - 1, orientation_point.Y + i });
		std::cout << main_right;

		SetConsoleTextAttribute(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { orientation_point.X - decoration_left_size - wobbling + theme.secondary_pos_left, orientation_point.Y + i });
		std::cout << secondary_left;
		SetConsoleCursorPosition(handle, { orientation_point.X + line_size + theme.decoration_distance + wobbling - 1 + theme.secondary_pos_right, orientation_point.Y + i });
		std::cout << secondary_right;
	}
}
