#include "NetworkRole.h"

std::vector<std::string> SinglePlayer::ReadFile(const std::string path)
{
	std::vector<std::string> data;
	std::fstream fvar;
	std::string helper;
	fvar.open(path);
	while (getline(fvar, helper) && helper != "")
	{
		data.push_back(helper);
	}
	fvar.close();
	return data;
}
std::string SinglePlayer::ChooseName(const std::string current_name, const int max_name_size)
{
	char button;
	std::string name = current_name;
	HANDLE handle = main_window->GetHandle();
	int name_size = static_cast<int>(name.size());

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
		name = "Racer";
	}
	return name;
}
int SinglePlayer::NumericalAction(const COORD coords)
{
	HANDLE window = main_window->GetHandle();
	char button;
	int value = 0;
	short decimal_position = 0;

	std::cout << ": ";
	while (true)
	{
		button = _getch();
		SetConsoleCursorPosition(window, {coords.X+2+decimal_position, coords.Y });
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
	return value * (static_cast<bool>(take_action_position)?-1:1);
}
int SinglePlayer::BinaryAction(const COORD coords)
{
	HANDLE window = main_window->GetHandle();
	char button;

	while (true)
	{
		ShowChances(participants[0].car_modifiers[CarModifiers::hand_brake_value] * -1 * static_cast<int>(take_action_position == 2));
		SetConsoleCursorPosition(window, coords);
		const std::string text = " - Do you really want to do this ? (Y/N) ";
		std::cout << text;
		button = _getch();
		SetConsoleCursorPosition(window, coords);
		for (int i = 0; i < static_cast<int>(text.size()); ++i)
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
SinglePlayer::SinglePlayer(ToT_Window &main_window)
{
	this->main_window = &main_window;
	COORD infobox_position = { 0,static_cast<short>(main_window.GetHeight() - 12) };
	this->infobox = std::make_shared<InfoBox>(10, Text::TextAlign::left, infobox_position, 1, main_window);
}
std::vector<std::string> SinglePlayer::GetTourNames()
{
	return this->ReadFile("Tour.txt");
}
std::vector<std::string> SinglePlayer::GetCarNames(const std::string tour)
{
	return this->ReadFile(tour);
}
std::vector<std::string> SinglePlayer::GetTireNames()
{
	return this->ReadFile("Tire.txt");
}
std::vector<int> SinglePlayer::GetCarParameters(const std::string path)
{
	std::vector<std::string> data = this->ReadFile(path);
	std::vector<int> car_parameters;
	for (int i = 0; i < static_cast<int>(data.size()); ++i)
	{
		car_parameters.push_back(atoi(data[i].c_str()));
	}
	return car_parameters;
}
std::vector<std::string> SinglePlayer::GetTireParameters(const std::string path)
{
	return this->ReadFile(path);
}
std::vector<std::string> SinglePlayer::GetTourParameters(const int position, const int visibility)
{
	std::vector<std::string> ret;
	std::fstream fvar;
	std::string helper;

	fvar.open(tour.c_str());
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
		ret.push_back(helper);
	}
	fvar.close();
	return ret;
}
void SinglePlayer::GetParticipants(const std::string name, const int ais, const std::string tour, const std::string car, const std::string tire)
{
	this->tour = tour;
	this->ais = ais;
	participants.emplace_back(name, car, tire, *this);

	for (int i = 0; i < ais; ++i)
	{
		participants.emplace_back(this, tour);
	}
}
std::vector<std::pair<float, std::string>> SinglePlayer::GetRankingInfo()
{
	bool flag = true;
	std::pair<float, std::string> helper;
	std::vector<std::pair<float, std::string>> ret;

	for (int i = 0; i < participants.size(); ++i)
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
		participants[i].Test(current_field, i < static_cast<int>(participants.size()) - ais);
		if (participants[i].current_durability <= 0.0f && participants[i].alive)
		{
			infobox->Push("RIP, " +participants[i].name + " dezintegrated his vehichle...", "");
			participants[i].alive = false;
			if (i == 0)
			{
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color1);
				SetConsoleCursorPosition(main_window->GetHandle(), { 0, 20 });
				std::cout << " Your vehice has ";
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color2);
				std::cout << static_cast<int>(participants[0].current_durability) << " durability   ";
				return false;
			}
		}
	}
	return true;
}
void SinglePlayer::Attack()
{
	std::vector<std::string> rival_name;
	std::vector<int> rival_id;
	HANDLE handle = main_window->GetHandle();
	const int forward_attack_distance = 4;
	const int backward_attack_distance = 6;
	std::multimap<float, Participant*> sorted_participants;

	rival_name.push_back("Don't attack");
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
	for (int i = static_cast<int>(participants.size()) - ais; i < static_cast<int>(participants.size()); ++i)
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
	const std::vector<std::string> actions = { "Speed up","Slow down","Hand-Brake","Do nothing","Abaddon Race" };
	int value;
	while (true)
	{
		take_action_position = Text::Choose::Veritcal(actions, take_action_position, { 1,39 }, 2, Text::TextAlign::left, false, *main_window);
		if (participants[0].current_speed == 0 && take_action_position % 4 != 0)
		{
			const std::string text = " - You can't do this because you aren't moving...";
			std::cout << text;
			main_window->Pause(1500);
			for (int i = 0; i < static_cast<int>(text.size()); ++i)
			{
				std::cout << "\b";
			}
			for (int i = 0; i < static_cast<int>(text.size()); ++i)
			{
				std::cout << " ";
			}
			continue;
		}
		if (take_action_position < 2)
		{
			value = NumericalAction({ static_cast<short>(actions[take_action_position].size()) + 1, 39 + 2 * take_action_position });
			if (value != 0)
			{
				break;
			}
		}
		else
		{
			int option = BinaryAction({ static_cast<short>(actions[take_action_position].size()) + 1, 39 + 2 * take_action_position });
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
	participants[0].current_speed += value;
	if (participants[0].current_speed < 0)
	{
		participants[0].current_speed = 0;
	}
	else if (participants[0].current_speed > static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed]))
	{
		if (participants[0].current_speed > static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25f)
		{
			participants[0].current_speed = static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed] * 1.25f);
		}
		participants[0].current_durability -= participants[0].CalculateBurning(participants[0].current_speed - participants[0].car_modifiers[CarModifiers::max_speed]);
	}
	participants[0].current_speed = participants[0].current_speed*0.9f;
}
void SinglePlayer::GetOthersAction(const int turn)
{
	const auto tour = GetTourParameters(turn, 7);
	int safe_speed = INT32_MAX;
	std::string helper;

	for (int i = static_cast<int>(participants.size()) - ais; i < static_cast<int>(participants.size()); ++i)
	{
		for (int j = 0; j < participants[i].car_modifiers[CarModifiers::visibility] && j < static_cast<int>(tour.size()); ++j)
		{
			if (static_cast<int>(tour[j].size()) > 1)
			{
				if (atoi(tour[j].substr(1, static_cast<int>(tour[j].size()) - 1).c_str()) + (j*participants[i].car_modifiers[CarModifiers::max_braking]) < safe_speed)
					safe_speed = atoi(tour[j].substr(1, static_cast<int>(tour[j].size()) - 1).c_str()) + (j*participants[i].car_modifiers[CarModifiers::max_braking]);
			}
		}
		participants[i].TakeAction(safe_speed, tour[0].size() > 1);
	}
}
int SinglePlayer::Possible_AIs()
{
	return 7;
}
void SinglePlayer::ShowChances(const int value, const bool reset)
{
	float speed_estimation = (participants[0].current_speed + static_cast<float>(value));
	if (speed_estimation > static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25f)
	{
		speed_estimation = static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25f;
	}
	else if (speed_estimation < 0)
	{
		speed_estimation = 0;
	}
	bool drift = take_action_position == 2 && participants[0].current_speed > 40.0f && current_field.size() > 1;
	float chance_to_succeed = static_cast<float>(static_cast<int>((100.0f - participants[0].EvaluateChance(current_field, speed_estimation*0.9f, drift))));
	float estimated_time = drift ? 1.5f : 100.0f / (1.0f + speed_estimation * 9.0f / 36.0f);
	float burned_durability = participants[0].CalculateBurning(speed_estimation - participants[0].car_modifiers[CarModifiers::max_speed]);

	HANDLE window = main_window->GetHandle();
	std::string helper;
	std::vector<std::pair<double, std::string>> values = { {chance_to_succeed, "Chance: "},  {estimated_time, "Estimated time: "}, {burned_durability, "Durability burning: "} };

	if (!reset)
	{
		for (short i = 0; i < values.size(); ++i)
		{
			if (values[i].first < 0.01)
			{
				values[i].first = 0;
			}
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 27 - i) });
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
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 27 - i) });
			std::cout << values[i].second << "                   ";
		}
		SetConsoleTextAttribute(window, main_window->color2);
	}
}
void SinglePlayer::ShowTiresParameters(const std::string tire_path, bool clear)
{
	std::vector<std::string> params2 = GetTireParameters(tire_path);
	std::vector<std::string> tire_parameters;
	std::vector<Text::OrdinaryText_atributes> text_atributes;
	for (int i = 0; i < static_cast<int>(Modifiers::tire_modifiers.size()); ++i)
	{
		text_atributes.push_back(Text::OrdinaryText_atributes::color2);
		text_atributes.push_back(Text::OrdinaryText_atributes::endl);
	}
	for (int i = 0; i < static_cast<int>(params2.size()); ++i)
	{
		tire_parameters.push_back(Modifiers::tire_modifiers[i] + ": ");
		tire_parameters.push_back(params2[i] + "      ");
	}
	Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 40, *main_window, clear);
}
void SinglePlayer::ShowCarParameters(const std::string car_path, const bool clear)
{
	std::vector<int> params1 = GetCarParameters(car_path);
	std::vector<std::string> car_parameters;
	std::vector<Text::OrdinaryText_atributes> text_atributes;
	for (int i = 0; i < static_cast<int>(Modifiers::car_modifiers.size()); ++i)
	{
		text_atributes.push_back(Text::OrdinaryText_atributes::color2);
		text_atributes.push_back(Text::OrdinaryText_atributes::endl);
	}
	for (int i = 0; i < static_cast<int>(params1.size()); ++i)
	{
		car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");
		car_parameters.push_back(std::to_string(params1[i]) + "      ");
	}
	Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 22, *main_window, clear);
}
void SinglePlayer::GameLobby()
{
	//Lobby parameters
	const std::vector<std::string> options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	COORD starting_point = { static_cast<short>(main_window->GetWidth()) / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	HANDLE handle = main_window->GetHandle();
	std::string name = "Racer";
	const std::vector<std::string> tours = GetTourNames();
	const std::vector<std::string> tires = GetTireNames();
	std::vector<std::string> cars = GetCarNames(tours[0]);
	int ais = 0;
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;

	if (static_cast<int>(tours.size()) == 0)
	{
		MessageBox(0, "Cannot load any map files", "error", 0);
		return;
	}
	if (static_cast<int>(tires.size()) == 0)
	{
		MessageBox(0, "Cannot load any tire files", "error", 0);
		return;
	}
	if (static_cast<int>(cars.size()) == 0)
	{
		MessageBox(0, "Cannot load any car from selected tour", "error", 0);
		return;
	}
	ShowCarParameters(cars[cars_pos]);
	ShowTiresParameters(tires[tires_pos]);

	while (main_menu_position != 5 || static_cast<int>(cars.size()) == 0)
	{
		switch (main_menu_position = Text::Choose::Veritcal(options, main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window))
		{
			case 0://choosing name
			{
				name = ChooseName(name, 14);
				break;
			}
			case 1://Number of ais
			{
				std::vector<std::string> text;

				for (int i = 0; i <= Possible_AIs(); ++i)
				{
					text.push_back(std::to_string(i));
				}
				ais = Text::Choose::Horizontal(text, ais, { starting_point.X + static_cast<short>(options[main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
				break;
			}
			case 2://choosing tour
			{
				int i = tours_pos;
				tours_pos = Text::Choose::Horizontal(tours, tours_pos, { starting_point.X + static_cast<short>(options[main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
				if (i != tours_pos)
				{
					cars = GetCarNames(tours[tours_pos]);
					cars_pos = 0;
					ShowCarParameters(cars[cars_pos]);
					break;
				}
				break;
			}
			case 3://choosing car
			{
				cars = GetCarNames(tours[tours_pos]);
				if (static_cast<int>(cars.size()) == 0)
				{
					MessageBoxA(0, "Cannot load any car from selected tour", "error", 0);
				}
				else
				{
					cars_pos = Text::Choose::Horizontal(cars, cars_pos, { starting_point.X + static_cast<short>(options[main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
					ShowCarParameters(cars[cars_pos]);
					break;
				}
			}
			case 4://choosing tires
			{
				tires_pos = Text::Choose::Horizontal(tires, tires_pos, { starting_point.X + static_cast<short>(options[main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
				ShowTiresParameters(tires[tires_pos]);
				break;
			}
		}
	}
	for (int i = 0; i < static_cast<int>(options.size()); ++i)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(options[i].size())), starting_point.Y + static_cast<short>(i * spacing) });
		for (int j = 0; j < static_cast<int>(options[i].size()); ++j)
		{
			std::cout << " ";
		}
	}
	ShowCarParameters(tires[tires_pos], true);
	ShowCarParameters(cars[cars_pos], true);
	GetParticipants(name, ais, tours[tours_pos], cars[cars_pos], tires[tires_pos]);
}
int SinglePlayer::Ranking(const bool clear)
{
	std::vector<std::string> text = { "PLACE", "RACER", "SCORE" };
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
	HANDLE window = main_window->GetHandle();
	const std::string possible_actions[5] = {
		"You can speed up by (0 to " + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::max_accelerating])) + ")   ",
		"You can slow down by (0 to " + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::max_braking])) + ")   ",
		"Hand brake value is - " + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::hand_brake_value])) + "   ",
		"You can just wait if you're moving...",
		"Remember only weak ones surrenders" };
	std::string speed = std::to_string(participants[0].current_speed);
	std::string durability = std::to_string(participants[0].current_durability);
	speed = speed.substr(0, static_cast<int>(speed.size()) - 4);
	durability = durability.substr(0, static_cast<int>(durability.size()) - 4);

	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << " You're racing as: ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << participants[0].name;
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << " Your current speed is: ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << speed << "     ";
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 20 });
	std::cout << " Your vehice has ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << durability << " durability   ";

	SetConsoleTextAttribute(window, 8);
	for (short i = 0; i < 5; ++i)
	{
		SetConsoleCursorPosition(window, { 2, 40 + i * 2 });
		std::cout << possible_actions[i];
	}
	const std::string border = "________________________________________________________  ";
	const std::vector<std::tuple<std::string, COORD, short>> boxes = { {"Infobox  ", {0, static_cast<short>(main_window->GetHeight() - 15)}, 12 },
				{ "Attack box", {static_cast<short>(main_window->GetWidth() - static_cast<short>(border.size())), static_cast<short>(main_window->GetHeight() - 20) }, 17},
				{"Chance of succeeding",  { static_cast<short>(main_window->GetWidth() - static_cast<short>(border.size())), static_cast<short>(main_window->GetHeight() - 31) }, 7 } };
	

	SetConsoleTextAttribute(window, main_window->color2);
	for (int i = 0; i < static_cast<int>(boxes.size()); ++i)
	{
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X + static_cast<short>(border.size())/2 - static_cast<short>(std::get<0>(boxes[i]).size())/2, std::get<1>(boxes[i]).Y});
		std::cout << std::get<0>(boxes[i]);
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + 1});
		std::cout << border;
		SetConsoleCursorPosition(window, { std::get<1>(boxes[i]).X, std::get<1>(boxes[i]).Y + std::get<2>(boxes[i]) + 2 });
		std::cout << border;
	}
}
bool SinglePlayer::VisionBox(const int turn)
{
	const std::vector<std::string> visible_tour = GetTourParameters(turn, participants[0].car_modifiers[CarModifiers::visibility]);
	int ret = true;
	HANDLE window = main_window->GetHandle();
	std::string helper;
	const std::vector<std::string> distance = { "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ",
								"At a considerable distance: ", "Far ahead: ", "Barely noticeable: " };

	if (visible_tour.size() > 0)
	{
		current_field = visible_tour[0];
	}
	else
	{
		current_field = "0";
		ret = false;
	}
	for (short i = 0; i < participants[0].car_modifiers[CarModifiers::visibility] && i < static_cast<short>(distance.size()); ++i)
	{
		if (static_cast<int>(visible_tour.size()) == i)
		{
			SetConsoleCursorPosition(window, { 1,24 + 2 * i });
			SetConsoleTextAttribute(window, main_window->color1);
			std::cout << distance[i];
			SetConsoleTextAttribute(window, main_window->color2);
			std::cout << "META                                              ";
			SetConsoleCursorPosition(window, { 1,26 + 2 * i });
			std::cout << "                                                   ";
			break;
		}
		helper = visible_tour[i];
		SetConsoleTextAttribute(window, main_window->color1);
		SetConsoleCursorPosition(window, { 1,24 + 2 *i });
		std::cout << distance[i];
		SetConsoleTextAttribute(window, main_window->color2);
		switch (visible_tour[i][0])
		{
			case '0':
			{
				if (static_cast<int>(visible_tour[i].size()) > 1)
				{
					std::cout << "Turn on asphalt - safe speed is around " << helper.erase(0, 1) << "          ";
				}
				else
				{
					std::cout << "Straight road, terrain is asphalt" << "                    ";
				}
				break;
			}
			case '1':
			{
				if (static_cast<int>(visible_tour[i].size()) > 1)
				{
					std::cout << "Turn on grass - safe speed is around " << helper.erase(0, 1) << "          ";
				}
				else
				{
					std::cout << "Straight grassy road" << "                    ";
				}
				break;
			}
			case '2':
			{
				if (static_cast<int>(visible_tour[i].size()) > 1)
				{
					std::cout << "Turn on gravel - safe speed is around " << helper.erase(0, 1) << "          ";
				}
				else
				{
					std::cout << "Straight road, gravel" << "                    ";
				}
				break;
			}
			case '3':
			{
				if (static_cast<int>(visible_tour[i].size()) > 1)
				{
					std::cout << "Turn on sand - safe speed is around " << helper.erase(0, 1) << "          ";
				}
				else
				{
					std::cout << "Linear part of tour, but sandy" << "                    ";
				}
				break;
			}
			case '4':
			{
				if (static_cast<int>(visible_tour[i].size()) > 1)
				{
					std::cout << "Turn on mud - safe speed is around " << helper.erase(0, 1) << "          ";
				}
				else
				{
					std::cout << "Muddy road, without turn" << "                    ";
				}
				break;
			}
			case '5':
			{
				if (static_cast<int>(visible_tour[i].size()) > 1)
				{
					std::cout << "Turn on ice - safe speed is around " << helper.erase(0, 1) << "          ";
				}
				else
				{
					std::cout << "Slippery road with ice" << "                    ";
				}
				break;
			}
		}
	}
	return ret;
}