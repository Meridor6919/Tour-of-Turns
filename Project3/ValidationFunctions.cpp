#include "ValidationFunctions.h"

void ValidationCheck::WindowInfo(MeridorConsoleLib::WindowInfoEx& window_info, char& status)
{
	if (window_info.characters_capacity.Y < Validation::minimum_window_size.Y)
	{
		window_info.characters_capacity.Y = Validation::minimum_window_size.Y;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (window_info.characters_capacity.X < Validation::minimum_window_size.X)
	{
		window_info.characters_capacity.X = Validation::minimum_window_size.X;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (!MeridorConsoleLib::Between(Validation::first_color, Validation::last_color, window_info.main_color))
	{
		window_info.main_color = Validation::default_main_color;
		window_info.secondary_color = Validation::default_secondary_color;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (!MeridorConsoleLib::Between(Validation::first_color, Validation::last_color, window_info.secondary_color))
	{
		window_info.main_color = Validation::default_main_color;
		window_info.secondary_color = Validation::default_secondary_color;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (window_info.secondary_color == window_info.main_color)
	{
		window_info.main_color = Validation::default_main_color;
		window_info.secondary_color = Validation::default_secondary_color;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (window_info.window_mode > MeridorConsoleLib::WindowMode::last)
	{
		window_info.window_mode = MeridorConsoleLib::WindowMode::fullscreen;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
}
void ValidationCheck::Music(float& music_volume, char& status)
{
	if (!MeridorConsoleLib::Between(0.0f, 1.0f, music_volume))
	{
		music_volume = 0.0f;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
}
void ValidationCheck::AIModule(std::string& ai_module, char& status)
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
	if (!(status & Validation::StatusFlags::repaired))
	{
		status = status | Validation::StatusFlags::repaired;
	}
}
void ValidationCheck::ThemeName(std::string& theme_name, char& status)
{
	std::string temp = "";
	std::ifstream ifvar;
	ifvar.open(FolderName::main + '\\' + FileName::title_theme);
	if (theme_name == "")
	{
		theme_name = "Empty";
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
	if (!(status & Validation::StatusFlags::repaired))
	{
		status = status | Validation::StatusFlags::repaired;
	}
	theme_name = temp;

	if (temp == "")
	{
		std::ofstream ofvar;
		ofvar.open(FolderName::main + '\\' + FileName::title_theme);
		ofvar << Validation::default_title_theme_file_content;
		ofvar.close();

		theme_name = "Empty";
		MessageBox(0, ErrorMsg::missing_file, ErrorTitle::ai_error, 0);
	}
}
void ValidationCheck::ToTWindowConfig(::ToTWindowConfig& window_config, char& status)
{
	WindowInfo(window_config.window_info, status);
	ThemeName(window_config.theme_name, status);
	if (!(status & Validation::StatusFlags::unplayable))
	{
		AIModule(window_config.ai_module, status);
	}
	if (!(status & Validation::StatusFlags::no_music))
	{
		Music(window_config.music_volume, status);
	}
}
void ValidationCheck::ToTGameConfig(::ToTGameConfig& game_config, char& status)
{
	bool name_valid = true;
	bool space = true;
	for (int i = 0; i < static_cast<int>(game_config.name.size()); ++i)
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
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (!MeridorConsoleLib::Between(1, Validation::maximum_name_length, static_cast<int>(game_config.name.size())) || !name_valid)
	{
		game_config.name = LanguagePack::text[LanguagePack::other_strings][OtherStrings::default_name];
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (!MeridorConsoleLib::Between(0, Validation::maximum_timer, game_config.timer_settings))
	{
		game_config.timer_settings = 0;
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
}

void ValidationCheck::FileIntegrity(char& status)
{
	FileIntegrity::MainDirectory(status);
	FileIntegrity::MiscFiles(status);
	FileIntegrity::GameFiles(status);
}
void ValidationCheck::FileIntegrity::MainDirectory(char& status)
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
				if (!(status & Validation::StatusFlags::no_music))
				{
					status = status | Validation::StatusFlags::no_music;
				}
				continue;
			}
			std::ofstream ofvar;
			ofvar.open(FolderName::main + '\\' + required_files[i]);
			if (required_files[i] == FileName::title_theme)
			{
				ofvar << Validation::default_title_theme_file_content;
				if (!(status & Validation::StatusFlags::repaired))
				{
					status = status | Validation::StatusFlags::repaired;
				}
			}
			else if (required_files[i] == FileName::game_config)
			{
				ofvar << Validation::default_game_config_file_content;
				if (!(status & Validation::StatusFlags::repaired))
				{
					status = status | Validation::StatusFlags::repaired;
				}
			}
			else if (required_files[i] == FileName::window_config)
			{
				if (!(status & Validation::StatusFlags::unplayable))
				{
					status = status | Validation::StatusFlags::unplayable;
				}
			}
			ofvar.close();
		}
	}
}
void ValidationCheck::FileIntegrity::GameFiles(char& status)
{
	std::vector<std::string> tours = MeridorConsoleLib::GetFilesInDirectory(FolderName::tour);
	std::vector<std::string> tires = MeridorConsoleLib::GetFilesInDirectory(FolderName::tire);
	std::vector<std::string> cars = MeridorConsoleLib::GetFilesInDirectory(FolderName::car);

	if (tours.size() == 0 || tires.size() == 0 || cars.size() == 0)
	{
		if (!(status & Validation::StatusFlags::unplayable))
		{
			status = status | Validation::StatusFlags::unplayable;
		}
	}
}
void ValidationCheck::FileIntegrity::MiscFiles(char& status)
{
	std::vector<std::string> files;
	
	files = MeridorConsoleLib::GetFilesInDirectory(FolderName::language);
	if (files.size() <= 0)
	{
		if (!(status & Validation::StatusFlags::corrupted))
		{
			status = status | Validation::StatusFlags::corrupted;
		}
	}
	files = MeridorConsoleLib::GetFilesInDirectory(FolderName::ranking);
	if (files.size() <= 0)
	{
		if (!(status & Validation::StatusFlags::no_ranking))
		{
			status = status | Validation::StatusFlags::no_ranking;
		}
	}
}

void ValidationCheck::TitleThemes(char& status)
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

		if (title_theme.main_left.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.main_left.resize(Validation::maximum_title_decoration_size);
			if (!(status & Validation::StatusFlags::repaired))
			{
				status = status | Validation::StatusFlags::repaired;
			}
		}
		if (title_theme.main_right.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.main_right.resize(Validation::maximum_title_decoration_size);
			if (!(status & Validation::StatusFlags::repaired))
			{
				status = status | Validation::StatusFlags::repaired;
			}
		}
		if (title_theme.secondary_left.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.secondary_left.resize(Validation::maximum_title_decoration_size);
			if (!(status & Validation::StatusFlags::repaired))
			{
				status = status | Validation::StatusFlags::repaired;
			}
		}
		if (title_theme.secondary_right.size() > Validation::maximum_title_decoration_size)
		{
			title_theme.secondary_right.resize(Validation::maximum_title_decoration_size);
			if (!(status & Validation::StatusFlags::repaired))
			{
				status = status | Validation::StatusFlags::repaired;
			}
		}
		if (!MeridorConsoleLib::Between(Validation::minimum_title_decoration_distance, Validation::maximum_title_decoration_distance, title_theme.decoration_distance))
		{
			title_theme.decoration_distance = Validation::minimum_title_decoration_distance;
			if (!(status & Validation::StatusFlags::repaired))
			{
				status = status | Validation::StatusFlags::repaired;
			}
		}
		if (!(status & Validation::StatusFlags::repaired))
		{
			temp = FileManagement::GetStringFromTitleTheme(title_theme);
		}
		title_theme_contents.push_back(temp);
	}
	ifvar.close();
	if (title_theme_contents.size() < 1)
	{
		title_theme_contents.push_back(Validation::default_title_theme_file_content);
		if (!(status & Validation::StatusFlags::repaired))
		{
			status = status | Validation::StatusFlags::repaired;
		}
	}
	if (status & Validation::StatusFlags::repaired)
	{
		std::ofstream ofvar;
		ofvar.open(path, std::ios::trunc);
		for (size_t i = 0; i < title_theme_contents.size() - 1; ++i)
		{
			ofvar << title_theme_contents[i] << '\n';
		}
		ofvar << title_theme_contents[title_theme_contents.size()-1];
		ofvar.close();
	}
	
}
void ValidationCheck::Rankings(char& status)
{

	std::vector<std::string> ranking_files = MeridorConsoleLib::GetFilesInDirectory(FolderName::ranking);
	std::ifstream fvar;
	bool good_files = false;

	for (short i = 0; i < static_cast<short>(ranking_files.size()); ++i)
	{
		std::string path = (FolderName::ranking + '\\' + ranking_files[i]).c_str();

		fvar.open(path);
		std::string line;
		int iterations = 0;
		for (; std::getline(fvar, line); ++iterations);
		if (iterations % Validation::ranking_details != 0)
		{
			MessageBox(0, (path + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file, MB_TOPMOST);
			//delete file?
		}
		else
		{
			good_files = true;
		}
		fvar.close();
	}
	if (!good_files)
	{
		if (!(status & Validation::StatusFlags::no_ranking))
		{
			status = status | Validation::StatusFlags::no_ranking;
		}
	}
}
void ValidationCheck::LanguagePacks(char& status)
{
	std::vector<std::string> lang_files = MeridorConsoleLib::GetFilesInDirectory(FolderName::language);
	bool good_files = false;

	for (int i = 0; i < lang_files.size(); ++i)
	{
		std::string path = (FolderName::language + '\\' + lang_files[i]).c_str();
		FileManagement::LoadLanguagePack(path);
		if(
			(static_cast<int>(LanguagePack::text.size()) != LanguagePack::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::car_attributes].size()) != CarAttributes::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::terrain_types].size()) != TerrainTypes::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::main_menu].size()) != Validation::main_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::game_options].size()) != Validation::game_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::game_options_multiplayer].size()) != Validation::multiplayer_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_menu].size()) != Validation::multiplayer_before_game_lobby) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::general_options].size()) != Validation::tot_options) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::tour_information].size()) != GameLobbyInformations::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::selectable_colors].size()) != Validation::colors) ||
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
			(static_cast<int>(LanguagePack::text[LanguagePack::display_settings].size()) != static_cast<int>(MeridorConsoleLib::WindowMode::last)) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::ranking_menu].size()) != Validation::ranking_menu) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::ranking_classification_types].size()) != Validation::ranking_classification) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::ranking_details].size()) != Validation::ranking_details) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::other_strings].size()) != OtherStrings::last) ||
			(static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_client_lobby].size()) < Multiplayer::last))
		{
			//delete file?
			MessageBox(0, (path + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file, MB_TOPMOST);
		}
		else
		{
			good_files = true;
		}
	}
	if (!good_files)
	{
		if (!(status & Validation::StatusFlags::corrupted))
		{
			status = status | Validation::StatusFlags::corrupted;
		}
	}
}

void ValidationCheck::GameFiles(char& status)
{
	Tires(status);
	Cars(status);
	Tours(status);
}
void ValidationCheck::InvalidGameFile(const std::string& directory, const std::string& file_name)
{
	//delete file
	MessageBox(0, (directory + '\\' + file_name + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file, MB_TOPMOST);
}
void ValidationCheck::Tours(char& status)
{
	std::vector<std::string> tours = MeridorConsoleLib::GetFilesInDirectory(FolderName::tour);

	int good_files = false;
	for (size_t i = 0; i < tours.size(); ++i)
	{
		//all cars for race exist
		std::vector<std::string> cars_for_race = FileManagement::GetCarNames(tours[i]);
		std::vector<std::string> car_names = MeridorConsoleLib::GetFilesInDirectory(FolderName::car);
		if (car_names.size() == 0)
		{
			InvalidGameFile(FolderName::tour, tours[i]);
			continue;
		}
		for(size_t j = 0; j < cars_for_race.size(); ++j)
		{
			for (size_t k = 0; k < car_names.size(); ++k)
			{
				if (cars_for_race[j] == car_names[k])
				{
					car_names.erase(car_names.begin() + k);
					break;
				}
			}
		}
		if (car_names.size() > 0)
		{
			InvalidGameFile(FolderName::tour, tours[i]);
			continue;
		}

		//params check
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
		if (!(status & Validation::StatusFlags::unplayable))
		{
			status = status | Validation::StatusFlags::unplayable;
		}
	}
}
void ValidationCheck::Cars(char& status)
{
	const std::vector<std::string> cars = MeridorConsoleLib::GetFilesInDirectory(FolderName::car);
	bool good_files = false;

	for (size_t i = 0; i < cars.size(); ++i)
	{
		std::vector<int> car_params = FileManagement::GetCarParameters(cars[i]);
		if (car_params.size() != CarAttributes::last)//checking if car has all parameters set
		{
			InvalidGameFile(FolderName::car, cars[i]);
			continue;
		}
		for (size_t j = 0; j < CarAttributes::last; ++j)
		{
			if (car_params[j] < 1 || (j == CarAttributes::visibility && j > Validation::max_visibility))
			{
				InvalidGameFile(FolderName::car, cars[i]);
				break;
			}
		}
		good_files = true;
	}
	if (!good_files)
	{
		if (!(status & Validation::StatusFlags::unplayable))
		{
			status = status | Validation::StatusFlags::unplayable;
		}
	}
}
void ValidationCheck::Tires(char& status)
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
			for (short k = 0; k < tire_param_size; ++k)
			{
				if (params[j][k] == 'x')
				{
					x = atoi(params[j].substr(0, k).c_str());
					y = atoi(params[j].substr(k + 1, tire_param_size - k - 1).c_str());
				}
			}
		}
		if (x * y == 0 || x > y)//checking if tires attributes make sense
		{
			InvalidGameFile(FolderName::tire, tires[i]);
			continue;
		}
		good_files = true;
	}
	if (!good_files)
	{
		if (!(status & Validation::StatusFlags::unplayable))
		{
			status = status | Validation::StatusFlags::unplayable;
		}
	}
}

