#include "ValidationFunctions.h"

void ValidationCheck::WindowInfo(MeridorConsoleLib::WindowInfo& window_info, Validation::Status& status)
{
	if (!MeridorConsoleLib::Between(Validation::min_title_size, Validation::max_title_size, static_cast<int>(window_info.title.size())))
	{
		window_info.title = Validation::default_title_name;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (window_info.window_size.X < Validation::minimum_window_size.X)
	{
		window_info.window_size.X = Validation::minimum_window_size.X;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (window_info.window_size.Y < Validation::minimum_window_size.Y)
	{
		window_info.window_size.Y = Validation::minimum_window_size.Y;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (!MeridorConsoleLib::Between(Validation::first_color, Validation::last_color, window_info.main_color))
	{
		window_info.main_color = Validation::default_main_color;
		window_info.secondary_color = Validation::default_secondary_color;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (!MeridorConsoleLib::Between(Validation::first_color, Validation::last_color, window_info.secondary_color))
	{
		window_info.main_color = Validation::default_main_color;
		window_info.secondary_color = Validation::default_secondary_color;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (window_info.secondary_color == window_info.main_color)
	{
		window_info.main_color = Validation::default_main_color;
		window_info.secondary_color = Validation::default_secondary_color;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (window_info.window_mode > MeridorConsoleLib::WindowMode::last)
	{
		window_info.window_mode = MeridorConsoleLib::WindowMode::fullscreen;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
}
void ValidationCheck::Music(float& music_volume, Validation::Status& status)
{
	if (!MeridorConsoleLib::Between(0.0f, 1.0f, music_volume))
	{
		music_volume = 0.0f;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
}
void ValidationCheck::AIModule(std::string& ai_module, Validation::Status& status)
{
	std::vector<std::string> ai_modules = MeridorConsoleLib::GetFilesInDirectory(FolderName::ai);
	for (size_t i = 0; i < ai_modules.size(); ++i)
	{
		if (ai_modules[i] == ai_module)
		{
			return;
		}
	}
	ai_module = ai_modules[0];
	status.SetFlag(Validation::Status::Flags::repaired);
}
void ValidationCheck::ThemeName(std::string& theme_name, Validation::Status& status)
{
	std::string temp = "";
	std::ifstream ifvar;
	ifvar.open(FolderName::main + '\\' + FileName::title_theme);
	if (theme_name == "")
	{
		theme_name = Validation::default_theme_name;
	}
	while (std::getline(ifvar, temp))
	{
		if (MeridorConsoleLib::GetSeparatedValue(temp, 0, '\t') == theme_name)
		{
			ifvar.close();
			return;
		}
	}
	ifvar.close();
	status.SetFlag(Validation::Status::Flags::repaired);
	theme_name = MeridorConsoleLib::GetSeparatedValue(temp, 0, '\t');

	if (temp == "")
	{
		std::ofstream ofvar;
		ofvar.open(FolderName::main + '\\' + FileName::title_theme);
		ofvar << Validation::default_title_theme_file_content;
		ofvar.close();

		theme_name = Validation::default_theme_name;
	}
}
void ValidationCheck::Language(std::string& lang, Validation::Status& status)
{
	std::vector<std::string> languages = MeridorConsoleLib::GetFilesInDirectory(FolderName::language);
	for (size_t i = 0; i < languages.size(); ++i)
	{
		if (languages[i] == lang)
		{
			return;
		}
	}
	lang = languages[0];
	MessageBox(0, (lang + ErrorMsg::placeholder_language).c_str(), ErrorTitle::placeholder_language, MB_TOPMOST);
}
void ValidationCheck::ToTWindowConfig(::ToTWindowConfig& window_config, Validation::Status& status)
{
	WindowInfo(window_config.window_info, status);
	ThemeName(window_config.theme_name, status);
	Language(window_config.lang, status);
	if (!(status.IsFlagActive(Validation::Status::Flags::unplayable)))
	{
		AIModule(window_config.ai_module, status);
	}
	if (!(status.IsFlagActive(Validation::Status::Flags::no_music)))
	{
		Music(window_config.music_volume, status);
	}
	if (status.IsFlagActive(Validation::Status::Flags::repaired))
	{
		FileManagement::SaveWindowConfig(window_config);
		status.UnsetFlag(Validation::Status::Flags::repaired);
		MessageBox(0, ErrorMsg::repaired, ErrorTitle::repaired, MB_TOPMOST);
	}
}
void ValidationCheck::ToTGameConfig(::ToTGameConfig& game_config, Validation::Status& status)
{
	bool name_valid = true;
	bool space = true;
	for (size_t i = 0; i < game_config.name.size(); ++i)
	{
		if (game_config.name[i] == '_')
		{
			game_config.name[i] = ' ';
			if (space)
			{
				name_valid = false;
				break;
			}
		}
		if (!(MeridorConsoleLib::Between('a', 'z', game_config.name[i]) || MeridorConsoleLib::Between('A', 'Z', game_config.name[i])))
		{
			name_valid = false;
			space = false;
			break;
		}
	}
	if (!MeridorConsoleLib::Between(0, GameConstants::max_ais, game_config.number_of_ais))
	{
		game_config.number_of_ais = GameConstants::max_ais;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (!MeridorConsoleLib::Between(1, Validation::maximum_name_length, static_cast<int>(game_config.name.size())) || !name_valid)
	{
		game_config.name = GameConstants::default_racer_name;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (!MeridorConsoleLib::Between(0, Validation::maximum_timer, game_config.timer_settings))
	{
		game_config.timer_settings = 0;
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (status.IsFlagActive(Validation::Status::Flags::repaired))
	{
		FileManagement::SaveGameConfig(game_config);
		status.UnsetFlag(Validation::Status::Flags::repaired);
		MessageBox(0, ErrorMsg::repaired, ErrorTitle::repaired, MB_TOPMOST);
	}
}

void ValidationCheck::FileIntegrity(Validation::Status& status)
{
	FileIntegrity::MainDirectory(status);
	FileIntegrity::MiscFiles(status);
	if (!(status.IsFlagActive(Validation::Status::Flags::unplayable)))
	{
		FileIntegrity::GameFiles(status);
	}
}
void ValidationCheck::FileIntegrity::MainDirectory(Validation::Status& status)
{
	std::vector<std::string> files = MeridorConsoleLib::GetFilesInDirectory(FolderName::main);
	std::vector<std::string> required_files = { FileName::window_config, FileName::game_config, FileName::title_theme, FileName::music };

	for (size_t i = 0; i < files.size(); ++i)
	{
		for (size_t j = 0; j < required_files.size(); ++j)
		{
			if (files[i] == required_files[j])
			{
				required_files.erase(required_files.begin() + j);
				break;
			}
		}
	}
	if (required_files.size() > 0)
	{
		for (size_t i = 0; i < required_files.size(); ++i)
		{
			if (required_files[i] == FileName::music)
			{
				MessageBox(0, ErrorMsg::no_music, ErrorTitle::missing_file, MB_TOPMOST);
				status.SetFlag(Validation::Status::Flags::no_music);
				continue;
			}
			std::ofstream ofvar;
			ofvar.open(FolderName::main + '\\' + required_files[i]);
			if (required_files[i] == FileName::title_theme)
			{
				MessageBox(0, ErrorMsg::no_title_theme, ErrorTitle::missing_file, MB_TOPMOST);
				ofvar << Validation::default_title_theme_file_content;
			}
			else if (required_files[i] == FileName::game_config)
			{
				MessageBox(0, ErrorMsg::no_game_config, ErrorTitle::missing_file, MB_TOPMOST);
				ofvar << Validation::default_game_config_file_content;
			}
			else if (required_files[i] == FileName::window_config)
			{
				MessageBox(0, ErrorMsg::no_window_config, ErrorTitle::missing_file, MB_TOPMOST);
			}
			ofvar.close();
		}
	}
}
void ValidationCheck::FileIntegrity::GameFiles(Validation::Status& status)
{
	std::array<std::string, 4> file_names = { FolderName::tour, FolderName::tire, FolderName::car, FolderName::ai };
	std::array<std::string, 4> file_ext = { ExtName::tour, ExtName::tire, ExtName::car, ExtName::ai };
	std::vector<std::string> files;

	for (int i = static_cast<int>(file_names.size()) - 1; i >= 0; --i)
	{
		files = MeridorConsoleLib::GetFilesInDirectory(file_names[i]);
		for (size_t j = 0; j < files.size(); ++j)
		{
			if (MeridorConsoleLib::GetExtension(files[j]) != file_ext[i])
			{
				InvalidGameFile(file_names[i], files[j]);
				files.erase(files.begin() + i);
			}
		}
		if (files.size() == 0)
		{
			status.SetFlag(Validation::Status::Flags::unplayable);
			MessageBox(0, ErrorMsg::no_game_files, ErrorTitle::missing_file, MB_TOPMOST);
			return;
		}
	}
}
void ValidationCheck::FileIntegrity::MiscFiles(Validation::Status& status)
{
	std::array<std::string, 2> file_names = { FolderName::language, FolderName::ranking };
	std::array<std::string, 2> file_ext = { ExtName::language, ExtName::ranking };
	std::vector<std::string> files;

	for (int i = static_cast<int>(file_names.size()) - 1; i >= 0; --i)
	{
		files = MeridorConsoleLib::GetFilesInDirectory(file_names[i]);
		for (size_t j = 0; j < files.size(); ++j)
		{
			if (MeridorConsoleLib::GetExtension(files[j]) != file_ext[i])
			{
				InvalidGameFile(file_names[i], files[j]);
				files.erase(files.begin() + i);
			}
		}
		if (files.size() == 0)
		{
			if (i == 0)
			{
				MessageBox(0, ErrorMsg::no_lang_pack, ErrorTitle::missing_file, MB_TOPMOST);
				status.SetFlag(Validation::Status::Flags::corrupted);
			}
			else
			{
				MessageBox(0, ErrorMsg::no_ranking, ErrorTitle::missing_file, MB_TOPMOST);
				status.SetFlag(Validation::Status::Flags::no_ranking);
			}
		}
	}
}
void ValidationCheck::TitleThemes(Validation::Status& status)
{
	std::ifstream ifvar;
	std::string temp;
	bool good_files = false;
	std::vector<std::string> title_theme_contents;
	const std::string path = FolderName::main + '\\' + FileName::title_theme;

	ifvar.open(path);
	while (std::getline(ifvar, temp))
	{
		TitleTheme title_theme = FileManagement::GetTitleThemeFromString(temp);
		bool good = true;

		if (!MeridorConsoleLib::Between(1, Validation::maximum_name_length, static_cast<int>(title_theme.name.size())))
		{
			title_theme.name.resize(Validation::maximum_name_length);
			good = false;
		}
		if (title_theme.main_left.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.main_left.resize(Validation::maximum_title_decoration_size);
			good = false;
		}
		if (title_theme.main_right.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.main_right.resize(Validation::maximum_title_decoration_size);
			good = false;
		}
		if (title_theme.secondary_left.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.secondary_left.resize(Validation::maximum_title_decoration_size);
			good = false;
		}
		if (title_theme.secondary_right.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.secondary_right.resize(Validation::maximum_title_decoration_size);
			good = false;
		}
		if (!MeridorConsoleLib::Between(Validation::minimum_title_decoration_distance, Validation::maximum_title_decoration_distance, title_theme.decoration_distance))
		{
			title_theme.decoration_distance = Validation::minimum_title_decoration_distance;
			good = false;
		}
		if (!good)
		{
			temp = FileManagement::GetStringFromTitleTheme(title_theme);
			status.SetFlag(Validation::Status::Flags::repaired);
		}
		title_theme_contents.push_back(temp);
	}
	ifvar.close();
	if (title_theme_contents.size() < 1)
	{
		title_theme_contents.push_back(Validation::default_title_theme_file_content);
		status.SetFlag(Validation::Status::Flags::repaired);
	}
	if (status.IsFlagActive(Validation::Status::Flags::repaired))
	{
		std::ofstream ofvar;
		ofvar.open(path, std::ios::trunc);
		for (size_t i = 0; i < title_theme_contents.size() - 1; ++i)
		{
			ofvar << title_theme_contents[i] << '\n';
		}
		ofvar << title_theme_contents[title_theme_contents.size() - 1];
		ofvar.close();
		status.UnsetFlag(Validation::Status::Flags::repaired);
		MessageBox(0, ErrorMsg::repaired, ErrorTitle::repaired, MB_TOPMOST);
	}

}
void ValidationCheck::Rankings(Validation::Status& status)
{
	std::vector<std::string> ranking_files = MeridorConsoleLib::GetFilesInDirectory(FolderName::ranking);
	std::ifstream fvar;
	bool good_files = false;

	for (size_t i = 0; i < ranking_files.size(); ++i)
	{
		fvar.open(FolderName::ranking + '\\' + ranking_files[i]);
		std::string line;
		int iterations = 0;
		for (; std::getline(fvar, line); ++iterations);
		if (iterations % Validation::ranking_details != 0)
		{
			InvalidGameFile(FolderName::ranking, ranking_files[i]);
		}
		else
		{
			good_files = true;
		}
		fvar.close();
	}
	if (!good_files)
	{
		status.SetFlag(Validation::Status::Flags::no_ranking);
		MessageBox(0, ErrorMsg::no_ranking, ErrorTitle::missing_file, MB_TOPMOST);
	}
}
void ValidationCheck::LanguagePacks(Validation::Status& status)
{
	std::vector<std::string> lang_files = MeridorConsoleLib::GetFilesInDirectory(FolderName::language);
	bool good_files = false;

	for (size_t i = 0; i < lang_files.size(); ++i)
	{
		FileManagement::LoadLanguagePack(FolderName::language + '\\' + lang_files[i]);
		if (
			(static_cast<int>(LanguagePack::text.size()) != LanguagePack::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::car_attributes].size()) != CarAttributes::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::terrain_types].size()) != TerrainTypes::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::main_menu].size()) != Validation::main_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::game_options].size()) != Validation::game_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::game_options_multiplayer].size()) != Validation::multiplayer_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_menu].size()) != Validation::multiplayer_before_game_lobby) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::general_options].size()) != Validation::tot_options) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::tour_information].size()) != GameLobbyInformations::last) ||
			(static_cast<MeridorConsoleLib::Color>(LanguagePack::text[LanguagePack::selectable_colors].size()) != Validation::colors) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::race_actions].size()) != Validation::actions) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::race_chancebox].size()) != Validation::chancebox) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::race_player_informations].size()) != Validation::general_info) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::race_leaderboard].size()) != Validation::leaderboard) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::race_actions_descritions].size()) != Validation::actions) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::race_distances].size()) != Validation::distances) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::race_box_names].size()) != Validation::names_of_boxes) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::tour_segments_descriptions].size()) != TerrainTypes::last * 2) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::accident_effects].size()) != Validation::accidents) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::on_off].size()) != 2) ||
			(static_cast<MeridorConsoleLib::WindowMode>(LanguagePack::text[LanguagePack::display_settings].size()) != MeridorConsoleLib::WindowMode::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::ranking_menu].size()) != Validation::ranking_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::ranking_classification_types].size()) != Validation::ranking_classification) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::ranking_details].size()) != Validation::ranking_details) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::other_strings].size()) != OtherStrings::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_client_lobby].size()) < Multiplayer::last))
		{
			InvalidGameFile(FolderName::language, lang_files[i]);
		}
		else
		{
			good_files = true;
		}
	}
	if (!good_files)
	{
		status.SetFlag(Validation::Status::Flags::corrupted);
		MessageBox(0, ErrorMsg::no_lang_pack, ErrorTitle::missing_file, MB_TOPMOST);
	}
}
void ValidationCheck::GameFiles(Validation::Status& status)
{
	if (!(status.IsFlagActive(Validation::Status::Flags::unplayable)))
	{
		Tires(status);
		Cars(status);
		Tours(status);
		if (status.IsFlagActive(Validation::Status::Flags::unplayable))
		{
			MessageBox(0, ErrorMsg::no_game_files, ErrorTitle::missing_file, MB_TOPMOST);
		}
	}
}
void ValidationCheck::InvalidGameFile(const std::string& directory, const std::string& file_name)
{
	int decision = MessageBox(0, (directory + '\\' + file_name + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file, MB_TOPMOST | MB_YESNO);
	if (decision == IDYES)
	{
		remove((directory + '\\' + file_name).c_str());
	}
	else
	{
		if (std::rename((directory + '\\' + file_name).c_str(), (FolderName::quarantine + '\\' + file_name).c_str()))
		{
			remove((directory + '\\' + file_name).c_str());
			MessageBox(0, ErrorMsg::coudnt_move_man, ErrorTitle::coudnt_move_man, MB_TOPMOST);
		}
	}
}
void ValidationCheck::Tours(Validation::Status& status)
{
	std::vector<std::string> tours = MeridorConsoleLib::GetFilesInDirectory(FolderName::tour);

	int good_files = false;
	for (size_t i = 0; i < tours.size(); ++i)
	{
		//do all cars for race exist
		std::vector<std::string> cars_for_race = FileManagement::GetCarNames(tours[i]);
		MeridorConsoleLib::AddExtension(cars_for_race, ExtName::car);
		std::vector<std::string> car_names = MeridorConsoleLib::GetFilesInDirectory(FolderName::car);
		if (car_names.size() == 0)
		{
			InvalidGameFile(FolderName::tour, tours[i]);
			continue;
		}
		for (size_t j = 0; j < car_names.size(); ++j)
		{
			for (size_t k = 0; k < cars_for_race.size(); ++k)
			{
				if (cars_for_race[k] == car_names[j])
				{
					cars_for_race.erase(cars_for_race.begin() + k);
					break;
				}
			}
		}
		if (cars_for_race.size() > 0)
		{
			InvalidGameFile(FolderName::tour, tours[i]);
			continue;
		}

		//are params valid
		std::vector<std::string> params = FileManagement::GetTourParameters(tours[i], 0, INT_MAX);
		if (static_cast<short>(params.size()) < Validation::minimum_tour_lenght)
		{
			InvalidGameFile(FolderName::tour, tours[i]);
			continue;
		}
		bool good = true;
		for (size_t j = 0; j < params.size(); ++j)
		{
			const short segment_lenght = static_cast<short>(params[j].size());
			if ((!MeridorConsoleLib::Between(0, TerrainTypes::last - 1, params[j][0] - '0')) ||		//invalid terrain type
				(segment_lenght > Validation::digits_of_max_speed) ||								//safe speed exceeding allowed values
				(segment_lenght > 1 && atoi(params[j].substr(1, segment_lenght - 1).c_str()) < 1))	//safe speed less or equal to 0
			{
				good = false;
				InvalidGameFile(FolderName::tour, tours[i]);
				break;
			}
		}
		if (good)
		{
			good_files = true;
		}
	}
	if (!good_files)
	{
		status.SetFlag(Validation::Status::Flags::unplayable);
	}
}
void ValidationCheck::Cars(Validation::Status& status)
{
	const std::vector<std::string> cars = MeridorConsoleLib::GetFilesInDirectory(FolderName::car);
	bool good_files = false;

	for (size_t i = 0; i < cars.size(); ++i)
	{
		const std::vector<std::string> car_data = MeridorConsoleLib::ReadFile(FolderName::car + '\\' + cars[i]);

		if (car_data.size() != CarAttributes::last)//checking if car has all parameters set
		{
			InvalidGameFile(FolderName::car, cars[i]);
			continue;
		}
		for (size_t j = 0; j < CarAttributes::last; ++j)
		{
			if ((!MeridorConsoleLib::Between(1, Validation::digits_of_max_speed, static_cast<int>(car_data[j].size()))) //too large param values 
				|| (j == CarAttributes::visibility && (!MeridorConsoleLib::Between(1, Validation::max_visibility, atoi(car_data[j].c_str()))))) //wrong visibility
			{
				InvalidGameFile(FolderName::car, cars[i]);
				break;
			}
		}
		good_files = true;
	}
	if (!good_files)
	{
		status.SetFlag(Validation::Status::Flags::unplayable);
	}
}
void ValidationCheck::Tires(Validation::Status& status)
{
	const std::vector<std::string> tires = MeridorConsoleLib::GetFilesInDirectory(FolderName::tire);
	bool good_files = false;
	int x = 0;
	int y = 0;

	for (size_t i = 0; i < tires.size(); ++i)
	{
		const std::vector<std::string> params = FileManagement::GetTireParameters(tires[i]);
		if (static_cast<short>(params.size()) != TerrainTypes::last)//checking if tires have all parameters set
		{
			InvalidGameFile(FolderName::tire, tires[i]);
			continue;
		}
		for (short j = 0; j < TerrainTypes::last; ++j)//getting tires attributes
		{
			short tire_param_size = static_cast<short>(params[j].size());
			bool good = false;
			for (short k = 0; k < tire_param_size; ++k)
			{
				if (params[j][k] == 'x')
				{
					x = atoi(params[j].substr(0, k).c_str());
					y = atoi(params[j].substr(k + 1, tire_param_size - k - 1).c_str());

					if (x * y != 0 && MeridorConsoleLib::Between(0, y, x) && MeridorConsoleLib::Between(0, Validation::max_number_of_tests, y))
					{
						good = true;
					}
					break;
				}
			}
			if (!good)
			{
				InvalidGameFile(FolderName::tire, tires[i]);
				break;
			}
			else if (j == TerrainTypes::last - 1)
			{
				good_files = true;
			}
		}
	}
	if (!good_files)
	{
		status.SetFlag(Validation::Status::Flags::unplayable);
	}
}

