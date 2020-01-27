#include "ToT_Window.h"

ToT_Window::ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns) : Window(title, color1, color2, chars_in_rows, chars_in_columns)
{
	const COORD infobox_position = { 0,static_cast<short>(GetHeight() - 12) };
	this->infobox = std::make_shared<InfoBox>(10, Text::TextAlign::left, infobox_position, 1, *this);
	playable = true;
	enable_ranking = true;
	const std::string error_msg = " " + ErrorMsg::missing_file;
	const char* errot_title = ErrorTitle::missing_file.c_str();

	if (!SaveFileNames(FolderName::tour, FolderName::tour + "\\" + FileName::ranking, ExtName::ranking))
	{
		enable_ranking = false;
	}
	if (!SaveFileNames(FolderName::tire, FolderName::tire + "\\" + FileName::tire, ExtName::tire))
	{
		MessageBox(0, (ExtName::tire +error_msg).c_str(), errot_title, 0);
		playable = false;
	}
	if (!SaveFileNames(FolderName::tour, FolderName::tour + "\\" + FileName::tour, ExtName::tour))
	{
		MessageBox(0, (ExtName::tour + error_msg).c_str(), errot_title, 0);
		playable = false;
	}
	if (!SaveFileNames(FolderName::language, FolderName::language + "\\" + FileName::language, ExtName::language))
	{
		MessageBox(0, ErrorMsg::language_error.c_str(), ErrorTitle::language_error.c_str(), 0);
		exit(0);
	}
	enable_ranking = enable_ranking & ValidateRanking();
	playable = playable & ValidateGameFiles();
	wav_transformer = new WavTransformer(FolderName::main + "\\" + FileName::music);
	LoadAtributes();
}
void ToT_Window::LoadAtributes()
{
	std::fstream fvar;
	hamachi_enabled = true;
	music_volume = 1.0f;
	ais = 0;

	fvar.open(FolderName::main + "\\" + FileName::config, std::ios::in);
	fvar >> color1;
	fvar >> color2;
	fvar >> music_volume;
	fvar >> hamachi_enabled;
	fvar >> ais;
	fvar >> name;
	fvar >> lang;
	fvar.close();

	if (!LanguagePack::LoadVector(FolderName::language + "\\" + lang))
	{
		MessageBox(0, (lang + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		
		std::vector<std::string> languages = this->ReadFile(FolderName::language + "\\" + FileName::language);
		bool no_valid_lang_packs = true;
		for (int i = 0; i < static_cast<int>(languages.size()); ++i)
		{
			if (LanguagePack::LoadVector(FolderName::language + "\\" + languages[i]))
			{
				no_valid_lang_packs = false;
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
	if (name.size() < 1 || name.size() > 14)
	{
		name = LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::default_name];
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
		MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		return false;
	}
	for (short i = 0; i < number_of_tours; ++i)
	{
		std::vector<std::string> params = GetTourParameters(tours[i], 0, INT_MAX);
		if (static_cast<short>(params.size()) < 1)
		{
			MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}
		for (short j = 0; j < static_cast<short>(params.size()); ++j)
		{
			const short size_of_segment = static_cast<short>(params[j].size());
			if (params[j][0] - 48 < 0 || params[j][0] - 48 >= TireModifiers::last)//terrain type validation
			{
				MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
				return false;
			}
			else if (size_of_segment > 11)//checking if safe speed isn't exceeding speed of light
			{
				MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
				return false;
			}
			else if (size_of_segment != 1 && atoi(params[j].substr(1, size_of_segment - 1).c_str()) < 1)//checking if safe speed is at least 1
			{
				MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
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
			MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}		
		for (short j = 0; j < static_cast<short>(cars.size()); ++j)
		{
			if (static_cast<short>(GetCarParameters(cars[j]).size()) != CarModifiers::last)//checking if car has all parameters set
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
		if (static_cast<short>(params.size()) != TireModifiers::last)//checking if tires have all parameters set
		{
			MessageBox(0, (FolderName::tire + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
			return false;
		}
		for (short j = 0; j < TireModifiers::last; ++j)//getting tires attributes
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
	std::vector<std::string> ranking_files = ReadFile(FolderName::tour + "\\" + FileName::ranking);
	std::ifstream fvar;
	for (short i = 0; i < static_cast<short>(ranking_files.size()); ++i)
	{
		fvar.open(ranking_files[i]);
		std::string line;
		int iterations = 0;
		for (; std::getline(fvar, line); ++iterations);
		if (iterations % ValidationConstants::ranking_details != 0)
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
	const COORD orientation_point = { starting_point.X - static_cast<short>(static_cast<float>(text_align) / 2.0f * LanguagePack::vector_of_strings[LanguagePack::title_main][0].size()), starting_point.Y };
	const short decoration_distance = 5;
	const std::string main_decoration = "{ }";
	const std::string additional_decoration = "*";
	const short main_title_size = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::title_main].size());
	const short additional_title_size = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::title_additional].size());
	//Main text
	SetConsoleTextAttribute(window_handle, color2);
	for (short i = 0; i < main_title_size; ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X, orientation_point.Y + i });
		std::cout << LanguagePack::vector_of_strings[LanguagePack::title_main][i];
	}
	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < additional_title_size; ++i)
	{
		const short main_line_size = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::title_main][i].size());
		const short additional_line_size = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::title_additional][i].size());
		SetConsoleCursorPosition(window_handle, { orientation_point.X + main_line_size / 2 - additional_line_size / 2, orientation_point.Y + i + main_title_size / 3 });
		std::cout << LanguagePack::vector_of_strings[LanguagePack::title_additional][i];
	}
	//Decoration
	for (short i = 0; i < main_title_size; ++i)
	{
		const short decoration_size = static_cast<short>(decoration_distance + main_decoration.size());
		const short line_size = static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::title_main][i].size());
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
std::vector<std::string> ToT_Window::GetTourNames()
{
	return ReadFile(FolderName::tour + "\\" + FileName::tour);
}
std::vector<std::string> ToT_Window::GetCarNames(const std::string tour)
{
	return ReadFile(FolderName::tour + "\\" + tour);
}
std::vector<std::string> ToT_Window::GetTireNames()
{
	return ReadFile(FolderName::tire + "\\" + FileName::tire);
}
std::vector<std::string> ToT_Window::GetTourParameters(std::string tour, int position, const int visibility)
{
	std::vector<std::string> ret;
	std::fstream fvar;
	std::string helper;

	fvar.open((FolderName::tour + "\\" + tour).c_str());
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
	const std::vector<std::string> data = ReadFile(FolderName::car + "\\" + path);
	std::vector<int> car_parameters;
	for (short i = 0; i < CarModifiers::last; ++i)
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
	return ReadFile(FolderName::tire + "\\" + path);
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
bool ToT_Window::SetLanguage(std::string lang)
{
	return LanguagePack::LoadVector(lang);
}
void ToT_Window::SaveAtributes()
{
	std::fstream fvar;
	fvar.open(FolderName::main + "\\" + FileName::config);
	fvar << color1 << "\n";
	fvar << color2 << "\n";;
	fvar << music_volume << "\n";;
	fvar << hamachi_enabled << "\n";;
	fvar << ais << "\n";
	fvar << name << "\n";
	fvar << lang << "\n";
	fvar.close();
}