#include "NetworkRole.h"

SinglePlayer::SinglePlayer(ToT_Window &main_window)
{
	this->main_window = &main_window;
	request_handler = std::make_unique<GeneralMultiPlayer::RequestHandler>();
	participants.push_back(Participant(this->main_window));
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
		button = Text::Button(&timer_running, 13, 20);
		mutex.lock();
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
	Text::Spaces(decimal_position + 2);
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
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::action_confirm];
		mutex.unlock();
		button = Text::Button(&timer_running, 13, 20);

		mutex.lock();
		SetConsoleCursorPosition(window, coords);
		Text::Spaces(static_cast<int>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::action_confirm].size()));
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
		SetConsoleTextAttribute(window, main_window->color1);
		std::cout << " < ";
		SetConsoleTextAttribute(window, main_window->color2);
		std::cout << name;
		SetConsoleTextAttribute(window, main_window->color1);
		std::cout << " >  ";
		SetConsoleTextAttribute(window, main_window->color2);
		mutex.unlock();
	};
	show_name_func();
	do
	{
		button = Text::Button(&timer_running, 13, 20);
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
	}
	while (button != 13);
	mutex.lock();
	SetConsoleCursorPosition(window, coords);
	Text::Spaces(name_size + 5);
	mutex.unlock();
	if (name_size == 0)
	{
		name = LanguagePack::text[LanguagePack::other_strings][OtherStrings::default_name];
	}
	return name;
}
void SinglePlayer::ShowCarParameters(const std::string car_path, const bool clear)
{
	const std::vector<int> car_params = main_window->GetCarParameters(car_path);
	std::vector < std::pair<std::string, std::string>> text;

	for (short i = 0; i < static_cast<short>(car_params.size()); ++i)
	{
		text.push_back(std::make_pair(LanguagePack::text[LanguagePack::car_attributes][i], std::to_string(car_params[i])));
	}
	ShowLobbyInformation(LanguagePack::text[LanguagePack::game_lobby_informations][GameLobbyInformations::player_information_title], text, { 0, 19 }, 1, 2, clear);
}
void SinglePlayer::ShowTiresParameters(const std::string tire_path, bool clear)
{
	const std::vector<std::string> tire_params = main_window->GetTireParameters(tire_path);
	std::vector < std::pair<std::string, std::string>> text;

	for (short i = 0; i < static_cast<short>(tire_params.size()); ++i)
	{
		text.push_back(std::make_pair(LanguagePack::text[LanguagePack::terrain_types][i], tire_params[i]));
	}
	ShowLobbyInformation("", text, { 0, 38 }, 1, 2, clear);
}
void SinglePlayer::ShowTourParameters(const std::string tour_path, bool clear)
{
	ShowRankingParameters(tour_path.substr(0, static_cast<int>(tour_path.size()) - static_cast<int>(ExtName::tour.size())) + ExtName::ranking, clear);
	const std::vector<std::string> tour_params = main_window->GetTourParameters(tour_path, 0, INT_MAX);
	int segment_quantity[6] = { 0,0,0,0,0,0 };
	int turns = 0;

	for (short i = 0; i < static_cast<short>(tour_params.size()); ++i)
	{
		++segment_quantity[tour_params[i][0]-48];
		if (tour_params[i].size() > 1)
		{
			++turns;
		}
	}
	std::vector<std::pair<std::string, std::string>> text = { {LanguagePack::text[LanguagePack::game_lobby_informations][GameLobbyInformations::length_of_tour], std::to_string(static_cast<int>(tour_params.size()))}, {LanguagePack::text[LanguagePack::game_lobby_informations][GameLobbyInformations::number_of_turns], std::to_string(static_cast<int>(static_cast<float>(turns) / static_cast<float>(tour_params.size())*100.f)) + "%"} };
	for (short i = 0; i < 6; ++i)
	{
		text.push_back(std::make_pair(LanguagePack::text[LanguagePack::terrain_types][i], std::to_string(static_cast<int>(static_cast<float>(segment_quantity[i]) / static_cast<float>(tour_params.size())*100.f)) + '%'));
	}
	ShowLobbyInformation("", text, { static_cast<short>(main_window->GetWidth()) - static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size()), 28 }, 1, 2, clear);
}
void SinglePlayer::ShowRankingParameters(const std::string ranking_path, bool clear)
{
	std::ifstream fvar;
	std::string temp;
	std::string best_name = "", local_name;
	int best_place = 0, local_place;
	float winrate = 0;;
	int local_won_games;
	int local_games_in_total;
	int classification = 0 + (main_window->GetAIs() == 7);
	fvar.open(FolderName::tour + '\\' + ranking_path);
	for (int i = 0; std::getline(fvar, temp); ++i)
	{
		if (i%GameConstants::validate_ranking_details == 0)
		{
			local_name = temp;
		}
		else if (i%GameConstants::validate_ranking_details == 1)
		{
			local_games_in_total = atoi(main_window->GetClassifiedDetail(temp, classification).c_str());
		}
		else if (i%GameConstants::validate_ranking_details == 2)
		{
			local_won_games = atoi(main_window->GetClassifiedDetail(temp, classification).c_str());
		}
		else if (i%GameConstants::validate_ranking_details == 3)
		{
			local_place = atoi(main_window->GetClassifiedDetail(temp, classification).c_str());
		}
		else if (i%GameConstants::validate_ranking_details == 6)
		{
			int number_of_finished_games = local_games_in_total - atoi(main_window->GetClassifiedDetail(temp, classification).c_str());
			if (!number_of_finished_games)
			{
				continue;
			}
			if (winrate < static_cast<float>(local_won_games) / static_cast<float>(local_games_in_total) * 100.0f)
			{
				winrate = static_cast<float>(local_won_games) / static_cast<float>(local_games_in_total) * 100.0f;
				best_name = local_name;
				best_place = local_place/ number_of_finished_games;
			}
		}
	}
	fvar.close();

	const std::vector<std::pair<std::string, std::string>> vector = { {LanguagePack::text[LanguagePack::game_lobby_informations][GameLobbyInformations::current_champion], best_name},
		{LanguagePack::text[LanguagePack::game_lobby_informations][GameLobbyInformations::win_ratio],std::to_string(winrate).substr(0, static_cast<int>(std::to_string(winrate).size()) - 4) + '%'},
		{LanguagePack::text[LanguagePack::game_lobby_informations][GameLobbyInformations::average_place], std::to_string(best_place)} };
	ShowLobbyInformation(LanguagePack::text[LanguagePack::game_lobby_informations][GameLobbyInformations::tour_information_title], vector, { static_cast<short>(main_window->GetWidth()) - static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size()), 19 }, 1, 2, clear);
}
void SinglePlayer::ShowLobbyInformation(const std::string title, const std::vector<std::pair<std::string, std::string>> text, const COORD base_position, const short paragraph_size, const short spacing, const bool clear)
{
	const HANDLE handle = main_window->GetHandle();
	mutex.lock();
	if (clear)
	{
		const int border_size = static_cast<int>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size());
		if (title != "")
		{
			SetConsoleCursorPosition(handle, { base_position.X, base_position.Y });
			Text::Spaces(border_size);
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		Text::Spaces(border_size);
		for (short i = 0; i < static_cast<short>(text.size()); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			Text::Spaces(static_cast<short>(text[i].first.size()) + static_cast<short>(text[i].second.size()) + 2);
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(text.size()) + 2) });
		Text::Spaces(border_size);
	}
	else
	{
		if (title != "")
		{
			SetConsoleTextAttribute(handle, main_window->color2);
			SetConsoleCursorPosition(handle, { base_position.X + static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size()) / 2 - static_cast<short>(title.size()) / 2, base_position.Y });
			std::cout << title;
		}
		SetConsoleTextAttribute(handle, main_window->color2);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
		for (short i = 0; i < static_cast<short>(text.size()); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << text[i].first + ": ";
			SetConsoleTextAttribute(handle, main_window->color2);
			std::cout << text[i].second;
		}
		SetConsoleTextAttribute(handle, main_window->color2);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(text.size()) + 2) });
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
	}
	mutex.unlock();
}
void SinglePlayer::ShowLeaderboard(const std::vector<std::string> text, short pos, int color, const bool clear)
{
	mutex.lock();
	SetConsoleTextAttribute(main_window->GetHandle(), color);
	for (int j = 0; j < static_cast<int>(text.size()); ++j)
	{
		const short x_pos = static_cast<short>(main_window->GetWidth() - 55 + 16 * j + 8) - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(text[j].size()));
		const short y_pos = 16 + pos * 2;
		SetConsoleCursorPosition(main_window->GetHandle(), { x_pos, y_pos });
		if (clear)
		{
			Text::Spaces(static_cast<int>(text.size()));
		}
		else
		{
			std::cout << text[j];
		}
	}
	mutex.unlock();
}
void SinglePlayer::ShowChances(const int value, const bool reset)
{
	float speed_estimation = (participants[0].current_speed + static_cast<float>(value) * (0.9f + 0.2f*participants[0].TireEffectivness(current_field)));
	if (speed_estimation > static_cast<float>(participants[0].car_modifiers[CarAttributes::max_speed])*1.25f)
	{
		speed_estimation = static_cast<float>(participants[0].car_modifiers[CarAttributes::max_speed])*1.25f;
	}
	else if (speed_estimation < 0)
	{
		speed_estimation = 0;
	}
	float burned_durability = participants[0].CalculateBurning(speed_estimation - participants[0].car_modifiers[CarAttributes::max_speed]);
	speed_estimation *= GameConstants::friction_scalar;
	bool drift = take_action_position == 2 && participants[0].current_speed > 40.0f && current_field.size() > 1;
	float chance_to_succeed = static_cast<float>(static_cast<int>((100.0f - participants[0].EvaluateChance(current_field, speed_estimation, drift))));
	float estimated_time = drift ? GameConstants::drift_value : 100.0f / (1.0f + speed_estimation * 10.0f / 36.0f);

	HANDLE window = main_window->GetHandle();
	std::string helper;
	std::vector<std::pair<double, std::string>> values = { {chance_to_succeed, LanguagePack::text[LanguagePack::race_chancebox_informations][0]},  {estimated_time, LanguagePack::text[LanguagePack::race_chancebox_informations][1]},
														{burned_durability, LanguagePack::text[LanguagePack::race_chancebox_informations][2]}, {speed_estimation, LanguagePack::text[LanguagePack::race_chancebox_informations][3]} };
	mutex.lock();
	if (!reset)
	{
		for (short i = 0; i < values.size(); ++i)
		{
			if (values[i].first < 0.01)
			{
				values[i].first = 0;
			}
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 28 + i) });
			SetConsoleTextAttribute(window, main_window->color1);
			std::cout << values[i].second;
			SetConsoleTextAttribute(window, main_window->color2);
			helper = std::to_string(values[i].first);
			helper = helper.substr(0, static_cast<int>(helper.size()) - 4);
			std::cout << helper << "              ";
		}
	}
	else
	{
		for (short i = 0; i < values.size(); ++i)
		{
			SetConsoleTextAttribute(window, main_window->color1);
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 28 + i) });
			std::cout << values[i].second << "                   ";
		}
		SetConsoleTextAttribute(window, main_window->color2);
	}
	mutex.unlock();
}
void SinglePlayer::ShowIndicator(int participant)
{
	const COORD coord = { main_window->GetWidth() - 55, 16 + participants[participant].place * 2 };
	mutex.lock();
	SetConsoleCursorPosition(main_window->GetHandle(), coord);
	SetConsoleTextAttribute(main_window->GetHandle(), participants[participant].action_performed ? main_window->color2 : 8);
	std::cout << '*';
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
		participants.push_back(Participant(main_window));
	}
	if (number_of_ais)
	{
		ai_connector = std::make_unique<AIConnector>(FolderName::main + '\\' + FileName::ai, 255);
		if (!ai_connector->HandleConnection(&SinglePlayer::HandleAIConnection, this))
		{
			MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
			return;
		}
	}
	while (ai_init != number_of_ais)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		participants[i].Init(tour);
	}
}
void SinglePlayer::SortLeaderboard()
{
	//ugly quadratic complexity but it is justified by the fact that n is at most 8
	for (int i = 0; i < participants.size(); ++i)
	{
		participants[i].place = 1;
		for (int j = 0; j < participants.size(); ++j)
		{
			if (participants[i].score > participants[j].score)
			{
				++participants[i].place;
			}
			else if (participants[i].score == participants[j].score && i > j)
			{
				++participants[i].place;
			}
		}
	}
}
void SinglePlayer::GetCurrentAttributes()
{
	if (main_window->GetTimerSettings())
	{
		timer->StartTimer(main_window->GetTimerSettings());
	}
	for (int i = static_cast<int>(participants.size()) - 1; i >= 0; --i)
	{
		if (participants[i].IsAlive())
		{
			mutex.lock();
			participants[i].Test(current_field, i < static_cast<int>(participants.size()) - main_window->GetAIs());
			mutex.unlock();
		}
	}
}
std::string SinglePlayer::GetTour()
{
	return tour;
}
bool SinglePlayer::GameLobby()
{
	const HANDLE handle = main_window->GetHandle();
	COORD starting_point = { static_cast<short>(main_window->GetWidth()) / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	std::string name = main_window->GetName();
	int timer_settings = main_window->GetTimerSettings();
	std::vector<std::string> tours = main_window->GetTourNames();
	std::vector<std::string> tires = main_window->GetTireNames();
	std::vector<std::string> cars = main_window->GetCarNames(tours[0]);
	main_window->RemoveExtension(tours, ExtName::tour);
	main_window->RemoveExtension(tires, ExtName::tire);
	main_window->RemoveExtension(cars, ExtName::car);
	int ais = main_window->GetAIs();
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;

	ShowCarParameters(cars[cars_pos] + ExtName::car);
	ShowTiresParameters(tires[tires_pos] + ExtName::tire);
	ShowTourParameters(tours[tours_pos] + ExtName::tour);

	while (true)
	{
		main_menu_position = Text::Choose::Veritcal(LanguagePack::text[LanguagePack::game_menu_options], main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window, &mutex, &timer_running);
		if (!timer_running)
		{
			break;
		}
		const COORD starting_local_pos = { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::game_menu_options][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing };
		switch (main_menu_position)
		{
			case 0://choosing name
			{
				name = StringSelection(name, GameConstants::maximum_name_length, {starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::game_menu_options][0].size()), 25 });
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
				ais = Text::Choose::Horizontal(text, ais, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
				ShowRankingParameters(tours[tours_pos] + ExtName::ranking, true);
				main_window->SetAIs(ais);
				ShowRankingParameters(tours[tours_pos] + ExtName::ranking);
				break;
			}
			case 2: //timer
			{
				std::vector<std::string> timer_values = { LanguagePack::text[LanguagePack::on_off][1]};
				for (int i = 1; i <= GameConstants::maximum_timer; ++i)
				{
					timer_values.push_back((i < 60 ? "0" : "")+std::to_string(i / 6) + ':' + std::to_string(i % 6) + '0');
				}
				timer_settings = Text::Choose::Horizontal(timer_values, timer_settings, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
				main_window->SetTimerSettings(timer_settings);
				break;
			}
			case 3://choosing tour
			{
				int i = tours_pos;
				tours_pos = Text::Choose::Horizontal(tours, tours_pos, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
				if (i != tours_pos)
				{
					ShowCarParameters(cars[cars_pos] + ExtName::car, true);
					cars = main_window->GetCarNames(tours[tours_pos] + ExtName::tour);
					main_window->RemoveExtension(cars, ExtName::car);
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
				cars_pos = Text::Choose::Horizontal(cars, cars_pos, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
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
				tires_pos = Text::Choose::Horizontal(tires, tires_pos, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
				if (i != tires_pos)
				{
					ShowTiresParameters(tires[i] + ExtName::tire, true);
					ShowTiresParameters(tires[tires_pos] + ExtName::tire);
				}
				break;
			}
			case 7://Back
			{
				main_window->SaveAtributes();
				return false;
			}
		}
		if (main_menu_position == 6)//Next
		{
			break;
		}
	}
	for (int i = 0; i < static_cast<int>(LanguagePack::text[LanguagePack::game_menu_options].size()); ++i)
	{
		mutex.lock();
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::text[LanguagePack::game_menu_options][i].size())), starting_point.Y + static_cast<short>(i * spacing) });
		Text::Spaces(static_cast<int>(LanguagePack::text[LanguagePack::game_menu_options][i].size()));
		mutex.unlock();
	}
	if (timer_settings)
	{
		COORD coord = { 0,0 };
		timer = std::make_unique<VisibleTimer>(coord, main_window->GetHandle(), &timer_running, &mutex);
		timer->StartTimer(timer_settings);
	}
	main_window->SaveAtributes();
	ShowTiresParameters(tires[tires_pos] + ExtName::tire, true);
	ShowCarParameters(cars[cars_pos] + ExtName::car, true);
	ShowTourParameters(tours[tours_pos] + ExtName::tour, true);
	GetParticipants(name, tours[tours_pos] + ExtName::tour, cars[cars_pos] + ExtName::car, tires[tires_pos] + ExtName::tire);
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
		if (target != 10)
		{
			if (participants[target].score < participants[participant].score + GameConstants::attack_backward_distance && 
				participants[target].score >participants[participant].score - GameConstants::attack_forward_distance && 
				participants[target].IsAlive() &&
				!participants[participant].attack_performed)
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
				MessageBox(0, (participants[participant].name + ErrorMsg::cheating_attempt).c_str(), ErrorTitle::cheating_attempt.c_str(), 0);
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
			mutex.unlock();
			return;
		}
		if ((action.first == 0 && (action.second > participants[participant].car_modifiers[CarAttributes::max_accelerating] || action.second <= 0)) ||
			(action.first == 1 && (action.second < participants[participant].car_modifiers[CarAttributes::max_braking] * -1 || action.second >= 0 || participants[participant].current_speed <= 0)) ||
			(action.first == 2 && (action.second < participants[participant].car_modifiers[CarAttributes::hand_brake_value] * -1 || action.second >= 0 || participants[participant].current_speed <= 0)) ||
			(action.first == 3 && (action.second != 0 || participants[participant].current_speed <= 0)) ||
			participants[participant].action_performed)
		{
			mutex.lock();
			participants[participant].KillParticipant();
			mutex.unlock();
			MessageBox(0, (participants[participant].name + ErrorMsg::cheating_attempt).c_str(), ErrorTitle::cheating_attempt.c_str(), 0);
			return;
		}
		if (action.first == 2 && participants[participant].current_speed >= GameConstants::minimum_drift_speed)
		{
			participants[participant].drift = true;
		}
		participants[participant].CalculateParameters(static_cast<float>(action.second), current_field);
		participants[participant].action_performed = true;
		ShowIndicator(participant);
		
	}
}
int SinglePlayer::PerformAttack()
{
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
			short i = Text::Choose::Veritcal(rival_name, 0, { static_cast<short>(main_window->GetWidth() - 28), static_cast<short>(main_window->GetHeight() - 17) }, 2, Text::TextAlign::center, true, *main_window, &mutex, &timer_running);
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
	OutputDebugString(msg_received.c_str());
	const std::string msg = msg_received.substr(2, msg_length - 4);
	switch (code)
	{
		case ConnectionCodes::Start:
		{
			ai_connector->Write(std::to_string(code));
			break;
		}
		case ConnectionCodes::GetInit:
		{
			ai_connector->Write(std::to_string(main_window->GetAIs()));
			ai_connector->Write(std::to_string(static_cast<int>(participants.size())));
			break;
		}
		case ConnectionCodes::NewTurn:
		{
			if (msg_length < 3)
			{
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			const int ai_position_start = static_cast<int>(participants.size()) - main_window->GetAIs();
			const int id = msg_received[2] - 48 + ai_position_start;
			if (id < ai_position_start || id >= static_cast<int>(participants.size()))
			{
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			ai_connector->Write(std::to_string(participants[id].action_performed));
			break;
		}
		case ConnectionCodes::GetCarNames:
		{
			const std::vector<std::string> car_names = main_window->GetCarNames(tour);
			const int car_names_size = static_cast<int>(car_names.size());
			ai_connector->Write(std::to_string(car_names_size));
			for (int i = 0; i < car_names_size; ++i)
			{
				ai_connector->Write(car_names[i]);
			}
			break;
		}
		case ConnectionCodes::GetTireNames:
		{
			const std::vector<std::string> tire_names = main_window->GetTireNames();
			const int tire_names_size = static_cast<int>(tire_names.size());
			ai_connector->Write(std::to_string(tire_names_size));
			for (int i = 0; i < tire_names_size; ++i)
			{
				ai_connector->Write(tire_names[i]);
			}
			break;
		}
		case ConnectionCodes::GetCarParams:
		{
			bool valid = false;
			const std::vector<std::string> car_names = main_window->GetCarNames(tour);
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
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			const std::vector<int> car_param = main_window->GetCarParameters(msg);
			for (int i = 0; i < static_cast<int>(car_param.size()); ++i)
			{
				ai_connector->Write(std::to_string(car_param[i]));
			}
			break;
		}
		case ConnectionCodes::GetTireParams:
		{
			bool valid = false;
			const std::vector<std::string> tire_names = main_window->GetTireNames();
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
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			const std::vector<std::string> tires_param = main_window->GetTireParameters(msg);
			for (int i = 0; i < static_cast<int>(tires_param.size()); ++i)
			{
				ai_connector->Write(tires_param[i]);
			}
			break;
		}
		case ConnectionCodes::GetAllAttributes:
		{
			for (int i = 0; i < static_cast<int>(participants.size()); ++i)
			{
				ai_connector->Write(std::to_string(participants[i].current_speed));
				ai_connector->Write(std::to_string(participants[i].current_durability));
				ai_connector->Write(std::to_string(participants[i].score));
			}
			break;
		}
		case ConnectionCodes::GetTour:
		{
			const std::vector<std::string> tour_param = main_window->GetTourParameters(tour, 0, INT_MAX);
			const int tour_size = static_cast<int>(tour_param.size());
			ai_connector->Write(std::to_string(tour_size));
			for (int i = 0; i < tour_size; ++i)
			{
				ai_connector->Write(tour_param[i]);
			}
			break;
		}
		case ConnectionCodes::SetName:
		{
			const std::string selected_name = msg.substr(1, msg.size() - 1);
			const int ai_id = msg[0] - 48;
			//name validation
			const int selected_name_size = static_cast<int>(selected_name.size());
			if (selected_name_size > GameConstants::maximum_name_length || selected_name_size <= 0)
			{
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			//ai id validation
			if (ai_id < 0 || ai_id > main_window->GetAIs())
			{
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].name = selected_name;
			break;
		}
		case ConnectionCodes::SetCar:
		{
			bool valid = false;
			const std::vector<std::string> car_names = main_window->GetCarNames(tour);
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
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			//ai id validation
			if(ai_id < 0 || ai_id > main_window->GetAIs())
			{
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].car_path = selected_car;
			break;
		}
		case ConnectionCodes::SetTires:
		{
			bool valid = false;
			const std::vector<std::string> tire_names = main_window->GetTireNames();
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
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			//ai id validation
			if (ai_id < 0 || ai_id > main_window->GetAIs())
			{
				MessageBox(0, ErrorMsg::ai_connection.c_str(), ErrorTitle::ai_connection.c_str(), 0);
				exit(0);
			}
			participants[static_cast<int>(participants.size()) - main_window->GetAIs() + ai_id].tire_path = selected_tires;
			++ai_init;
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
			take_action_position = Text::Choose::Veritcal(LanguagePack::text[LanguagePack::race_actions], take_action_position, { 1,39 }, 2, Text::TextAlign::left, false, *main_window, &mutex, &timer_running);
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
				main_window->Pause(1500);
				mutex.lock();
				SetConsoleCursorPosition(window, { static_cast<short>(LanguagePack::text[LanguagePack::race_actions][take_action_position].size()) + 1, 39 + 2 * take_action_position });
				Text::Spaces(static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::unable_to_move].size()));
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
	ShowLeaderboard(LanguagePack::text[LanguagePack::race_leaderboard], 0, main_window->color2, clear);
	if (!clear)
	{
		SortLeaderboard();
		for (int i = 0; i < static_cast<int>(participants.size()); ++i)
		{
			participants[i].attack_performed = false;
			participants[i].action_performed = false;
			ShowIndicator(i);
		}
	}
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		std::vector<std::string> leaderboard_info = { std::to_string(participants[i].place), participants[i].name, std::to_string(participants[i].score) };
		leaderboard_info[2] = leaderboard_info[2].substr(0, static_cast<int>(leaderboard_info[2].size()) - 4);
		ShowLeaderboard(leaderboard_info, static_cast<short>(participants[i].place), main_window->color1, clear);
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
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << LanguagePack::text[LanguagePack::race_general_informations][0];
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << participants[0].name;
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << LanguagePack::text[LanguagePack::race_general_informations][1];
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << speed << "     ";
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 20 });
	std::cout << LanguagePack::text[LanguagePack::race_general_informations][2];
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << durability << LanguagePack::text[LanguagePack::race_general_informations][3];
	SetConsoleTextAttribute(window, 8);
	for (short i = 0; i < 5; ++i)
	{
		SetConsoleCursorPosition(window, { 2, 40 + i * 2 });
		std::cout << possible_actions[i];
	}
	mutex.unlock();
	const short x = static_cast<short>(main_window->GetWidth() - static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size()));
	const std::vector<std::tuple<std::string, COORD, short>> boxes = { {LanguagePack::text[LanguagePack::race_names_of_boxes][0], {0, static_cast<short>(main_window->GetHeight() - 15)}, 12 },
				{ LanguagePack::text[LanguagePack::race_names_of_boxes][1], { x, static_cast<short>(main_window->GetHeight() - 20) }, 17},
				{ LanguagePack::text[LanguagePack::race_names_of_boxes][2],  { x, static_cast<short>(main_window->GetHeight() - 31) }, 7 } };
	
	mutex.lock();
	SetConsoleTextAttribute(window, main_window->color2);
	for (int i = 0; i < static_cast<int>(boxes.size()); ++i)
	{
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X + static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size())/2 - static_cast<short>(std::get<0>(boxes[i]).size())/2, std::get<1>(boxes[i]).Y});
		std::cout << std::get<0>(boxes[i]);
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + 1});
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + std::get<2>(boxes[i]) + 2 });
		std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
	}
	mutex.unlock();
	ShowChances(0, true);
}
bool SinglePlayer::VisionBox(const int turn)
{
	const std::vector<std::string> visible_tour = main_window->GetTourParameters(GetTour(), turn, participants[0].car_modifiers[CarAttributes::visibility]);
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
			SetConsoleTextAttribute(window, main_window->color1);
			std::cout << LanguagePack::text[LanguagePack::race_distances][i];
			SetConsoleTextAttribute(window, main_window->color2);
			std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::meta] + "                                              ";
			SetConsoleCursorPosition(window, { 1,26 + 2 * i });
			std::cout << "                                                   ";
			break;
		}
		helper = visible_tour[i];
		SetConsoleTextAttribute(window, main_window->color1);
		SetConsoleCursorPosition(window, { 1,24 + 2 *i });
		std::cout << LanguagePack::text[LanguagePack::race_distances][i];
		SetConsoleTextAttribute(window, main_window->color2);
		std::cout << LanguagePack::text[LanguagePack::race_segments_descriptions][(visible_tour[i][0] - 48) * 2 + (static_cast<int>(visible_tour[i].size()) > 1 ? 0 : 1)] << helper.erase(0, 1) << "                                               ";
	}
	mutex.unlock();
	return ret;
}
void SinglePlayer::Finish()
{
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		main_window->SaveRanking(tour, participants[i].name, participants[i].place, static_cast<int>(participants[i].score), !participants[i].IsAlive(), participants[i].sum_of_performed_attacks, participants[i].sum_of_performed_drifts, static_cast<int>(participants[i].sum_of_durability_burned), participants[i].car_path, participants[i].tire_path);
	}
	main_window->infobox->infobox.clear();
	if (main_window->GetTimerSettings())
	{
		timer->StopTimer();
	}
	ai_connector.reset();
}