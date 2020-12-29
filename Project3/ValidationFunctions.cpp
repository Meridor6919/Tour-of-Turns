#include "ValidationFunctions.h"

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
		MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		return false;
	}
	for (short i = 0; i < number_of_tours; ++i)
	{
		std::vector<std::string> params = FileManagement::GetTourParameters(tours[i], 0, INT_MAX);
		if (static_cast<short>(params.size()) < 1)
		{
			MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}
		for (short j = 0; j < static_cast<short>(params.size()); ++j)
		{
			const short size_of_segment = static_cast<short>(params[j].size());
			if (params[j][0] - 48 < 0 || params[j][0] - 48 >= TerrainTypes::last)//terrain type validation
			{
				MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
				return false;
			}
			else if (size_of_segment > 11)//checking if safe speed isn't exceeding speed of light
			{
				MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
				return false;
			}
			else if (size_of_segment != 1 && atoi(params[j].substr(1, size_of_segment - 1).c_str()) < 1)//checking if safe speed is at least 1
			{
				MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
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
			MessageBox(0, (tours[i] + " " + ErrorMsg::available_cars).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}
		for (short j = 0; j < static_cast<short>(cars.size()); ++j)
		{
			if (static_cast<short>(FileManagement::GetCarParameters(cars[j]).size()) != CarAttributes::last)//checking if car has all parameters set
			{
				MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
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
			MessageBox(0, (FolderName::tire + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
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
			MessageBox(0, (FolderName::tire + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}
	}
	return true;
}
bool Validation::ValidateRanking()
{
	std::vector<std::string> ranking_files = MeridorConsoleLib::ReadFile(FolderName::tour + '\\' + FileName::ranking);
	std::ifstream fvar;
	for (short i = 0; i < static_cast<short>(ranking_files.size()); ++i)
	{
		fvar.open(FolderName::tour + '\\' + ranking_files[i]);
		std::string line;
		int iterations = 0;
		for (; std::getline(fvar, line); ++iterations);
		if (iterations % Validation::ranking_details != 0)
		{
			MessageBox(0, (+" " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
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
	if (static_cast<int>(LanguagePack::text[LanguagePack::main_menu_options].size()) != Validation::main_menu)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::game_menu_options].size()) != Validation::game_menu)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_menu_options].size()) != Validation::multiplayer_menu)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_before_game_lobby].size()) != Validation::multiplayer_before_game_lobby)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::tot_general_options].size()) != Validation::tot_options)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::game_lobby_informations].size()) != GameLobbyInformations::last)
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
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_chancebox_informations].size()) != Validation::chancebox)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_general_informations].size()) != Validation::general_info)
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
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_names_of_boxes].size()) != Validation::names_of_boxes)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_segments_descriptions].size()) != TerrainTypes::last * 2)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::race_accident_effects].size()) != Validation::accidents)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::on_off].size()) != 2)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::ranking_search_menu].size()) != Validation::ranking_menu)
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
	if (static_cast<int>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::loading_wheel].size()) != 8)
	{
		return false;
	}
	if (static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_lobby].size()) < Multiplayer::last)
	{
		return false;
	}
	return true;
}
