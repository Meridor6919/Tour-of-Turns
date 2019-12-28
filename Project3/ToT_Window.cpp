#include "ToT_Window.h"

ToT_Window::ToT_Window(const std::string title, const int color1, const int color2, const short chars_in_rows, const short chars_in_columns) : Window(title, color1, color2, chars_in_rows, chars_in_columns)
{
	playable = true;
	ranking_found = true;

	if (!SaveFileNames(FolderName::tour, FolderName::tour + "\\" + FileName::ranking, ExtName::ranking))
	{
		ranking_found = false;
	}
	if (!SaveFileNames(FolderName::tire, FolderName::tire + "\\" + FileName::tire, ExtName::tire))
	{
		MessageBox(0, (ExtName::tire +" "+ ErrorMsg::missing_file).c_str(), ErrorTitle::missing_file.c_str(), 0);
		playable = false;
	}
	if (!SaveFileNames(FolderName::tour, FolderName::tour + "\\" + FileName::tour, ExtName::tour))
	{
		MessageBox(0, (ExtName::tour + " " + ErrorMsg::missing_file).c_str(), ErrorTitle::missing_file.c_str(), 0);
		playable = false;
	}
	if (!ValidateGameFiles())
	{
		playable = false;
	}
	wav_transformer = new WavTransformer(FolderName::main + "\\" + FileName::music);
	LoadAtributes();
}
void ToT_Window::LoadAtributes()
{
	hamachi_enabled = true;
	music_volume = 1.0f;
	ais = 0;

	std::fstream fvar;
	fvar.open(FolderName::main + "\\" + FileName::config, std::ios::in);
	fvar >> color1;
	fvar >> color2;
	fvar >> music_volume;
	fvar >> hamachi_enabled;
	fvar >> ais;
	fvar >> name;
	fvar.close();

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
		name = String::default_name;
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
	bool invalid = false;

	if (static_cast<int>(tours.size()) == 0)
	{
		invalid = true;
	}
	for (int i = 0; i < static_cast<int>(tours.size()); ++i)
	{
		std::vector<std::string> params = GetTourParameters(tours[i], 0, INT_MAX);
		if (static_cast<int>(params.size()) < 1)
		{
			invalid = true;
			break;
		}
		for (int j = 0; j < static_cast<int>(params.size()); ++j)
		{
			if (params[j][0] - 48 < 0 || params[j][0] - 48 > TireModifiers::last)
			{
				invalid = true;
				break;
			}
			else if (static_cast<int>(params[j].size()) > 5)
			{
				invalid = true;
				break;
			}
			else if (static_cast<int>(params[j].size()) != 1 && atoi(params[j].substr(1, static_cast<int>(params[j].size()) - 1).c_str()) < 1)
			{
				invalid = true;
				break;
			}
		}
	}
	if (invalid)
	{
		MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		return false;
	}
	return true;
}
bool ToT_Window::ValidateCarFiles()
{
	std::vector<std::string> tours = GetTourNames();
	bool invalid = false;

	for (int i = 0; i < static_cast<int>(tours.size()); ++i)
	{
		std::vector<std::string> cars = GetCarNames(tours[i]);
		if (static_cast<int>(cars.size()) == 0)
		{
			invalid = true;
			break;
		}
		for (int j = 0; j < static_cast<int>(cars.size()); ++j)
		{
			std::vector<int> params = GetCarParameters(cars[j]);
			if (static_cast<int>(params.size()) != CarModifiers::last)
			{
				invalid = true;
				break;
			}
			for (int j = 0; j < CarModifiers::last; ++j)
			{
				if (params[j] < 1)
				{
					invalid = true;
					break;
				}
			}
		}
	}
	if (invalid)
	{
		MessageBox(0, (FolderName::car + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		return false;
	}
	return true;
}
bool ToT_Window::ValidateTireFiles()
{
	std::vector<std::string> tires = GetTireNames();
	bool invalid = false;
	int x = 0;
	int y = 0;

	if (static_cast<int>(tires.size()) == 0)
	{
		invalid = true;
	}
	for (int i = 0; i < static_cast<int>(tires.size()); ++i)
	{
		std::vector<std::string> params = GetTireParameters(tires[i]);
		if (static_cast<int>(params.size()) != TireModifiers::last)
		{
			invalid = true;
			break;
		}
		for (int j = 0; j < TireModifiers::last; ++j)
		{
			for (int k = 0; k < static_cast<int>(params[j].size()); ++k)
			{
				if (params[j][k] == 'x')
				{
					x = static_cast<int>(atoi(params[j].substr(0, k).c_str()));
					y = static_cast<int>(atoi(params[j].substr(k + 1, static_cast<int>(params[j].size()) - k - 1).c_str()));
				}
			}
		}
		if (x*y == 0 || x > y)
		{
			invalid = true;
			break;
		}
	}
	if (invalid)
	{
		MessageBox(0, (FolderName::tire + " " + ErrorMsg::corrupted_file).c_str(), ErrorTitle::corrupted_file.c_str(), 0);
		return false;
	}
	return true;
}
std::vector<std::string> ToT_Window::ReadFile(const std::string path)
{
	std::vector<std::string> data;
	std::fstream fvar;
	std::string helper;
	fvar.open(path);
	while (getline(fvar, helper) && helper != "")
	{
		data.push_back(std::move(helper));
	}
	fvar.close();
	return data;
}
bool ToT_Window::SaveFileNames(std::string src_path, std::string dst_path, const std::string ext)
{
	dst_path = "dir " + src_path + "\\*." + ext + " > " + dst_path + " /b";
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
	std::vector<std::string> data = ReadFile(FolderName::car + "\\" + path);
	std::vector<int> car_parameters;
	for (int i = 0; i < static_cast<int>(data.size()); ++i)
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
	fvar.close();
}
void ToT_Window::Title(const COORD starting_point, const Text::TextAlign text_align)
{
	const COORD orientation_point = { starting_point.X - static_cast<short>((float)text_align / 2 * VectorOfStrings::title_main[0].size()), starting_point.Y };
	const short decoration_distance = 5;
	const std::string main_decoration = "{ }";
	const std::string additional_decoration = "*";
	//Main text
	SetConsoleTextAttribute(window_handle, color2);
	for (short i = 0; i < static_cast<short>(VectorOfStrings::title_main.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X, orientation_point.Y + i });
		std::cout << VectorOfStrings::title_main[i];
	}
	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < static_cast<short>(VectorOfStrings::title_additional.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X + static_cast<short>(VectorOfStrings::title_main[i].size() / 2 - VectorOfStrings::title_additional[i].size() / 2), orientation_point.Y + i + static_cast<short>(VectorOfStrings::title_main.size()) / 3 });
		std::cout << VectorOfStrings::title_additional[i];
	}
	//Decoration
	SetConsoleTextAttribute(window_handle, color2);
	for (short i = 0; i < static_cast<short>(VectorOfStrings::title_main.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X - static_cast<short>(decoration_distance + main_decoration.size()) - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
		SetConsoleCursorPosition(window_handle, { orientation_point.X + static_cast<short>(VectorOfStrings::title_main[i].size() + decoration_distance - 1) - i % 2, orientation_point.Y + i });
		std::cout << main_decoration;
	}
	SetConsoleTextAttribute(window_handle, color1);
	for (short i = 0; i < static_cast<short>(VectorOfStrings::title_main.size()); ++i)
	{
		SetConsoleCursorPosition(window_handle, { orientation_point.X - static_cast<short>(decoration_distance + main_decoration.size()) + 1 - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
		SetConsoleCursorPosition(window_handle, { orientation_point.X + static_cast<short>(VectorOfStrings::title_main[i].size() + decoration_distance - 1) + 1 - i % 2, orientation_point.Y + i });
		std::cout << additional_decoration;
	}
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
void ToT_Window::SetMusic(float volume)
{
	this->music_volume = volume;
	if (volume)
	{
		wav_transformer->ChangeVolume(volume);
		wav_transformer->Start(SND_ASYNC | SND_LOOP);
	}
	else
	{
		wav_transformer->Stop();
	}
}
