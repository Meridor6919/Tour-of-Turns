#include "ValidationFunctions.h"

void Validation::ValidateToTGameConfig(ToTGameConfig &game_config)
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
	if (!MeridorConsoleLib::Between(0, 7, game_config.ais))
	{
		game_config.ais = 7;
	}
	if (!MeridorConsoleLib::Between(1, Validation::maximum_name_length, static_cast<int>(game_config.name.size())) || !name_valid)
	{
		game_config.name = LanguagePack::text[LanguagePack::other_strings][OtherStrings::default_name];
	}
	if (!MeridorConsoleLib::Between(0, Validation::maximum_timer, game_config.timer_settings))
	{
		game_config.timer_settings = 0;
	}
}
void Validation::ValidateToTWindowConfig(ToTWindowConfig& window_config)
{
	if (window_config.window_info.characters_capacity.Y < Validation::minimum_window_size.Y)
	{
		window_config.window_info.characters_capacity.Y = Validation::minimum_window_size.Y;
	}
	if (window_config.window_info.characters_capacity.X < Validation::minimum_window_size.X)
	{
		window_config.window_info.characters_capacity.X = Validation::minimum_window_size.X;
	}
	if (!MeridorConsoleLib::Between(2, 15, window_config.window_info.main_color))
	{
		window_config.window_info.main_color = Validation::default_main_color;
	}
	if (!MeridorConsoleLib::Between(FOREGROUND_BLUE, Validation::colors, window_config.window_info.secondary_color))
	{
		window_config.window_info.main_color = Validation::default_main_color;
		window_config.window_info.secondary_color = Validation::default_secondary_color;
	}
	if (window_config.window_info.secondary_color == window_config.window_info.main_color)
	{
		window_config.window_info.main_color = Validation::default_main_color;
		window_config.window_info.secondary_color = Validation::default_secondary_color;
	}
	if (window_config.window_info.window_mode > MeridorConsoleLib::WindowMode::last)
	{
		window_config.window_info.window_mode = MeridorConsoleLib::WindowMode::fullscreen;
	}
	if (!MeridorConsoleLib::Between(0.0f, 1.0f, window_config.music_volume))
	{
		window_config.music_volume = 0;
	}
}
bool Validation::ValidateGameFiles()
{
	return ValidateCarFiles() * ValidateTireFiles() * ValidateTourFiles();
}
bool Validation::ValidateTourFiles()
{
	std::vector<std::string> tours = FileManagement::GetTourNames();
	const short number_of_tours = static_cast<short>(tours.size());
	if (!number_of_tours)
	{
		MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
		return false;
	}
	for (short i = 0; i < number_of_tours; ++i)
	{
		std::vector<std::string> params = FileManagement::GetTourParameters(tours[i], 0, INT_MAX);
		if (static_cast<short>(params.size()) < 1)
		{
			MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
			return false;
		}
		for (short j = 0; j < static_cast<short>(params.size()); ++j)
		{
			const short size_of_segment = static_cast<short>(params[j].size());
			if (!MeridorConsoleLib::Between(0, TerrainTypes::last -1, params[j][0] - 48))//terrain type validation
			{
				MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
				return false;
			}
			else if (size_of_segment > 11)//checking if safe speed isn't exceeding speed of light
			{
				MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
				return false;
			}
			else if (size_of_segment != 1 && atoi(params[j].substr(1, size_of_segment - 1).c_str()) < 1)//checking if safe speed is at least 1
			{
				MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
				return false;
			}
		}
	}
	return true;
}
bool Validation::ValidateCarFiles()
{
	const std::vector<std::string> tours = FileManagement::GetTourNames();

	for (short i = 0; i < static_cast<short>(tours.size()); ++i)
	{
		const std::vector<std::string> cars = FileManagement::GetCarNames(tours[i]);
		if (!static_cast<bool>(cars.size()))//checking if there is at least one car that can be driven for any given tour
		{
			MessageBox(0, (tours[i] + " " + ErrorMsg::available_cars).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
			return false;
		}
		for (short j = 0; j < static_cast<short>(cars.size()); ++j)
		{
			if (static_cast<short>(FileManagement::GetCarParameters(cars[j]).size()) != CarAttributes::last)//checking if car has all parameters set
			{
				MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
				return false;
			}
		}
	}
	return true;
}
bool Validation::ValidateTireFiles()
{
	const std::vector<std::string> tires = FileManagement::GetTireNames();
	int x = 0;
	int y = 0;

	for (short i = 0; i < static_cast<short>(tires.size()); ++i)
	{
		const std::vector<std::string> params = FileManagement::GetTireParameters(tires[i]);
		if (static_cast<short>(params.size()) != TerrainTypes::last)//checking if tires have all parameters set
		{
			MessageBox(0, (FolderName::tire + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
			return false;
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
			MessageBox(0, (FolderName::tire + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
			return false;
		}
	}
	return true;
}
bool Validation::ValidateRanking()
{
	std::vector<std::string> ranking_files = MeridorConsoleLib::GetFilesInDirectory(FolderName::ranking);
	std::ifstream fvar;

	for (short i = 0; i < static_cast<short>(ranking_files.size()); ++i)
	{
		fvar.open(FolderName::ranking + '\\' + ranking_files[i]);
		std::string line;
		int iterations = 0;
		for (; std::getline(fvar, line); ++iterations);
		if (iterations % Validation::ranking_details != 0)
		{
			MessageBox(0, (+" " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), MB_TOPMOST);
			return false;
		}
		fvar.close();
	}
	return true;
}
bool Validation::ValidateLanguagePack()
{
	if (static_cast<int>(LanguagePack::text.size()) != LanguagePack::last)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::car_attributes].size()) != CarAttributes::last)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::terrain_types].size()) != TerrainTypes::last)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::main_menu].size()) != Validation::main_menu)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::game_options].size()) != Validation::game_menu)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::game_options_multiplayer].size()) != Validation::multiplayer_menu)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_menu].size()) != Validation::multiplayer_before_game_lobby)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::general_options].size()) != Validation::tot_options)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::tour_information].size()) != GameLobbyInformations::last)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::selectable_colors].size()) != Validation::colors)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_actions].size()) != Validation::actions)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_chancebox].size()) != Validation::chancebox)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_player_informations].size()) != Validation::general_info)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_leaderboard].size()) != Validation::leaderboard)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_actions_descritions].size()) != Validation::actions)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_distances].size()) != Validation::distances)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_box_names].size()) != Validation::names_of_boxes)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::tour_segments_descriptions].size()) != TerrainTypes::last * 2)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::accident_effects].size()) != Validation::accidents)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::on_off].size()) != 2)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::display_settings].size()) != static_cast<int>(MeridorConsoleLib::WindowMode::last))
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::ranking_menu].size()) != Validation::ranking_menu)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::ranking_classification_types].size()) != Validation::ranking_classification)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::ranking_details].size()) != Validation::ranking_details)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::other_strings].size()) != OtherStrings::last)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_client_lobby].size()) < Multiplayer::last)
	{
		return false;
	}
	return true;
}
