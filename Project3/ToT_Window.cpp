#include "ToT_Window.h"

ToT_Window::ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns) : Window(title, color1, color2, chars_in_rows, chars_in_columns)
{
	const COORD infobox_position = { 0,static_cast<short>(GetHeight() - 12) };
	this->infobox = std::make_shared<InfoBox>(10, infobox_position, 1, *this);
	playable = true;
	enable_ranking = true;
	const std::string error_msg = " " + ErrorMsg::missing_file;
	const char* errot_title = ErrorTitle::missing_file.c_str();

	if (!SaveFileNames(FolderName::tour, FolderName::tour + '\\' + FileName::ranking, ExtName::ranking))
	{
		enable_ranking = false;
	}
	if (!SaveFileNames(FolderName::tire, FolderName::tire + '\\' + FileName::tire, ExtName::tire))
	{
		MessageBox(0, (ExtName::tire +error_msg).c_str(), errot_title, 0);
		playable = false;
	}
	if (!SaveFileNames(FolderName::tour, FolderName::tour + '\\' + FileName::tour, ExtName::tour))
	{
		MessageBox(0, (ExtName::tour + error_msg).c_str(), errot_title, 0);
		playable = false;
	}
	if (!SaveFileNames(FolderName::language, FolderName::language + '\\' + FileName::language, ExtName::language))
	{
		MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), 0);
		exit(0);
	}
	enable_ranking = enable_ranking & ValidateRanking();
	playable = playable & ValidateGameFiles();
	wav_transformer = new WavTransformer(FolderName::main + '\\' + FileName::music);
	LoadAtributes();
}
std::string ToT_Window::UpdateRankingFavorites(std::string text, std::string phrase, int added_value)
{
	if (!added_value)
	{
		return "";
	}
	bool phrase_value_flag = true;
	std::string current_phrase = "";
	std::string value = "";
	int phrase_index = -1;

	for (int i = 0; i < static_cast<int>(text.size()); ++i)
	{
		if (text[i] == ':')
		{
			phrase_value_flag = !phrase_value_flag;
			if (!phrase_value_flag)
			{
				if (current_phrase == phrase)
				{
					phrase_index = i;
				}
			}
			else
			{
				if (phrase_index != -1)
				{
					return text.substr(0, phrase_index + 1) + std::to_string(atoi(value.c_str()) + added_value) + text.substr(i, static_cast<int>(text.size()) - i);
				}
				current_phrase = "";
				value = "";
			}
		}
		else if (phrase_value_flag)
		{
			current_phrase += text[i];
		}
		else
		{
			value += text[i];
		}
	}
	return text + phrase + ':' + std::to_string(added_value) + ':';
}
void ToT_Window::LoadAtributes()
{
	std::fstream fvar;
	hamachi_enabled = true;
	music_volume = 1.0f;
	ais = 0;

	fvar.open(FolderName::main + '\\' + FileName::config, std::ios::in);
	fvar >> color1;
	fvar >> color2;
	fvar >> music_volume;
	fvar >> hamachi_enabled;
	fvar >> ais;
	fvar >> name;
	fvar >> lang;
	fvar >> timer_settings;
	fvar.close();
	
	bool name_valid = true;
	for (int i = 0; i < static_cast<int>(name.size()); ++i)
	{
		if (name[i] == '_')
		{
			name[i] = ' ';
		}
		else if (!(name[i] >= 'a' && name[i] <= 'z' || name[i] >= 'A' && name[i] <= 'Z'))
		{
			name_valid = false;
			break;
		}
	}

	if (!LanguagePack::LoadLanguagePack(FolderName::language + '\\' + lang))
	{
		MessageBox(0, (lang + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		
		std::vector<std::string> languages = this->ReadFile(FolderName::language + '\\' + FileName::language);
		bool no_valid_lang_packs = true;
		for (int i = 0; i < static_cast<int>(languages.size()); ++i)
		{
			if (LanguagePack::LoadLanguagePack(FolderName::language + '\\' + languages[i]))
			{
				no_valid_lang_packs = false;
				lang = languages[i];
				MessageBox(0, (languages[i] + ErrorMsg::placeholder_language).c_str(), ErrorTitle::placeholder_language.c_str(), 0);
				break;
			}
		}
		if (no_valid_lang_packs)
		{
			MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), 0);
			exit(0);
		}
	}
	if (color1 < 0 || color1 > 15)
	{
		color1 = 15;
	}
	if (color2 < 0 || color2 > 15)
	{
		color2 = 10;
	}
	if (ais < 0 || ais > 7)
	{
		ais = 7;
	}
	if (static_cast<int>(name.size()) < 1 || static_cast<int>(name.size()) > GameConstants::maximum_name_length || !name_valid)
	{
		name = LanguagePack::text[LanguagePack::other_strings][OtherStrings::default_name];
	}
	if (timer_settings < 0 || timer_settings > GameConstants::maximum_timer)
	{
		timer_settings = 0;
	}
	wav_transformer->ChangeVolume(music_volume);
	if (music_volume)
	{
		wav_transformer->Start(SND_ASYNC | SND_LOOP);
	}
}
bool ToT_Window::ValidateGameFiles()
{
	return ValidateCarFiles()*ValidateTireFiles()*ValidateTourFiles();
}
bool ToT_Window::ValidateTourFiles()
{
	std::vector<std::string> tours = GetTourNames();
	const short number_of_tours = static_cast<short>(tours.size());
	if (!number_of_tours)
	{
		MessageBox(0, (FolderName::tour + ' ' + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		return false;
	}
	for (short i = 0; i < number_of_tours; ++i)
	{
		std::vector<std::string> params = GetTourParameters(tours[i], 0, INT_MAX);
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
				MessageBox(0, (FolderName::tour + ' '  + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
				return false;
			}
		}
	}
	
	return true;
}
bool ToT_Window::ValidateCarFiles()
{
	const std::vector<std::string> tours = GetTourNames();

	for (short i = 0; i < static_cast<short>(tours.size()); ++i)
	{
		const std::vector<std::string> cars = GetCarNames(tours[i]);
		if (!static_cast<bool>(cars.size()))//checking if there is at least one car that can be driven for any given tour
		{
			MessageBox(0, (tours[i] + " " + ErrorMsg::available_cars).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}		
		for (short j = 0; j < static_cast<short>(cars.size()); ++j)
		{
			if (static_cast<short>(GetCarParameters(cars[j]).size()) != CarAttributes::last)//checking if car has all parameters set
			{
				MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
				return false;
			}
		}
	}
	return true;
}
bool ToT_Window::ValidateTireFiles()
{
	const std::vector<std::string> tires = GetTireNames();
	int x = 0;
	int y = 0;

	for (short i = 0; i < static_cast<short>(tires.size()); ++i)
	{
		const std::vector<std::string> params = GetTireParameters(tires[i]);
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
		if (x*y == 0 || x > y)//checking if tires attributes make sense
		{
			MessageBox(0, (FolderName::tire + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}
	}
	return true;
}
bool ToT_Window::ValidateRanking()
{
	std::vector<std::string> ranking_files = ReadFile(FolderName::tour + '\\' + FileName::ranking);
	std::ifstream fvar;
	for (short i = 0; i < static_cast<short>(ranking_files.size()); ++i)
	{
		fvar.open(FolderName::tour + '\\' +  ranking_files[i]);
		std::string line;
		int iterations = 0;
		for (; std::getline(fvar, line); ++iterations);
		if (iterations % GameConstants::validate_ranking_details != 0)
		{
			MessageBox(0, (+" " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}
		fvar.close();
	}
	return true;
}
bool ToT_Window::SaveFileNames(std::string src_path, std::string dst_path, const std::string ext)
{
	dst_path = "dir " + src_path + "\\*" + ext + " > " + dst_path + " /b";
	if (system(dst_path.c_str()))
	{
		//Clearing error message that shows in a console window by default when files not found
		SetConsoleCursorPosition(window_handle, { 0,0 });
		std::cout << "                       ";
		SetConsoleCursorPosition(window_handle, { 0,0 });
		return false;
	}
	return true;
}
void ToT_Window::RemoveExtension(std::vector<std::string>& vector, std::string extension)
{
	short extension_size = static_cast<short>(extension.size());
	for (int i = 0; i < static_cast<int>(vector.size()); ++i)
	{
		vector[i] = vector[i].substr(0, static_cast<short>(vector[i].size()) - extension_size);
	}
}
void ToT_Window::Title(const COORD starting_point, const Text::TextAlign text_align)
{
	const COORD orientation_point = { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * LanguagePack::text[LanguagePack::title_main][0].size()), starting_point.Y };
	const short decoration_distance = 5;
	const std::string main_decoration = "{ }";
	const std::string additional_decoration = "*";
	const short main_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main].size());
	const short additional_title_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional].size());
	//Main text
	SetConsoleTextAttribute(window_handle, color2);
	for (short i = 0; i < main_title_size; ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X, orientation_point.Y + i });
		std::cout << LanguagePack::text[LanguagePack::title_main][i];
	}
	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < additional_title_size; ++i)
	{
		const short main_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		const short additional_line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_additional][i].size());
		SetConsoleCursorPosition(window_handle, { orientation_point.X + main_line_size / 2 - additional_line_size / 2, orientation_point.Y + i + main_title_size / 3 });
		std::cout << LanguagePack::text[LanguagePack::title_additional][i];
	}
	//Decoration
	for (short i = 0; i < main_title_size; ++i)
	{
		const short decoration_size = static_cast<short>(decoration_distance + main_decoration.size());
		const short line_size = static_cast<short>(LanguagePack::text[LanguagePack::title_main][i].size());
		SetConsoleTextAttribute(window_handle, color2);
		SetConsoleCursorPosition(window_handle, { orientation_point.X - decoration_size - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleCursorPosition(window_handle, { orientation_point.X + line_size + decoration_distance - 1 - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleTextAttribute(window_handle, color1);
		SetConsoleCursorPosition(window_handle, { orientation_point.X - decoration_size + 1 - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
		SetConsoleCursorPosition(window_handle, { orientation_point.X + line_size + decoration_distance - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
	}
}
std::string ToT_Window::GetClassifiedDetail(std::string text, int classification_type)
{
	int start = 0;
	int count = 0;
	for (int i = 0; i < static_cast<int>(text.size()); ++i)
	{
		if ((text[i] == '\t') || (i + 1 == static_cast<int>(text.size())))
		{
			if (!classification_type)
			{
				count = i - start + (i + 1 == static_cast<int>(text.size()));
				break;
			}
			else
			{
				--classification_type;
				start = i + 1;
			}
		}
	}
	return text.substr(start, count);
}
std::vector<std::string> ToT_Window::GetTourNames()
{
	return ReadFile(FolderName::tour + '\\' + FileName::tour);
}
std::vector<std::string> ToT_Window::GetCarNames(const std::string tour)
{
	return ReadFile(FolderName::tour + '\\' + tour);
}
std::vector<std::string> ToT_Window::GetTireNames()
{
	return ReadFile(FolderName::tire + '\\' + FileName::tire);
}
std::vector<std::string> ToT_Window::GetTourParameters(std::string tour, int position, const int visibility)
{
	std::vector<std::string> ret;
	std::fstream fvar;
	std::string helper;

	fvar.open((FolderName::tour + '\\' + tour).c_str());
	do
	{
		std::getline(fvar, helper);
	} 
	while (helper != "");
	for (int i = 0; i < position; ++i)
	{
		std::getline(fvar, helper);
	}
	for (int i = 0; i < visibility; ++i)
	{
		if (!std::getline(fvar, helper))
		{
			break;
		}
		ret.push_back(std::move(helper));
	}
	fvar.close();
	return ret;
}
std::vector<int> ToT_Window::GetCarParameters(const std::string path)
{
	const std::vector<std::string> data = ReadFile(FolderName::car + '\\' + path);
	std::vector<int> car_parameters;
	for (short i = 0; i < CarAttributes::last; ++i)
	{
		if (atoi(data[i].c_str()) < 1 || atoi(data[i].c_str()))
		{
			car_parameters.push_back(atoi(data[i].c_str()));
		}
	}
	return car_parameters;
}
std::vector<std::string> ToT_Window::GetTireParameters(const std::string path)
{
	return ReadFile(FolderName::tire + '\\' + path);
}
bool ToT_Window::GetHamachiConnectionFlag()
{
	return hamachi_enabled;
}
int ToT_Window::GetAIs()
{
	return ais;
}
std::string ToT_Window::GetName()
{
	return name;
}
int ToT_Window::GetTimerSettings()
{
	return timer_settings;
}
std::string ToT_Window::GetLanguage()
{
	return lang;
}
bool ToT_Window::RankingFound()
{
	return enable_ranking;
}
bool ToT_Window::IsPlayable()
{
	return playable;
}
bool ToT_Window::GetMultiplayer()
{
	return multiplayer;
}
void ToT_Window::SetHamachiConnectionFlag(const bool flag)
{
	hamachi_enabled = flag;
}
void ToT_Window::SetAIs(int number_of_ais)
{
	ais = number_of_ais;
}
void ToT_Window::SetName(std::string name)
{
	this->name = name;
}
void ToT_Window::SetTimerSettings(int timer_settings)
{
	this->timer_settings = timer_settings;
}
void ToT_Window::SetLanguage(std::string lang)
{
	this->lang = lang;
	if (!LanguagePack::LoadLanguagePack(FolderName::language + '\\' + lang))
	{
		MessageBox(0, (lang + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);

		std::vector<std::string> languages = this->ReadFile(FolderName::language + '\\' + FileName::language);
		bool no_valid_lang_packs = true;
		for (int i = 0; i < static_cast<int>(languages.size()); ++i)
		{
			if (LanguagePack::LoadLanguagePack(FolderName::language + '\\' + languages[i]))
			{
				no_valid_lang_packs = false;
				lang = languages[i];
				MessageBox(0, (languages[i] + ErrorMsg::placeholder_language).c_str(), ErrorTitle::placeholder_language.c_str(), 0);
				break;
			}
		}
		if (no_valid_lang_packs)
		{
			MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), 0);
			exit(0);
		}
	}
}
void ToT_Window::SetMultiplayer(bool multiplayer)
{
	this->multiplayer = multiplayer;
}
void ToT_Window::SaveRanking(std::string tour, std::string name, int place, int score, bool crash, int attacks, int drifts, int durability_burning, std::string car, std::string tires)
{
	const bool classification[GameConstants::validate_ranking_classification] = { true, ais == 7, multiplayer && ais != 7 };
	const std::string ranking_path = FolderName::tour + '\\' + tour.substr(0, static_cast<int>(tour.size()) - static_cast<int>(ExtName::tour.size())) + ExtName::ranking;
	std::vector<std::string> ranking_data = { "" };
	int racer_index = -1;
	std::fstream fvar;
	std::string temp;
	car = car.substr(0, static_cast<int>(car.size()) - static_cast<int>(ExtName::car.size()));
	tires = tires.substr(0, static_cast<int>(tires.size()) - static_cast<int>(ExtName::tire.size()));

	//Loading data
	fvar.open(ranking_path.c_str(), std::ios::in);
	for (int i = 0; std::getline(fvar, ranking_data[i]); ++i)
	{
		ranking_data.push_back("");
	}
	fvar.close();
	//Searching if the racer is already in the data
	for (int i = 0; i < static_cast<int>(ranking_data.size()); i+=GameConstants::validate_ranking_details)
	{
		if (ranking_data[i] == name)
		{
			racer_index = i+1;
			break;
		}
	}
	//Add record if racer not found
	if (racer_index < 0)
	{
		racer_index = static_cast<int>(ranking_data.size());
		ranking_data[racer_index - 1] = name;
		for (int i = 0; i < GameConstants::validate_ranking_details - 1; ++i)
		{
			ranking_data.push_back("");
		}
		ranking_data.push_back("");
	}
	//Modifying ranking_data by selected racer's performance
	if (!crash)
	{
		const std::vector<int> added_value = { 1, place == 1,place, score, 0, crash, attacks, drifts, durability_burning };
		for (int i = 0; i < static_cast<int>(added_value.size()); ++i)
		{
			temp = "";
			for (int j = 0; j < GameConstants::validate_ranking_classification; ++j)
			{
				temp += std::to_string(atoi(GetClassifiedDetail(ranking_data[racer_index + i], j).c_str()) + added_value[i] * classification[j]) + '\t';
			}
			ranking_data[racer_index + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
		}
		temp = "";
		for (int i = 0; i < GameConstants::validate_ranking_classification; ++i)
		{
			int local_score = atoi(GetClassifiedDetail(ranking_data[racer_index + 4], i).c_str());
			if (classification[i] && (score < local_score || local_score == 0))
			{
				local_score = score;
			}
			temp += std::to_string(local_score) + '\t';
		}
		ranking_data[racer_index + 4] = temp.substr(0, static_cast<int>(temp.size()) - 1);
	}
	else
	{
		for (int i = 0; i < 10; i += 5)
		{
			temp = "";
			for (int j = 0; j < GameConstants::validate_ranking_classification; ++j)
			{
				temp += std::to_string(atoi(GetClassifiedDetail(ranking_data[racer_index + i], j).c_str()) + 1 * classification[j]) + '\t';
			}
			ranking_data[racer_index + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
		}
	}
	for (int i = 0; i < 2; ++i)
	{
		temp = "";
		for (int j = 0; j < GameConstants::validate_ranking_classification; ++j)
		{
			temp += UpdateRankingFavorites(GetClassifiedDetail(ranking_data[racer_index + 9 + i], j), i % 2 ? car : tires, 1) + "\t";
		}
		ranking_data[racer_index + 9 + i] = temp.substr(0, static_cast<int>(temp.size()) - 1);
	}
	//Saving ranking_data to file
	fvar.open(ranking_path.c_str(), std::ios::out);
	for (int i = 0; i < static_cast<int>(ranking_data.size())-1; ++i)
	{
		if(i)
		{
			fvar << '\n';
		}
		fvar << ranking_data[i];
	}
	fvar.close();
}
void ToT_Window::SaveAtributes()
{
	for (int i = 0; i < static_cast<int>(name.size()); ++i)
	{
		if (name[i] == ' ')
		{
			name[i] = '_';
		}
	}
	std::ofstream fvar;
	fvar.open(FolderName::main + '\\' + FileName::config);
	fvar << color1 << '\n';
	fvar << color2 << '\n';
	fvar << music_volume << '\n';
	fvar << hamachi_enabled << '\n';
	fvar << ais << '\n';
	fvar << name + '\n';
	fvar << lang + '\n';
	fvar << timer_settings << '\n';
	fvar.close();
}