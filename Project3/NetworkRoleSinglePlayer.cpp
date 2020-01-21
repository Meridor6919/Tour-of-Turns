#include "NetworkRole.h"

int SinglePlayer::NumericalSelection(const COORD coords)
{
	const HANDLE window = main_window->GetHandle();
	char button;
	int value = 0;
	short decimal_position = 0;

	std::cout << ": ";
	while (true)
	{
		button = _getch();
		SetConsoleCursorPosition(window, { coords.X + 2 + decimal_position, coords.Y });
		if (button >= '0' && button <= '9')
		{
			if (button == '0' && decimal_position == 0)
			{
				continue;
			}
			else if (value * 10 + button - 48 > participants[0].car_modifiers[CarModifiers::max_accelerating + take_action_position])
			{
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
			break;
		}
		if (value > 0)
		{
			ShowChances(value * (static_cast<bool>(take_action_position) ? -1 : 1));
		}
		else
		{
			ShowChances(0, true);
		}
	}
	SetConsoleCursorPosition(window, coords);
	for (int i = 0; i < decimal_position + 2; ++i)
	{
		std::cout << " ";
	}
	ShowChances(0, true);
	return value * (static_cast<bool>(take_action_position) ? -1 : 1);
}
int SinglePlayer::BinarySelection(const COORD coords)
{
	const HANDLE window = main_window->GetHandle();
	char button;

	while (true)
	{
		ShowChances(participants[0].car_modifiers[CarModifiers::hand_brake_value] * -1 * static_cast<int>(take_action_position == 2), take_action_position == 4);
		SetConsoleCursorPosition(window, coords);
		std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::action_confirm];
		button = _getch();
		SetConsoleCursorPosition(window, coords);

		for (int i = 0; i < static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::action_confirm].size()); ++i)
		{
			std::cout << " ";
		}
		if (button == 'y' || button == 'Y')
		{
			ShowChances(0, true);
			return take_action_position;
		}
		else if (button == 13 || button == 27 || button == 'n' || button == 'N')
		{
			ShowChances(0, true);
			return 0;
		}
	}
}
std::string SinglePlayer::StringSelection(const std::string current_name, const int max_name_size)
{
	const HANDLE handle = main_window->GetHandle();
	std::string name = current_name;
	int name_size = static_cast<int>(name.size());
	char button;

	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << " < ";
	SetConsoleTextAttribute(handle, main_window->color2);
	std::cout << name;
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << " >";
	SetConsoleTextAttribute(handle, main_window->color2);

	do
	{
		button = _getch();
		if (name_size < max_name_size && ((button >= 48 && button <= 57) || (button >= 65 && button <= 90 && GetKeyState(VK_SHIFT) != 1 && GetKeyState(VK_SHIFT) != 0) || (button >= 97 && button <= 122)))
		{
			name += button;
			SetConsoleTextAttribute(handle, main_window->color2);
			std::cout << "\b\b" << button;
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << " >";
		}
		else if (name_size < max_name_size && button == 32 && name_size > 0 && name[name_size - 1] != 32)
		{
			name += ' ';
			SetConsoleTextAttribute(handle, main_window->color2);
			std::cout << "\b\b" << button;
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << " >";
		}
		else if (button == 8 && name_size > 0)
		{
			name.erase(name_size - 1, 1);
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << "\b\b\b   \b\b>";
		}
		name_size = static_cast<int>(name.size());
	}

	while (button != 13);
	for (int i = 0; i < name_size + 4; ++i)
	{
		std::cout << "\b";
	}
	for (int i = 0; i < name_size + 4; ++i)
	{
		std::cout << " ";
	}
	if (name_size == 0)
	{
		name = LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::default_name];
	}
	return name;
}
void SinglePlayer::ShowLobbyInformation(const std::string title, const std::vector<std::pair<std::string, std::string>> text, const COORD base_position, const short paragraph_size, const short spacing, const bool clear)
{
	const HANDLE handle = main_window->GetHandle();
	if (clear)
	{
		const int border_size = static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border].size());
		if (title != "")
		{
			SetConsoleCursorPosition(handle, { base_position.X, base_position.Y });
			for (int i = 0; i < border_size; ++i)
			{
				std::cout << " ";
			}
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		for (int i = 0; i < border_size; ++i)
		{
			std::cout << " ";
		}
		for (short i = 0; i < static_cast<short>(text.size()); ++i)
		{
			SetConsoleCursorPosition(handle, { base_position.X + paragraph_size, base_position.Y + spacing * (i + 2) });
			for (int j = 0; j < static_cast<short>(text[i].first.size()) + static_cast<short>(text[i].second.size()) + 2; ++j)
			{
				std::cout << " ";
			}
		}
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + spacing * (static_cast<short>(text.size()) + 2) });
		for (int i = 0; i < border_size; ++i)
		{
			std::cout << " ";
		}
	}
	else
	{
		if (title != "")
		{
			SetConsoleTextAttribute(handle, main_window->color2);
			SetConsoleCursorPosition(handle, { base_position.X + static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border].size())/2 - static_cast<short>(title.size()) / 2, base_position.Y });
			std::cout << title;
		}
		SetConsoleTextAttribute(handle, main_window->color2);
		SetConsoleCursorPosition(handle, { base_position.X, base_position.Y + 1 });
		std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border];
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
		std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border];
	}
}
void SinglePlayer::ShowCarParameters(const std::string car_path, const bool clear)
{
	const std::vector<int> car_params = main_window->GetCarParameters(car_path);
	std::vector < std::pair<std::string, std::string>> text;

	for (short i = 0; i < static_cast<short>(car_params.size()); ++i)
	{
		text.push_back(std::make_pair(LanguagePack::vector_of_strings[LanguagePack::car_modifiers][i], std::to_string(car_params[i])));
	}
	ShowLobbyInformation(LanguagePack::vector_of_strings[LanguagePack::information_box_titles][GameInformation::player_info], text, { 0, 19 }, 1, 2, clear);
}
void SinglePlayer::ShowTiresParameters(const std::string tire_path, bool clear)
{
	const std::vector<std::string> tire_params = main_window->GetTireParameters(tire_path);
	std::vector < std::pair<std::string, std::string>> text;

	for (short i = 0; i < static_cast<short>(tire_params.size()); ++i)
	{
		text.push_back(std::make_pair(LanguagePack::vector_of_strings[LanguagePack::tire_modifiers][i], tire_params[i]));
	}
	ShowLobbyInformation("", text, { 0, 38 }, 1, 2, clear);
}
void SinglePlayer::ShowTourParameters(const std::string tour_path, bool clear)
{
	ShowRankingParameters(tour_path, clear);
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
	std::vector<std::pair<std::string, std::string>> text = { {LanguagePack::vector_of_strings[LanguagePack::information_box_titles][GameInformation::tour_length], std::to_string(static_cast<int>(tour_params.size()))}, {LanguagePack::vector_of_strings[LanguagePack::information_box_titles][GameInformation::turns], std::to_string(static_cast<int>(static_cast<float>(turns) / static_cast<float>(tour_params.size())*100.f)) + "%"} };
	for (short i = 0; i < 6; ++i)
	{
		text.push_back(std::make_pair(LanguagePack::vector_of_strings[LanguagePack::tire_modifiers][i], std::to_string(static_cast<int>(static_cast<float>(segment_quantity[i]) / static_cast<float>(tour_params.size())*100.f)) + "%"));
	}
	ShowLobbyInformation("", text, { static_cast<short>(main_window->GetWidth()) - static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border].size()), 28 }, 1, 2, clear);
}
void SinglePlayer::ShowRankingParameters(const std::string ranking_path, bool clear)
{
	const std::vector<std::pair<std::string, std::string>> vector = { {LanguagePack::vector_of_strings[LanguagePack::information_box_titles][GameInformation::champion], ""},{LanguagePack::vector_of_strings[LanguagePack::information_box_titles][GameInformation::win_rate], ""}, {LanguagePack::vector_of_strings[LanguagePack::information_box_titles][GameInformation::avg_place], ""} };
	ShowLobbyInformation(LanguagePack::vector_of_strings[LanguagePack::information_box_titles][GameInformation::tour_info], vector, { static_cast<short>(main_window->GetWidth()) - static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border].size()), 19 }, 1, 2, clear);
}
SinglePlayer::SinglePlayer(ToT_Window &main_window)
{
	this->main_window = &main_window;
	
	request_handler = std::make_unique<GeneralMultiPlayer::RequestHandler>();
}
bool SinglePlayer::GameLobby()
{
	const HANDLE handle = main_window->GetHandle();
	COORD starting_point = { static_cast<short>(main_window->GetWidth()) / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	std::string name = main_window->GetName();
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

	ShowCarParameters(cars[cars_pos]+ExtName::car);
	ShowTiresParameters(tires[tires_pos]+ ExtName::tire);
	ShowTourParameters(tours[tours_pos]+ ExtName::tour);

	while (main_menu_position != 5 || static_cast<int>(cars.size()) == 0)
	{
		switch (main_menu_position = Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::game_lobby], main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window))
		{
		case 0://choosing name
		{
			name = StringSelection(name, 14);
			break;
		}
		case 1://Number of ais
		{
			std::vector<std::string> text;

			for (int i = 0; i <= Possible_AIs(); ++i)
			{
				text.push_back(std::to_string(i));
			}
			ais = Text::Choose::Horizontal(text, ais, { starting_point.X + static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_lobby][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
			break;
		}
		case 2://choosing tour
		{
			int i = tours_pos;
			tours_pos = Text::Choose::Horizontal(tours, tours_pos, { starting_point.X + static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_lobby][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
			if (i != tours_pos)
			{
				ShowCarParameters(cars[cars_pos] + ExtName::car, true);
				cars = main_window->GetCarNames(tours[tours_pos]+ExtName::tour);
				main_window->RemoveExtension(cars, ExtName::car);
				cars_pos = 0;
				ShowCarParameters(cars[cars_pos] + ExtName::car);
			}
			ShowTourParameters(tours[i] + ExtName::tour, true);
			ShowTourParameters(tours[tours_pos] + ExtName::tour);
			break;
		}
		case 3://choosing car
		{
			int i = cars_pos;
			cars_pos = Text::Choose::Horizontal(cars, cars_pos, { starting_point.X + static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_lobby][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
			ShowCarParameters(cars[i] + ExtName::car, true);
			ShowCarParameters(cars[cars_pos] + ExtName::car);
			break;
		}
		case 4://choosing tires
		{
			int i = tires_pos;
			tires_pos = Text::Choose::Horizontal(tires, tires_pos, { starting_point.X + static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::game_lobby][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
			ShowTiresParameters(tires[i]+ExtName::tire, true);
			ShowTiresParameters(tires[tires_pos]+ ExtName::tire);
			break;
		}
		}
	}
	for (int i = 0; i < static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::game_lobby].size()); ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::vector_of_strings[LanguagePack::game_lobby][i].size())), starting_point.Y + static_cast<short>(i * spacing) });
		for (int j = 0; j < static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::game_lobby][i].size()); ++j)
		{
			std::cout << " ";
		}
	}
	main_window->SetAIs(ais);
	main_window->SetName(name);
	main_window->SaveAtributes();
	ShowTiresParameters(tires[tires_pos]+ ExtName::tire, true);
	ShowCarParameters(cars[cars_pos] + ExtName::car, true);
	ShowTourParameters(tours[tours_pos] + ExtName::tour, true);
	GetParticipants(name, tours[tours_pos] + ExtName::tour, cars[cars_pos] + ExtName::car, tires[tires_pos] + ExtName::tire);
	return true;
}
void SinglePlayer::GetParticipants(const std::string name, const std::string tour, const std::string car, const std::string tire)
{
	this->tour = tour;
	participants.emplace_back(name, car, tire, main_window);
}
std::vector<std::pair<float, std::string>> SinglePlayer::GetRankingInfo()
{
	bool flag = true;
	std::pair<float, std::string> helper;
	std::vector<std::pair<float, std::string>> ret;

	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		if (participants[i].alive)
		{
			ret.emplace_back(participants[i].score, participants[i].name);
		}
	}
	while (flag)
	{
		flag = false;
		for (int i = 0; i < static_cast<int>(ret.size()) - 1; ++i)
		{
			if (ret[i].first > ret[i+1].first)
			{
				flag = true;
				helper = std::move(ret[i]);
				ret[i] = std::move(ret[i + 1]);
				ret[i + 1] = std::move(helper);
			}
		}
	}
	return ret;
}
bool SinglePlayer::GetCurrentAtribs()
{
	for (int i = static_cast<int>(participants.size()) - 1; i >= 0; --i)
	{
		participants[i].Test(current_field, i < static_cast<int>(participants.size()) - main_window->GetAIs());
		if (participants[i].current_durability <= 0.0f && participants[i].alive)
		{
			main_window->infobox->Push(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::infobox_RIP1] +participants[i].name + LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::infobox_RIP2], "");
			participants[i].alive = false;
			if (i == 0)
			{
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color1);
				SetConsoleCursorPosition(main_window->GetHandle(), { 0, 20 });
				std::cout << LanguagePack::vector_of_strings[LanguagePack::race_attribs][2];
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color2);
				std::cout << static_cast<int>(participants[0].current_durability) << LanguagePack::vector_of_strings[LanguagePack::race_attribs][3];
				return false;
			}
		}
	}
	return true;
}
void SinglePlayer::Attack()
{
	const HANDLE handle = main_window->GetHandle();
	std::vector<std::string> rival_name;
	std::vector<int> rival_id;
	const int forward_attack_distance = 4;
	const int backward_attack_distance = 6;
	std::multimap<float, Participant*> sorted_participants;

	rival_name.push_back(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::attack]);
	rival_id.push_back(10);
	participants[0].attacked = 0;
	sorted_participants.insert(std::make_pair(participants[0].score, &participants[0]));

	for (int i = 1; i < static_cast<int>(participants.size()); ++i)
	{
		sorted_participants.insert(std::make_pair(participants[i].score,&participants[i]));
		if (participants[i].score < participants[0].score + backward_attack_distance && participants[i].score >participants[0].score - forward_attack_distance && participants[i].alive)
		{
			rival_name.push_back(participants[i].name);
			rival_id.push_back(i);
		}
		participants[i].attacked = 0;
	}
	if (rival_id.size() != 1 && participants[0].alive)
	{
		short i = Text::Choose::Veritcal(rival_name, 0, { static_cast<short>(main_window->GetWidth() - 28), static_cast<short>(main_window->GetHeight()-17) }, 2, Text::TextAlign::center, true, *main_window);
		if (rival_id[i] != 10)
		{
			participants[rival_id[i]].attacked+=1;
			participants[0].attacked += 0.5f;
		}
	}
	for (int i = static_cast<int>(participants.size()) - main_window->GetAIs(); i < static_cast<int>(participants.size()); ++i)
	{
		for (auto it = sorted_participants.begin(); it != sorted_participants.end(); ++it)
		{
			if (it->second->score > participants[i].score - forward_attack_distance && it->second->score <= participants[i].score && it->second->alive && it->second != &participants[i])
			{
				it->second->attacked += 1;
				participants[i].attacked += 0.5f;
				break;
			}
		}
	}
}
void SinglePlayer::TakeAction()
{
	if (!participants[0].alive)
		return;
	
	int value;
	while (true)
	{
		take_action_position = Text::Choose::Veritcal(LanguagePack::vector_of_strings[LanguagePack::race_actions], take_action_position, { 1,39 }, 2, Text::TextAlign::left, false, *main_window);
		if (participants[0].current_speed == 0 && take_action_position % 4 != 0)
		{
			
			std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::unable_to_move];
			main_window->Pause(1500);
			const int string_size = static_cast<int>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::unable_to_move].size());
			for (int i = 0; i < string_size; ++i)
			{
				std::cout << "\b";
			}
			for (int i = 0; i < string_size; ++i)
			{
				std::cout << " ";
			}
			continue;
		}
		if (take_action_position < 2)
		{
			value = NumericalSelection({ static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::race_actions][take_action_position].size()) + 1, 39 + 2 * take_action_position });
			if (value != 0)
			{
				break;
			}
		}
		else
		{
			int option = BinarySelection({ static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::race_actions][take_action_position].size()) + 1, 39 + 2 * take_action_position });
			if (option == 0)
			{
				continue;
			}
			else if (option == 2)
			{
				value = participants[0].car_modifiers[CarModifiers::hand_brake_value] * -1;
				participants[0].drift = true;
				break;
			}
			else if (option == 3)
			{
				value = 0;
				break;
			}
			else if (option == 4)
			{
				participants[0].current_durability = 0;
				return;
			}
		}
	}
	participants[0].CalculateParameters(static_cast<float>(value), current_field);
}
void SinglePlayer::GetOthersAction(const int turn)
{
	
}
int SinglePlayer::Possible_AIs()
{
	return 7;
}
void SinglePlayer::ShowChances(const int value, const bool reset)
{
	float speed_estimation = (participants[0].current_speed + static_cast<float>(value) * (0.9f + 0.2f*participants[0].TireEffectivness(current_field)));
	if (speed_estimation > static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25f)
	{
		speed_estimation = static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25f;
	}
	else if (speed_estimation < 0)
	{
		speed_estimation = 0;
	}
	float burned_durability = participants[0].CalculateBurning(speed_estimation - participants[0].car_modifiers[CarModifiers::max_speed]);
	speed_estimation *= GameValues::friction_scalar;
	bool drift = take_action_position == 2 && participants[0].current_speed > 40.0f && current_field.size() > 1;
	float chance_to_succeed = static_cast<float>(static_cast<int>((100.0f - participants[0].EvaluateChance(current_field, speed_estimation, drift))));
	float estimated_time = drift ? GameValues::drift_value : 100.0f / (1.0f + speed_estimation * 10.0f / 36.0f);
	
	HANDLE window = main_window->GetHandle();
	std::string helper;
	std::vector<std::pair<double, std::string>> values = { {chance_to_succeed, LanguagePack::vector_of_strings[LanguagePack::race_chances][0]},  {estimated_time, LanguagePack::vector_of_strings[LanguagePack::race_chances][1]},
														{burned_durability, LanguagePack::vector_of_strings[LanguagePack::race_chances][2]}, {speed_estimation, LanguagePack::vector_of_strings[LanguagePack::race_chances][3]} };

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
}
int SinglePlayer::Ranking(const bool clear)
{
	std::vector<std::string> text = LanguagePack::vector_of_strings[LanguagePack::race_ranking];
	int ret = 0;

	if (clear)
	{
		for (int i = 0; i < 8; ++i)
		{
			text.push_back(" ");
			text.push_back("                             ");
			text.push_back("         ");
		}
	}
	else
	{
		auto ranking_info = GetRankingInfo();
		for (int i = 0; i < static_cast<int>(ranking_info.size()); ++i)
		{
			text.push_back(std::to_string(i + 1));
			text.push_back(ranking_info[i].second);
			text.push_back(std::to_string(ranking_info[i].first));
			const int text_size = static_cast<int>(text.size());
			text[text_size - 1] = text[text_size - 1].substr(0, static_cast<int>(text[text_size - 1].size()) - 4);
			if (ranking_info[i].second == participants[0].name && ranking_info[i].first == participants[0].score)
				ret = i + 1;
		}
	}
	Text::TableText(text, 1, 3, 2, 16, { static_cast<short>(main_window->GetWidth() - 55), 16 }, *main_window, clear);
	return ret;	//returning players place
}
void SinglePlayer::Interface()
{
	const HANDLE window = main_window->GetHandle();
	const std::string possible_actions[5] = {
		LanguagePack::vector_of_strings[LanguagePack::race_interface][0] + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::max_accelerating])) + ")   ",
		LanguagePack::vector_of_strings[LanguagePack::race_interface][1] + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::max_braking])) + ")   ",
		LanguagePack::vector_of_strings[LanguagePack::race_interface][2] + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::hand_brake_value])) + "   ",
		LanguagePack::vector_of_strings[LanguagePack::race_interface][3],
		LanguagePack::vector_of_strings[LanguagePack::race_interface][4] };
	std::string speed = std::to_string(participants[0].current_speed);
	std::string durability = std::to_string(participants[0].current_durability);
	speed = speed.substr(0, static_cast<int>(speed.size()) - 4);
	durability = durability.substr(0, static_cast<int>(durability.size()) - 4);

	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << LanguagePack::vector_of_strings[LanguagePack::race_attribs][0];
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << participants[0].name;
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << LanguagePack::vector_of_strings[LanguagePack::race_attribs][1];
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << speed << "     ";
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 20 });
	std::cout << LanguagePack::vector_of_strings[LanguagePack::race_attribs][2];
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << durability << LanguagePack::vector_of_strings[LanguagePack::race_attribs][3];

	SetConsoleTextAttribute(window, 8);
	for (short i = 0; i < 5; ++i)
	{
		SetConsoleCursorPosition(window, { 2, 40 + i * 2 });
		std::cout << possible_actions[i];
	}
	const short x = static_cast<short>(main_window->GetWidth() - static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border].size()));
	const std::vector<std::tuple<std::string, COORD, short>> boxes = { {LanguagePack::vector_of_strings[LanguagePack::race_boxes][0], {0, static_cast<short>(main_window->GetHeight() - 15)}, 12 },
				{ LanguagePack::vector_of_strings[LanguagePack::race_boxes][1], { x, static_cast<short>(main_window->GetHeight() - 20) }, 17},
				{ LanguagePack::vector_of_strings[LanguagePack::race_boxes][2],  { x, static_cast<short>(main_window->GetHeight() - 31) }, 7 } };
	

	SetConsoleTextAttribute(window, main_window->color2);
	for (int i = 0; i < static_cast<int>(boxes.size()); ++i)
	{
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X + static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border].size())/2 - static_cast<short>(std::get<0>(boxes[i]).size())/2, std::get<1>(boxes[i]).Y});
		std::cout << std::get<0>(boxes[i]);
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + 1});
		std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border];
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + std::get<2>(boxes[i]) + 2 });
		std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::border];
	}
	ShowChances(0, true);
}
bool SinglePlayer::VisionBox(const int turn)
{
	const std::vector<std::string> visible_tour = main_window->GetTourParameters(GetTour(), turn, participants[0].car_modifiers[CarModifiers::visibility]);
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
	for (short i = 0; i < participants[0].car_modifiers[CarModifiers::visibility] && i < static_cast<short>(LanguagePack::vector_of_strings[LanguagePack::race_distance].size()); ++i)
	{
		if (static_cast<int>(visible_tour.size()) == i)
		{
			SetConsoleCursorPosition(window, { 1,24 + 2 * i });
			SetConsoleTextAttribute(window, main_window->color1);
			std::cout << LanguagePack::vector_of_strings[LanguagePack::race_distance][i];
			SetConsoleTextAttribute(window, main_window->color2);
			std::cout << LanguagePack::vector_of_strings[LanguagePack::other_string][OtherStrings::meta] + "                                              ";
			SetConsoleCursorPosition(window, { 1,26 + 2 * i });
			std::cout << "                                                   ";
			break;
		}
		helper = visible_tour[i];
		SetConsoleTextAttribute(window, main_window->color1);
		SetConsoleCursorPosition(window, { 1,24 + 2 *i });
		std::cout << LanguagePack::vector_of_strings[LanguagePack::race_distance][i];
		SetConsoleTextAttribute(window, main_window->color2);
		std::cout << LanguagePack::vector_of_strings[LanguagePack::race_infobox][(visible_tour[i][0] - 48) * 2 + (static_cast<int>(visible_tour[i].size()) > 1 ? 0 : 1)] << helper.erase(0, 1) << "                                               ";
	}
	return ret;
}