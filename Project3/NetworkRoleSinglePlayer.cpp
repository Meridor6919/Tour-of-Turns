#include "NetworkRole.h"

SinglePlayer::SinglePlayer(ToT_Window& main_window)
{
	this->main_window = &main_window;

	const COORD infobox_position = { 0,static_cast<short>(this->main_window->GetCharactersPerColumn() - 12) };
	InfoBoxDesc infobox_info = { 10, {1, static_cast<short>(this->main_window->GetCharactersPerColumn() - 18), 85, static_cast<short>(this->main_window->GetCharactersPerColumn() - 2)}, 2 };
	this->infobox = std::make_shared<InfoBox>(infobox_info, *this->main_window->GetWindowInfo());

	participants.push_back(Participant(infobox.get()));
	initiazlized = true;
}
bool SinglePlayer::isInit()
{
	return initiazlized;
}
int SinglePlayer::NumericalSelection(const COORD coords)
{
	const HANDLE window = main_window->GetHandle();
	char button;
	int value = 0;
	short decimal_position = 0;

	std::cout << ": ";
	while (true)
	{
		button = Text::Button(&timer_running, std::chrono::milliseconds(20));
		mutex.lock();
		SetColor(window, *main_window->main_color);
		SetConsoleCursorPosition(window, { coords.X + 2 + decimal_position, coords.Y });
		if (button >= '0' && button <= '9')
		{
			if (button == '0' && decimal_position == 0)
			{
				mutex.unlock();
				continue;
			}
			else if (value * 10 + button - 48 > participants[0].car_modifiers[CarAttributes::max_accelerating + take_action_position])
			{
				mutex.unlock();
				continue;
			}
			std::cout << button;
			value = value * 10 + button - 48;
			decimal_position++;
		}
		else if (button == '\b' && decimal_position != 0)
		{
			std::cout << "\b \b";
			value /= 10;
			decimal_position--;
		}
		else if (button == 13)
		{
			mutex.unlock();
			break;
		}
		mutex.unlock();
		if (value > 0)
		{
			ShowChances(value * (static_cast<bool>(take_action_position) ? -1 : 1));
		}
		else
		{
			ShowChances(0, true);
		}

	}
	mutex.lock();
	SetConsoleCursorPosition(window, coords);
	std::cout << Spaces(decimal_position + 2);
	mutex.unlock();
	ShowChances(0, true);
	return value;
}
int SinglePlayer::BinarySelection(const COORD coords)
{
	const HANDLE window = main_window->GetHandle();
	char button;

	while (true)
	{
		ShowChances(participants[0].car_modifiers[CarAttributes::hand_brake_value] * -1 * static_cast<int>(take_action_position == 2), take_action_position == 4);
		mutex.lock();
		SetConsoleCursorPosition(window, coords);
		SetColor(window, *main_window->main_color);
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::action_confirm];
		mutex.unlock();
		button = Text::Button(&timer_running, std::chrono::milliseconds(20));

		mutex.lock();
		SetConsoleCursorPosition(window, coords);
		std::cout << Spaces(static_cast<int>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::action_confirm].size()));
		mutex.unlock();
		ShowChances(0, true);
		if (button == 'y' || button == 'Y')
		{
			return take_action_position;
		}
		else if (button == 13 || button == 27 || button == 'n' || button == 'N')
		{
			return 0;
		}

	}
}
std::string SinglePlayer::StringSelection(const std::string current_name, const int max_name_size, const COORD coords)
{
	const HANDLE window = main_window->GetHandle();
	std::string name = current_name;
	int name_size = static_cast<int>(name.size());
	char button;

	auto show_name_func = [&]() {
		mutex.lock();
		SetConsoleCursorPosition(window, coords);
		SetColor(window, *main_window->main_color);
		std::cout << " < ";
		SetColor(window, *main_window->secondary_color);
		std::cout << name;
		SetColor(window, *main_window->main_color);
		std::cout << " >  ";
		SetColor(window, *main_window->secondary_color);
		mutex.unlock();
	};
	show_name_func();
	do
	{
		button = Text::Button(&timer_running, std::chrono::milliseconds(20));
		if (name_size < max_name_size && ((button >= 48 && button <= 57) || (button >= 65 && button <= 90 && GetKeyState(VK_SHIFT) != 1 && GetKeyState(VK_SHIFT) != 0) || (button >= 97 && button <= 122)))
		{
			name += button;
			show_name_func();
		}
		else if (name_size < max_name_size && button == 32 && name_size > 0 && name[name_size - 1] != 32)
		{
			name += ' ';
			show_name_func();;
		}
		else if (button == 8 && name_size > 0)
		{
			name.erase(name_size - 1, 1);
			show_name_func();
		}
		name_size = static_cast<int>(name.size());
	} while (button != 13);
	mutex.lock();
	SetConsoleCursorPosition(window, coords);
	std::cout << Spaces(name_size + 5);
	mutex.unlock();
	if (name_size == 0)
	{
		name = GameConstants::default_racer_name;
	}
	return name;
}
void SinglePlayer::ShowCarParameters(const std::string car_path, const bool clear, const COORD starting_pos)
{
	const std::vector<int> car_params = GetCarParameters(car_path);
	std::vector < std::pair<std::string, std::string>> text;

	for (short i = 0; i < static_cast<short>(car_params.size()); ++i)
	{
		text.push_back(std::make_pair(LanguagePack::text[LanguagePack::car_attributes][i], std::to_string(car_params[i])));
	}
	ShowLobbyInformation(LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::player_information_title], text, starting_pos, 1, 2, clear);
}
void SinglePlayer::ShowTiresParameters(const std::string tire_path, bool clear, const COORD starting_pos)
{
	const std::vector<std::string> tire_params = GetTireParameters(tire_path);
	std::vector < std::pair<std::string, std::string>> text;

	for (short i = 0; i < static_cast<short>(tire_params.size()); ++i)
	{
		text.push_back(std::make_pair(LanguagePack::text[LanguagePack::terrain_types][i], tire_params[i]));
	}
	ShowLobbyInformation("", text, starting_pos, 1, 2, clear);
}
void SinglePlayer::ShowTourParameters(const std::string tour_path, bool clear)
{
	ShowRankingParameters(tour_path.substr(0, static_cast<int>(tour_path.size()) - static_cast<int>(ExtName::tour.size())) + ExtName::ranking, clear);
	const std::vector<std::string> tour_params = GetTourParameters(tour_path, 0, INT_MAX);
	int segment_quantity[6] = { 0,0,0,0,0,0 };
	int turns = 0;

	for (short i = 0; i < static_cast<short>(tour_params.size()); ++i)
	{
		++segment_quantity[tour_params[i][0] - 48];
		if (tour_params[i].size() > 1)
		{
			++turns;
		}
	}
	std::vector<std::pair<std::string, std::string>> text = { {LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::length_of_tour], std::to_string(static_cast<int>(tour_params.size()))}, {LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::number_of_turns], std::to_string(static_cast<int>(static_cast<float>(turns) / static_cast<float>(tour_params.size()) * 100.f)) + "%"} };
	for (short i = 0; i < 6; ++i)
	{
		text.push_back(std::make_pair(LanguagePack::text[LanguagePack::terrain_types][i], std::to_string(static_cast<int>(static_cast<float>(segment_quantity[i]) / static_cast<float>(tour_params.size()) * 100.f)) + '%'));
	}
	ShowLobbyInformation("", text, { static_cast<short>(main_window->GetCharactersPerRow()) - static_cast<short>(Validation::box_width), 30 }, 1, 2, clear);
}
void SinglePlayer::ShowRankingParameters(const std::string ranking_path, bool clear)
{
	std::ifstream fvar;
	std::string temp;
	std::string best_name = "", local_name;
	int best_place = 0, local_place;
	float winrate = 0;
	int best_games_in_total = 0;
	int local_won_games;
	int local_games_in_total;
	int classification = 0 + (main_window->GetAIs() == 7);
	fvar.open(FolderName::ranking + '\\' + ranking_path);
	for (int i = 0; std::getline(fvar, temp); ++i)
	{
		if (i % Validation::ranking_details == 0)
		{
			local_name = temp;
		}
		else if (i % Validation::ranking_details == 1)
		{
			local_games_in_total = atoi(GetSeparatedValue(temp, classification).c_str());
		}
		else if (i % Validation::ranking_details == 2)
		{
			local_won_games = atoi(GetSeparatedValue(temp, classification).c_str());
		}
		else if (i % Validation::ranking_details == 3)
		{
			local_place = atoi(GetSeparatedValue(temp, classification).c_str());
		}
		else if (i % Validation::ranking_details == 6)
		{
			int number_of_finished_games = local_games_in_total - atoi(GetSeparatedValue(temp, classification).c_str());
			if (!number_of_finished_games)
			{
				continue;
			}
			if (winrate < static_cast<float>(local_won_games) / static_cast<float>(local_games_in_total) * 100.0f)
			{
				winrate = static_cast<float>(local_won_games) / static_cast<float>(local_games_in_total) * 100.0f;
				best_name = local_name;
				best_games_in_total = local_games_in_total;
				best_place = local_place / number_of_finished_games;
			}
		}
	}
	fvar.close();

	const std::vector<std::pair<std::string, std::string>> vector = { {LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::current_champion], best_name},
		{LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::win_ratio],std::to_string(winrate).substr(0, static_cast<int>(std::to_string(winrate).size()) - 4) + '%'},
		{LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::games_in_total], std::to_string(best_games_in_total)},
		{LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::average_place], std::to_string(best_place)} };
	ShowLobbyInformation(LanguagePack::text[LanguagePack::tour_information][GameLobbyInformations::tour_information_title], vector, { static_cast<short>(main_window->GetCharactersPerRow()) - static_cast<short>(Validation::box_width), 19 }, 1, 2, clear);
}
void SinglePlayer::ShowLobbyInformation(const std::string title, const std::vector<std::pair<std::string, std::string>> text, const COORD base_position, const short paragraph_size, const short spacing, const bool clear)
{
	const HANDLE handle = main_window->GetHandle();
	mutex.lock();
	if (clear)
	{
		const int border_size = Validation::box_width;
		if (title != "")
		{
			SetConsoleCursorPosition(handle, { base_position.X, base_position.Y });
			std::cout << Spaces(border_size);
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout << Spaces(border_size);
		for (short i = 0; i < static_cast<short>(text.size()); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			std::cout << Spaces(static_cast<short>(text[i].first.size()) + static_cast<short>(text[i].second.size()) + 2);
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(text.size()) + 2) });
		std::cout << Spaces(border_size);
	}
	else
	{
		if (title != "")
		{
			SetColor(handle, *main_window->secondary_color);
			SetConsoleCursorPosition(handle, { base_position.X + static_cast<short>(Validation::box_width) / 2 - static_cast<short>(title.size()) / 2, base_position.Y });
			std::cout << title;
		}
		SetColor(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout << GetMonoCharacterString(Validation::box_width, '_');
		for (short i = 0; i < static_cast<short>(text.size()); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			SetColor(handle, *main_window->main_color);
			std::cout << text[i].first + ": ";
			SetColor(handle, *main_window->secondary_color);
			std::cout << text[i].second;
		}
		SetColor(handle, *main_window->secondary_color);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(text.size()) + 2) });
		std::cout << GetMonoCharacterString(Validation::box_width, '_');
	}
	mutex.unlock();
}
void SinglePlayer::ShowLeaderboard(const std::vector<std::string> text, short pos, Color color, const bool clear)
{
	mutex.lock();
	SetColor(main_window->GetHandle(), color);
	for (int j = 0; j < static_cast<int>(text.size()); ++j)
	{
		const short x_pos = static_cast<short>(main_window->GetCharactersPerRow() - (16 * static_cast<int>(text.size())) + 16 * j) - static_cast<short>(static_cast<float>(TextAlign::center) / 2.0f * static_cast<float>(text[j].size()));
		const short y_pos = 16 + pos * 2;
		SetConsoleCursorPosition(main_window->GetHandle(), { x_pos, y_pos });
		if (clear)
		{
			std::cout << Spaces(static_cast<int>(text[j].size()));
		}
		else
		{
			std::cout << text[j];
		}
	}
	mutex.unlock();
}
void SinglePlayer::ShowLoading(std::string title, int& init, int condition, bool clear)
{
	const COORD loading_screen_postion = { static_cast<short>(main_window->GetCharactersPerRow()) / 2, static_cast<short>(main_window->GetCharactersPerColumn()) / 2 - 5 };
	const HANDLE handle = main_window->GetHandle();
	const short title_length = static_cast<short>(title.size());
	if (clear)
	{
		mutex.lock();
		SetConsoleCursorPosition(handle, { loading_screen_postion.X - title_length / 2, loading_screen_postion.Y });
		for (int i = 0; i < title_length; ++i)
		{
			std::cout << ' ';
		}
		for (short i = 0; i < 3; ++i)
		{
			SetConsoleCursorPosition(handle, { loading_screen_postion.X - 1, loading_screen_postion.Y + 2 + i });
			std::cout << "   ";
		}
		mutex.unlock();
		return;
	}
	else
	{
		int iteration = 0;
		while (init != condition)
		{
			std::string loading_wheel = GetMonoCharacterString(8, 'o');
			iteration = (iteration + 1) % 8;
			for (int i = 0; i < 4; ++i)
			{
				loading_wheel[(i + iteration) % 8] = ' ';
			}
			mutex.lock();
			SetConsoleCursorPosition(handle, { loading_screen_postion.X - title_length / 2, loading_screen_postion.Y });
			SetColor(handle, *main_window->main_color);
			std::cout << title;
			SetColor(handle, *main_window->secondary_color);
			{
				SetConsoleCursorPosition(handle, { loading_screen_postion.X - 1, loading_screen_postion.Y + 2 });
				std::cout << loading_wheel.substr(0, 3);
				SetConsoleCursorPosition(handle, { loading_screen_postion.X - 1, loading_screen_postion.Y + 3 });
				std::cout << loading_wheel[7] << ' ' << loading_wheel[3];
				SetConsoleCursorPosition(handle, { loading_screen_postion.X - 1, loading_screen_postion.Y + 4 });
				std::cout << loading_wheel[6] << loading_wheel[5] << loading_wheel[4];
			}
			mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}
void SinglePlayer::ShowChances(const int value, const bool reset)
{
	float speed_estimation = (participants[0].current_speed + static_cast<float>(value) * (0.9f + 0.2f * participants[0].TireEffectivness(current_field)));
	if (speed_estimation > static_cast<float>(participants[0].car_modifiers[CarAttributes::max_speed]) * 1.25f)
	{
		speed_estimation = static_cast<float>(participants[0].car_modifiers[CarAttributes::max_speed]) * 1.25f;
	}
	else if (speed_estimation < 0)
	{
		speed_estimation = 0;
	}
	float burned_durability = participants[0].CalculateBurning(speed_estimation);
	speed_estimation *= GameConstants::friction_scalar;
	bool drift = take_action_position == 2 && participants[0].current_speed > 40.0f && current_field.size() > 1;
	float chance_to_succeed = static_cast<float>(static_cast<int>((100.0f - participants[0].EvaluateChance(current_field, speed_estimation, drift))));
	float estimated_time = drift ? GameConstants::drift_value : 100.0f / (1.0f + speed_estimation * 10.0f / 36.0f);

	HANDLE window = main_window->GetHandle();
	std::string helper;
	std::vector<std::pair<double, std::string>> values = { {chance_to_succeed, LanguagePack::text[LanguagePack::race_chancebox][0]},  {estimated_time, LanguagePack::text[LanguagePack::race_chancebox][1]},
														{burned_durability, LanguagePack::text[LanguagePack::race_chancebox][2]}, {speed_estimation, LanguagePack::text[LanguagePack::race_chancebox][3]} };
	mutex.lock();
	if (!reset)
	{
		for (short i = 0; i < values.size(); ++i)
		{
			if (values[i].first < 0.01)
			{
				values[i].first = 0;
			}
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetCharactersPerRow() - 51), static_cast<short>(main_window->GetCharactersPerColumn() - 28 + i) });
			SetColor(window, *main_window->main_color);
			std::cout << values[i].second;
			SetColor(window, *main_window->secondary_color);
			helper = std::to_string(values[i].first);
			helper = helper.substr(0, static_cast<int>(helper.size()) - 4);
			std::cout << helper << "              ";
		}
	}
	else
	{
		for (short i = 0; i < values.size(); ++i)
		{
			SetColor(window, *main_window->main_color);
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetCharactersPerRow() - 51), static_cast<short>(main_window->GetCharactersPerColumn() - 28 + i) });
			std::cout << values[i].second << "                   ";
		}
	}
	mutex.unlock();
}
void SinglePlayer::ShowIndicator(int participant, bool clear)
{
	const COORD coord = { static_cast<short>(main_window->GetCharactersPerRow()) - 55, 16 };
	mutex.lock();
	if (clear)
	{
		SetConsoleCursorPosition(main_window->GetHandle(), { coord.X, coord.Y + static_cast<short>(participants[participant].place) * 2 });
		std::cout << ' ';
	}
	else
	{
		SetConsoleCursorPosition(main_window->GetHandle(), { coord.X, coord.Y + static_cast<short>(participants[participant].place) * 2 });
		SetColor(main_window->GetHandle(), participants[participant].action_performed ? *main_window->secondary_color : Color::dark_gray);
		std::cout << '*';
	}
	mutex.unlock();

}
void SinglePlayer::GetParticipants(const std::string name, const std::string tour, const std::string car, const std::string tire)
{
	const int number_of_ais = main_window->GetAIs();

	this->tour = tour;
	participants[0].name = name;
	participants[0].car_path = car;
	participants[0].tire_path = tire;

	for (int i = 0; i < number_of_ais; ++i)
	{
		participants.push_back(Participant(infobox.get()));
	}
	if (number_of_ais)
	{
		ai_connector = std::make_unique<PipeConnector>(FolderName::ai + '\\' + main_window->GetAIModule(), 255);
		if (!ai_connector->HandleConnection(&SinglePlayer::HandleAIConnection, this))
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			return;
		}
	}
	ShowLoading(LanguagePack::text[LanguagePack::other_strings][OtherStrings::loadingai_title], ai_init, main_window->GetAIs());
	ShowLoading(LanguagePack::text[LanguagePack::other_strings][OtherStrings::loadingai_title], ai_init, main_window->GetAIs(), true);
}
void SinglePlayer::SortLeaderboard()
{
	//ugly quadratic complexity but it is justified by the fact that n is at most 8
	for (int i = 0; i < participants.size(); ++i)
	{
		participants[i].place = 1;
		for (int j = 0; j < participants.size(); ++j)
		{
			if (participants[i].score > participants[j].score && participants[j].IsAlive())
			{
				++participants[i].place;
			}
			else if (participants[i].score == participants[j].score && i > j && participants[j].IsAlive())
			{
				++participants[i].place;
			}
		}
		if (!participants[i].IsAlive())
		{
			participants[i].place = 9;
		}
	}
}
void SinglePlayer::GetCurrentAttributes()
{
	auto seconds_per_turn = std::chrono::seconds(main_window->GetTimerSettings() * 10);
	timer->SetTimer(seconds_per_turn, &timer_running);
	for (int i = static_cast<int>(participants.size()) - 1; i >= 0; --i)
	{
		if (participants[i].IsAlive())
		{
			mutex.lock();
			participants[i].Test(current_field, i < static_cast<int>(participants.size()) - main_window->GetAIs());
			mutex.unlock();
		}
	}
	mutex.lock();
	infobox->Draw();
	mutex.unlock();
}
std::string SinglePlayer::GetTour()
{
	return tour;
}
bool SinglePlayer::GameLobby()
{
	const HANDLE handle = main_window->GetHandle();
	COORD starting_point = { static_cast<short>(main_window->GetCharactersPerRow()) / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	std::string name = main_window->GetName();
	int timer_settings = main_window->GetTimerSettings();
	std::vector<std::string> tours = GetTourNames();
	std::vector<std::string> tires = GetTireNames();
	std::vector<std::string> cars = GetCarNames(tours[0] + ExtName::tour);
	int ais = main_window->GetAIs();
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;

	ShowCarParameters(cars[cars_pos] + ExtName::car);
	ShowTiresParameters(tires[tires_pos] + ExtName::tire);
	ShowTourParameters(tours[tours_pos] + ExtName::tour);

	while (true)
	{
		Text::TextInfo text_info(LanguagePack::text[LanguagePack::game_options], main_menu_position, starting_point, TextAlign::center, spacing, false);
		MultithreadingData  multithreading_data = { &mutex, &timer_running };
		main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo(), multithreading_data);
		if (!timer_running)
		{
			break;
		}
		const COORD starting_local_pos = { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::game_options][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing };
		switch (main_menu_position)
		{
		case 0://choosing name
		{
			name = StringSelection(name, Validation::maximum_name_length, { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::game_options][0].size()), 25 });
			main_window->SetName(name);
			break;
		}
		case 1://Number of ais
		{
			std::vector<std::string> text;

			for (int i = 0; i <= Possible_AIs(); ++i)
			{
				text.push_back(std::to_string(i));
			}
			Text::TextInfo text_info(text, ais, starting_local_pos, TextAlign::left, 0, true);
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			ais = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			ShowRankingParameters(tours[tours_pos] + ExtName::ranking, true);
			main_window->SetAIs(ais);
			ShowRankingParameters(tours[tours_pos] + ExtName::ranking);
			break;
		}
		case 2: //timer
		{
			std::vector<std::string> timer_values = { LanguagePack::text[LanguagePack::on_off][1] };
			for (int i = 1; i <= Validation::maximum_timer; ++i)
			{
				timer_values.push_back((i < 60 ? "0" : "") + std::to_string(i / 6) + ':' + std::to_string(i % 6) + '0');
			}
			Text::TextInfo text_info(timer_values, timer_settings, starting_local_pos, TextAlign::left, 0, true);
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			timer_settings = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			main_window->SetTimerSettings(timer_settings);
			break;
		}
		case 3://choosing tour
		{
			int i = tours_pos;
			Text::TextInfo text_info(tours, tours_pos, starting_local_pos, TextAlign::left, 0, true);
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			tours_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			if (i != tours_pos)
			{
				ShowCarParameters(cars[cars_pos] + ExtName::car, true);
				cars = GetCarNames(tours[tours_pos] + ExtName::tour);
				cars_pos = 0;
				ShowCarParameters(cars[cars_pos] + ExtName::car);
				ShowTourParameters(tours[i] + ExtName::tour, true);
				ShowTourParameters(tours[tours_pos] + ExtName::tour);
			}

			break;
		}
		case 4://choosing car
		{
			int i = cars_pos;
			Text::TextInfo text_info(cars, cars_pos, starting_local_pos, TextAlign::left, 0, true);
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			cars_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			if (i != cars_pos)
			{
				ShowCarParameters(cars[i] + ExtName::car, true);
				ShowCarParameters(cars[cars_pos] + ExtName::car);
			}
			break;
		}
		case 5://choosing tires
		{
			int i = tires_pos;
			Text::TextInfo text_info(tires, tires_pos, starting_local_pos, TextAlign::left, 0, true);
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			tires_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			if (i != tires_pos)
			{
				ShowTiresParameters(tires[i] + ExtName::tire, true);
				ShowTiresParameters(tires[tires_pos] + ExtName::tire);
			}
			break;
		}
		case 7://Back
		{
			SaveGameConfig(main_window->GetToTGameConfig());
			return false;
		}
		}
		if (main_menu_position == 6)//Next
		{
			break;
		}
	}
	for (int i = 0; i < static_cast<int>(LanguagePack::text[LanguagePack::game_options].size()); ++i)
	{
		mutex.lock();
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::text[LanguagePack::game_options][i].size())), starting_point.Y + static_cast<short>(i * spacing) });
		std::cout << Spaces(static_cast<int>(LanguagePack::text[LanguagePack::game_options][i].size()));
		mutex.unlock();
	}
	SaveGameConfig(main_window->GetToTGameConfig());
	ShowTiresParameters(tires[tires_pos] + ExtName::tire, true);
	ShowCarParameters(cars[cars_pos] + ExtName::car, true);
	ShowTourParameters(tours[tours_pos] + ExtName::tour, true);
	GetParticipants(name, tours[tours_pos] + ExtName::tour, cars[cars_pos] + ExtName::car, tires[tires_pos] + ExtName::tire);
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		participants[i].Init(tour);
	}
	if (timer_settings)
	{
		COORD coord = { 1,1 };
		timer_multithreading_data = { &mutex, &timer_working, std::chrono::milliseconds(50) };
		timer = std::make_unique<VisibleTimer>(coord, main_window->GetWindowInfo(), &timer_multithreading_data);
		auto seconds_per_turn = std::chrono::seconds(main_window->GetTimerSettings() * 10);
		timer_working = true;
		timer->SetTimer(seconds_per_turn, &timer_running);
		timer->StartShowingTimer();
	}
	SortLeaderboard();
	return true;
}
void SinglePlayer::AttackPhase()
{
	ValidateAttack(PerformAttack(), 0);
}
void SinglePlayer::ActionPhase()
{
	ValidateAction(PerformAction(), 0);
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		while (participants[i].IsAlive() && !participants[i].action_performed)
		{
			if (!timer_running)
			{
				if (participants[i].current_speed > 0.f)
				{
					ValidateAction(std::make_pair<int>(3, 0), i);
				}
				else
				{
					ValidateAction(std::make_pair<int>(4, 0), i);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}
}
void SinglePlayer::ValidateAttack(int target, int participant)
{
	if (participants[participant].IsAlive())
	{
		if (target < static_cast<int>(participants.size()))
		{
			if (participants[target].score < participants[participant].score + GameConstants::attack_backward_distance &&
				participants[target].score > participants[participant].score - GameConstants::attack_forward_distance &&
				participants[target].IsAlive() &&
				!participants[participant].attack_performed &&
				target != participant)
			{
				participants[target].attacked += 1;
				participants[participant].attacked += 0.5f;
				participants[participant].attack_performed = true;
			}
			else
			{
				mutex.lock();
				participants[participant].KillParticipant();
				mutex.unlock();
				MessageBox(0, (participants[participant].name + ErrorMsg::cheating_attempt).c_str(), ErrorTitle::cheating_attempt, MB_TOPMOST);
				return;
			}
		}
	}
}
void SinglePlayer::ValidateAction(std::pair<int, int> action, int participant)
{
	if (participants[participant].IsAlive())
	{
		if (action.first == 4)
		{
			mutex.lock();
			participants[participant].KillParticipant();
			participants[participant].action_performed = true;
			mutex.unlock();
			ShowIndicator(participant);
			return;
		}
		if ((action.first == 0 && (action.second > participants[participant].car_modifiers[CarAttributes::max_accelerating] || action.second <= 0)) ||
			(action.first == 1 && (action.second < participants[participant].car_modifiers[CarAttributes::max_braking] * -1 || action.second >= 0 || participants[participant].current_speed <= 0)) ||
			(action.first == 2 && (action.second != participants[participant].car_modifiers[CarAttributes::hand_brake_value] * -1 || participants[participant].current_speed <= 0)) ||
			(action.first == 3 && (action.second != 0 || participants[participant].current_speed <= 0)) ||
			participants[participant].action_performed)
		{
			mutex.lock();
			participants[participant].KillParticipant();
			mutex.unlock();
			MessageBox(0, (participants[participant].name + ErrorMsg::cheating_attempt).c_str(), ErrorTitle::cheating_attempt, MB_TOPMOST);
			return;
		}
		if (action.first == 2 && participants[participant].current_speed >= GameConstants::minimum_drift_speed)
		{
			participants[participant].drift = true;
		}
		participants[participant].QueueAction(static_cast<float>(action.second), current_field);
		participants[participant].action_performed = true;
		ShowIndicator(participant);
	}
	else
	{
		participants[participant].action_performed = true;
		ShowIndicator(participant, true);
	}

}
int SinglePlayer::PerformAttack()
{
	attack_phase = true;
	if (participants[0].IsAlive())
	{
		std::vector<std::string> rival_name = { LanguagePack::text[LanguagePack::other_strings][OtherStrings::dont_attack] };
		std::vector<int> rival_id = { 10 };

		for (int i = 1; i < static_cast<int>(participants.size()); ++i)
		{
			if (participants[i].score < participants[0].score + GameConstants::attack_backward_distance && participants[i].score >participants[0].score - GameConstants::attack_forward_distance && participants[i].IsAlive())
			{
				rival_name.push_back(participants[i].name);
				rival_id.push_back(i);
			}
		}
		if (static_cast<int>(rival_id.size()) != 1)
		{
			Text::TextInfo text_info(rival_name, 0, { static_cast<short>(main_window->GetCharactersPerRow() - 28), static_cast<short>(main_window->GetCharactersPerColumn() - 17) }, TextAlign::center, 2, true);
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			short i = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			return rival_id[i];
		}
	}
	return 10;
}
void SinglePlayer::HandleAIConnection(std::string msg_received)
{
	const int msg_length = static_cast<int>(msg_received.size());
	if (msg_length < 2)
	{
		return;
	}
	const int code = atoi(msg_received.substr(0, 2).c_str());
	const std::string msg = msg_received.substr(2, msg_length - 2);
	switch (code)
	{
	case ConnectionCodes::Start:
	{
		mutex.lock();
		if (ai_connector)
		{
			ai_connector->Write(std::to_string(code));
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::GetInit:
	{
		mutex.lock();
		if (ai_connector)
		{
			ai_connector->Write(std::to_string(main_window->GetAIs()));
			ai_connector->Write(std::to_string(static_cast<int>(participants.size())));
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::NewTurn:
	{
		if (msg_length < 3)
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		const int ai_position_start = static_cast<int>(participants.size()) - main_window->GetAIs();
		const int id = msg_received[2] - 48 + ai_position_start;
		if (id < ai_position_start || id >= static_cast<int>(participants.size()))
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		mutex.lock();
		if (ai_connector)
		{
			ai_connector->Write(std::to_string(participants[id].action_performed));
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::GetCarNames:
	{
		const std::vector<std::string> car_names = GetCarNames(tour);
		const int car_names_size = static_cast<int>(car_names.size());
		mutex.lock();
		if (ai_connector)
		{
			ai_connector->Write(std::to_string(car_names_size));
			for (int i = 0; i < car_names_size; ++i)
			{
				ai_connector->Write(car_names[i]);
			}
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::GetTireNames:
	{
		const std::vector<std::string> tire_names = GetFilesInDirectory(FolderName::tire);
		const int tire_names_size = static_cast<int>(tire_names.size());
		mutex.lock();
		if (ai_connector)
		{
			ai_connector->Write(std::to_string(tire_names_size));
			for (int i = 0; i < tire_names_size; ++i)
			{
				ai_connector->Write(tire_names[i]);
			}
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::GetCarParams:
	{
		bool valid = false;
		const std::vector<std::string> car_names = GetCarNames(tour);
		for (int i = 0; i < static_cast<int>(car_names.size()); ++i)
		{
			if (msg == car_names[i])
			{
				valid = true;
				break;
			}
		}
		if (!valid)
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		const std::vector<int> car_param = GetCarParameters(msg + ExtName::car);
		mutex.lock();
		if (ai_connector)
		{
			for (int i = 0; i < static_cast<int>(car_param.size()); ++i)
			{
				ai_connector->Write(std::to_string(car_param[i]));
			}
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::GetTireParams:
	{
		bool valid = false;
		const std::vector<std::string> tire_names = GetFilesInDirectory(FolderName::tire);
		for (int i = 0; i < static_cast<int>(tire_names.size()); ++i)
		{
			if (msg == tire_names[i])
			{
				valid = true;
				break;
			}
		}
		if (!valid)
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		const std::vector<std::string> tires_param = GetTireParameters(msg);
		mutex.lock();
		if (ai_connector)
		{
			for (int i = 0; i < static_cast<int>(tires_param.size()); ++i)
			{
				ai_connector->Write(tires_param[i]);
			}
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::GetAllAttributes:
	{
		mutex.lock();
		if (ai_connector)
		{
			for (int i = 0; i < static_cast<int>(participants.size()); ++i)
			{
				ai_connector->Write(std::to_string(participants[i].current_speed));
				ai_connector->Write(std::to_string(participants[i].current_durability));
				ai_connector->Write(std::to_string(participants[i].score));
			}
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::GetTour:
	{
		const std::vector<std::string> tour_param = GetTourParameters(tour, 0, INT_MAX);
		const int tour_size = static_cast<int>(tour_param.size());
		mutex.lock();
		if (ai_connector)
		{
			ai_connector->Write(std::to_string(tour_size));
			for (int i = 0; i < tour_size; ++i)
			{
				ai_connector->Write(tour_param[i]);
			}
		}
		mutex.unlock();
		break;
	}
	case ConnectionCodes::SetName:
	{
		const std::string selected_name = msg.substr(1, msg.size() - 1);
		const int ai_id = msg[0] - 48;
		//name validation
		const int selected_name_size = static_cast<int>(selected_name.size());
		if (selected_name_size > Validation::maximum_name_length || selected_name_size <= 0)
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		//ai id validation
		if (ai_id < 0 || ai_id > main_window->GetAIs())
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].name = selected_name;
		break;
	}
	case ConnectionCodes::SetCar:
	{
		bool valid = false;
		const std::vector<std::string> car_names = GetCarNames(tour);
		const std::string selected_car = msg.substr(1, msg.size() - 1);
		const int ai_id = msg[0] - 48;
		//car validation
		for (int i = 0; i < static_cast<int>(car_names.size()); ++i)
		{
			if (selected_car == car_names[i])
			{
				valid = true;
				break;
			}
		}
		if (!valid)
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		//ai id validation
		if (ai_id < 0 || ai_id > main_window->GetAIs())
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].car_path = selected_car + ExtName::car;
		break;
	}
	case ConnectionCodes::SetTires:
	{
		bool valid = false;
		const std::vector<std::string> tire_names = GetFilesInDirectory(FolderName::tire);
		const std::string selected_tires = msg.substr(1, msg.size() - 1);
		const int ai_id = msg[0] - 48;
		//car validation
		for (int i = 0; i < static_cast<int>(tire_names.size()); ++i)
		{
			if (selected_tires == tire_names[i])
			{
				valid = true;
				break;
			}
		}
		if (!valid)
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		//ai id validation
		if (ai_id < 0 || ai_id > main_window->GetAIs())
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].tire_path = selected_tires;
		++ai_init;
		break;
	}
	case ConnectionCodes::SetAction:
	{
		const int ai_id = msg[0] - 48;
		const int selected_action_id = msg[1] - 48;
		const int selected_action_value = atoi(msg.substr(2, msg.size() - 2).c_str());
		//ai id validation
		if (ai_id < 0 || ai_id > main_window->GetAIs())
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		//action pre-validation
		if (selected_action_id < 0 || selected_action_id > 4 || !participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].IsAlive())
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		ValidateAction({ selected_action_id, selected_action_value }, static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id);
		break;
	}
	case ConnectionCodes::SetAttack:
	{
		const int ai_id = msg[0] - 48;
		const int selected_target = atoi(msg.substr(1, msg.size() - 1).c_str());
		//ai id validation
		if (ai_id < 0 || ai_id > main_window->GetAIs() || !attack_phase)
		{
			MessageBox(0, ErrorMsg::ai_connection, ErrorTitle::ai_error, MB_TOPMOST);
			exit(0);
		}
		if (!participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].IsAlive())
		{
			break;
		}
		ValidateAttack(selected_target, static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id);
		break;
	}
	default:
	{
		OutputDebugString((msg_received + '\n').c_str());
		break;
	}
	}
}
std::pair<int, int> SinglePlayer::PerformAction()
{
	if (participants[0].IsAlive())
	{
		const HANDLE window = main_window->GetHandle();
		int value;
		while (true)
		{
			Text::TextInfo text_info(LanguagePack::text[LanguagePack::race_actions], take_action_position, { 1,39 }, TextAlign::left, 2, false);
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			take_action_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			if (!timer_running)
			{
				if (participants[0].current_speed == 0)
				{
					return std::make_pair<int>(4, 0);
				}
				return std::make_pair(3, 0);
			}
			if (participants[0].current_speed == 0 && take_action_position % 4 != 0)
			{
				mutex.lock();
				SetConsoleCursorPosition(window, { static_cast<short>(LanguagePack::text[LanguagePack::race_actions][take_action_position].size()) + 1, 39 + 2 * take_action_position });
				std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::unable_to_move];
				mutex.unlock();
				main_window->BlockingSleep(1500);
				mutex.lock();
				SetConsoleCursorPosition(window, { static_cast<short>(LanguagePack::text[LanguagePack::race_actions][take_action_position].size()) + 1, 39 + 2 * take_action_position });
				std::cout << Spaces(static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::unable_to_move].size()));
				mutex.unlock();
				continue;
			}
			if (take_action_position < 2)
			{
				value = NumericalSelection({ static_cast<short>(LanguagePack::text[LanguagePack::race_actions][take_action_position].size()) + 1, 39 + 2 * take_action_position });
				if (value != 0)
				{
					return std::make_pair(take_action_position, value * (take_action_position ? -1 : 1));
				}
			}
			else
			{
				int option = BinarySelection({ static_cast<short>(LanguagePack::text[LanguagePack::race_actions][take_action_position].size()) + 1, 39 + 2 * take_action_position });
				if (option)
				{
					return std::make_pair(take_action_position, participants[0].car_modifiers[CarAttributes::hand_brake_value] * -1 * (take_action_position == 2));
				}
			}
		}
	}
	return std::make_pair(4, 0);
}
int SinglePlayer::Possible_AIs()
{
	return 7;
}
void SinglePlayer::Leaderboard(const bool clear)
{
	ShowLeaderboard(LanguagePack::text[LanguagePack::race_leaderboard], 0, *main_window->secondary_color, clear);
	if (!clear)
	{
		for (int i = 0; i < static_cast<int>(participants.size()); ++i)
		{
			ShowIndicator(i, true);
		}
		SortLeaderboard();
		for (int i = 0; i < static_cast<int>(participants.size()); ++i)
		{
			if (participants[i].IsAlive())
			{
				participants[i].attack_performed = false;
				participants[i].action_performed = false;
				ShowIndicator(i);
			}
		}
	}
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		if (participants[i].IsAlive() || clear)
		{
			std::vector<std::string> leaderboard_info = { std::to_string(participants[i].place), participants[i].name, std::to_string(participants[i].score) };
			leaderboard_info[2] = leaderboard_info[2].substr(0, static_cast<int>(leaderboard_info[2].size()) - 4);
			ShowLeaderboard(leaderboard_info, static_cast<short>(participants[i].place), *main_window->main_color, clear);
		}
	}
}
void SinglePlayer::Interface()
{
	const HANDLE window = main_window->GetHandle();
	const std::string possible_actions[5] = {
		LanguagePack::text[LanguagePack::race_actions_descritions][0] + std::to_string(static_cast<int>(participants[0].car_modifiers[CarAttributes::max_accelerating])) + ")   ",
		LanguagePack::text[LanguagePack::race_actions_descritions][1] + std::to_string(static_cast<int>(participants[0].car_modifiers[CarAttributes::max_braking])) + ")   ",
		LanguagePack::text[LanguagePack::race_actions_descritions][2] + std::to_string(static_cast<int>(participants[0].car_modifiers[CarAttributes::hand_brake_value])) + "   ",
		LanguagePack::text[LanguagePack::race_actions_descritions][3],
		LanguagePack::text[LanguagePack::race_actions_descritions][4] };
	std::string speed = std::to_string(participants[0].current_speed);
	std::string durability = std::to_string(participants[0].current_durability);
	speed = speed.substr(0, static_cast<int>(speed.size()) - 4);
	durability = durability.substr(0, static_cast<int>(durability.size()) - 4);

	mutex.lock();
	SetColor(window, *main_window->main_color);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << LanguagePack::text[LanguagePack::race_player_informations][0];
	SetColor(window, *main_window->secondary_color);
	std::cout << participants[0].name;
	SetColor(window, *main_window->main_color);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << LanguagePack::text[LanguagePack::race_player_informations][1];
	SetColor(window, *main_window->secondary_color);
	std::cout << speed << "     ";
	SetColor(window, *main_window->main_color);
	SetConsoleCursorPosition(window, { 0, 20 });
	std::cout << LanguagePack::text[LanguagePack::race_player_informations][2];
	SetColor(window, *main_window->secondary_color);
	std::cout << durability << LanguagePack::text[LanguagePack::race_player_informations][3];
	SetColor(window, MeridorConsoleLib::Color::dark_gray);
	for (short i = 0; i < 5; ++i)
	{
		SetConsoleCursorPosition(window, { 2, 40 + i * 2 });
		std::cout << possible_actions[i];
	}
	mutex.unlock();
	const short x = static_cast<short>(main_window->GetCharactersPerRow() - static_cast<short>(Validation::box_width));
	const std::vector<std::tuple<std::string, COORD, short>> boxes = { {LanguagePack::text[LanguagePack::race_box_names][0], {0, static_cast<short>(main_window->GetCharactersPerColumn() - 15)}, 12 },
				{ LanguagePack::text[LanguagePack::race_box_names][1], { x, static_cast<short>(main_window->GetCharactersPerColumn() - 20) }, 17},
				{ LanguagePack::text[LanguagePack::race_box_names][2],  { x, static_cast<short>(main_window->GetCharactersPerColumn() - 31) }, 7 } };

	mutex.lock();
	SetColor(window, *main_window->secondary_color);
	infobox->DrawBox();
	/*
	for (int i = 0; i < static_cast<int>(boxes.size()); ++i)
	{
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X + static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size())/2 - static_cast<short>(std::get<0>(boxes[i]).size())/2, std::get<1>(boxes[i]).Y});
		std::cout << std::get<0>(boxes[i]);
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + 1});
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + std::get<2>(boxes[i]) + 2 });
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
	}
	*/
	mutex.unlock();
	ShowChances(0, true);
}
bool SinglePlayer::VisionBox(const int turn)
{
	const std::vector<std::string> visible_tour = GetTourParameters(GetTour(), turn, participants[0].car_modifiers[CarAttributes::visibility]);
	int ret = true;
	const HANDLE window = main_window->GetHandle();
	std::string helper;

	if (visible_tour.size() > 0)
	{
		current_field = visible_tour[0];
	}
	else
	{
		current_field = "0";
		ret = false;
	}
	mutex.lock();
	for (short i = 0; i < participants[0].car_modifiers[CarAttributes::visibility] && i < static_cast<short>(LanguagePack::text[LanguagePack::race_distances].size()); ++i)
	{
		if (static_cast<int>(visible_tour.size()) == i)
		{
			SetConsoleCursorPosition(window, { 1,24 + 2 * i });
			SetColor(window, *main_window->main_color);
			std::cout << LanguagePack::text[LanguagePack::race_distances][i];
			SetColor(window, *main_window->secondary_color);
			std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::meta] + "                                              ";
			SetConsoleCursorPosition(window, { 1,26 + 2 * i });
			std::cout << "                                                   ";
			break;
		}
		helper = visible_tour[i];
		SetColor(window, *main_window->main_color);
		SetConsoleCursorPosition(window, { 1,24 + 2 * i });
		std::cout << LanguagePack::text[LanguagePack::race_distances][i];
		SetColor(window, *main_window->secondary_color);
		std::cout << LanguagePack::text[LanguagePack::tour_segments_descriptions][(visible_tour[i][0] - 48) * 2 + (static_cast<int>(visible_tour[i].size()) > 1 ? 0 : 1)] << helper.erase(0, 1) << "                                               ";
	}
	mutex.unlock();
	return ret;
}
void SinglePlayer::Finish()
{
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		RacerLeaderboardInfo ranking_info = {
			tour,
			participants[i].name,
			participants[i].place,
			participants[i].score,
			!participants[i].IsAlive(),
			participants[i].sum_of_performed_attacks,
			participants[i].sum_of_performed_drifts,
			participants[i].sum_of_durability_burned,
			participants[i].car_path,
			participants[i].tire_path,
			main_window->GetAIs(),
			multiplayer_flag
		};
		RankingManagement::Save(ranking_info);
	}
	if (main_window->GetTimerSettings())
	{
		timer->StopTimer();
	}
	attack_phase = false;
	mutex.lock();
	ai_connector.reset();
	take_action_position = 0;
	participants.clear();
	participants.push_back(Participant(infobox.get()));
	ai_init = 0;
	infobox->Push(LanguagePack::text[LanguagePack::other_strings][OtherStrings::race_finished], "");
	char c = _getch();
	if (c < 0)
	{
		_getch();
	}
	infobox->Reset();
	mutex.unlock();
}