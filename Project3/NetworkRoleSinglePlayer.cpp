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
		std::string text = " - Do you really want to do this ? (Y/N) ";
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
	participants.emplace_back(Participant(name, car, tire, *this));

	for (int i = 0; i < ais; ++i)
	{
		participants.emplace_back(Participant(this, tour));
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
			ret.push_back(std::make_pair(participants[i].score, participants[i].name));
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
		int i = Text::Choose::Veritcal(rival_name, 0, { static_cast<short>(main_window->GetWidth() - 28), static_cast<short>(main_window->GetHeight()-17) }, 2, Text::TextAlign::center, true, *main_window);
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
			value = NumericalAction({ static_cast<short>(actions[take_action_position].size() + 1), 39 + 2 * take_action_position });
			if (value != 0)
			{
				break;
			}
		}
		else
		{
			int option = BinaryAction({ static_cast<short>(actions[take_action_position].size() + 1), 39 + 2 * take_action_position });
			if (option == 0)
			{
				continue;
			}
			else if (option == 2)
			{
				value = 0;
				break;
			}
			else if (option == 3)
			{
				value = participants[0].car_modifiers[CarModifiers::hand_brake_value] * -1;
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
		if (participants[0].current_speed > static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25)
		{
			participants[0].current_speed = static_cast<float>(participants[0].car_modifiers[CarModifiers::max_speed] * 1.25);
		}
		participants[0].current_durability -= CalculateBurning(participants[0].current_speed - participants[0].car_modifiers[CarModifiers::max_speed]);
	}
	participants[0].current_speed = participants[0].current_speed*0.9f;
}
void SinglePlayer::GetOthersAction(int turn)
{
	auto tour = GetTourParameters(turn, 7);
	int safe_speed = INT32_MAX;
	std::string helper;

	for (int i = static_cast<int>(participants.size()) - ais; i < static_cast<int>(participants.size()); i++)
	{
		for (int j = 0; j < participants[i].car_modifiers[CarModifiers::visibility] && j < tour.size(); j++)
		{
			if (tour[j].size() > 1)
			{
				if (atoi(tour[j].substr(1, tour[j].size() - 1).c_str()) + (j*participants[i].car_modifiers[CarModifiers::max_braking]) < safe_speed)
					safe_speed = atoi(tour[j].substr(1, tour[j].size() - 1).c_str()) + (j*participants[i].car_modifiers[CarModifiers::max_braking]);
			}
		}
		participants[i].TakeAction(safe_speed, (bool)tour[0].size > 1);
	}
}
int SinglePlayer::Possible_AIs()
{
	return 7;
}
void SinglePlayer::ShowChances(int value, bool reset)
{
	double speed_estimation = (participants[0].current_speed + value);
	if (speed_estimation > static_cast<double>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25)
		speed_estimation = static_cast<double>(participants[0].car_modifiers[CarModifiers::max_speed])*1.25;
	else if (speed_estimation < 0)
		speed_estimation = 0;
	
	bool drift = take_action_position == 2 && participants[0].current_speed > 40 && current_field.size() > 1;
	float chance_to_succeed = static_cast<int>((100 - participants[0].EvaluateChance(current_field, speed_estimation*0.9, drift)));
	float estimated_time = drift ? 1.5 : 100 / (1 + speed_estimation * 9.0f / 36.0f);
	float burned_durability = CalculateBurning(speed_estimation - participants[0].car_modifiers[CarModifiers::max_speed]);

	HANDLE window = main_window->GetHandle();
	std::string helper;
	std::vector<std::pair<double, std::string>> values = { {chance_to_succeed, "Chance: "},  {estimated_time, "Estimated time: "}, {burned_durability, "Durability burning: "} };

	if (!reset)
	{
		for (short i = 0; i < values.size(); i++)
		{
			if (values[i].first < 0.01)
				values[i].first = 0;
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 27 - i) });
			SetConsoleTextAttribute(window, main_window->color1);
			std::cout << values[i].second;
			SetConsoleTextAttribute(window, main_window->color2);
			helper = std::to_string(values[i].first);
			helper = helper.substr(0, helper.size() - 4);
			std::cout << helper << "              ";
		}
	}
	else
	{
		for (short i = 0; i < values.size(); i++)
		{
			SetConsoleTextAttribute(window, main_window->color1);
			SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 27 - i) });
			std::cout << values[i].second << "                   ";
		}
		SetConsoleTextAttribute(window, main_window->color2);
	}
}
double SinglePlayer::CalculateBurning(double value)
{
	if (value < 0)
		return 0;
	double raw = value / participants[0].car_modifiers[CarModifiers::max_speed];

	if (raw > 0.25)
	{
		raw = 0.25;
		value = participants[0].car_modifiers[CarModifiers::max_speed] * 0.25;
	}

	double result = 0;
	for (int i = 1; i < raw / 0.05 + 3; i++)
	{
		result += i * value;
	}
	return result/10;
}
void SinglePlayer::GameLobby()
{
	//Lobby parameters
	std::vector<std::string> options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	COORD starting_point = { (short)main_window->GetWidth() / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	const int max_name_size = 14;
	HANDLE handle = main_window->GetHandle();

	//Player related variables
	std::string name = "Racer";
	std::vector<std::string> tours;
	std::vector<std::string> cars;
	std::vector<std::string> tires;
	std::string tour_path;
	std::string tire_path;
	std::string car_path;
	int ais = 0;
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;

	tours = GetTourNames();
	if (tours.size() == 0)
	{
		MessageBox(0, "Cannot load any map files", "error", 0);
		return;
	}
	tires = GetTireNames();
	if (tires.size() == 0)
	{
		MessageBox(0, "Cannot load any tire files", "error", 0);
		return;
	}
	cars = GetCarNames(tours[0]);
	if (cars.size() == 0)
		MessageBox(0, "Cannot load any car from selected tour", "error", 0);
	else
		car_path = cars[0];

	tour_path = tours[0];
	tire_path = tires[0];

	//TODO validating files
	std::vector<std::string> car_parameters;
	std::vector<std::string> tire_parameters;
	std::vector<Text::OrdinaryText_atributes> text_atributes;

	//preparing vectors for OrdinaryText function
	for (int i = 0; i < Modifiers::car_modifiers->size(); i++)
	{
		text_atributes.push_back(Text::OrdinaryText_atributes::color2);
		text_atributes.push_back(Text::OrdinaryText_atributes::endl);
	}

	//showing car parameters
	std::vector<int> params1 = GetCarParameters(car_path);
	for (int i = 0; i < params1.size(); i++)
	{
		car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");
		car_parameters.push_back(std::to_string(params1[i]));
	}
	Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 22, *main_window);

	//showing tire parameters
	auto params2 = GetTireParameters(tire_path);
	for (int i = 0; i < params2.size(); i++)
	{
		tire_parameters.push_back(Modifiers::tire_modifiers[i] + ": ");//atrib names:							
		tire_parameters.push_back(params2[i]);
	}
	Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 40,*main_window);

	//menu segment
	while (main_menu_position != 5 || cars.size() == 0)
	{
		switch (main_menu_position = Text::Choose::Veritcal(options, main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window))
		{
		case 0://choosing name
		{
			char button;
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << " < ";
			SetConsoleTextAttribute(handle, main_window->color2);
			std::cout << name;
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << " >";

			SetConsoleTextAttribute(handle, main_window->color2);
			int size = static_cast<int>(name.size());

			do
			{
				button = _getch();

				if (size < max_name_size && ((button >= 48 && button <= 57) || (button >= 65 && button <= 90 && GetKeyState(VK_SHIFT) != 1 && GetKeyState(VK_SHIFT) != 0) || (button >= 97 && button <= 122)))
				{
					name += button;
					SetConsoleTextAttribute(handle, main_window->color2);
					std::cout << "\b\b" << button;
					SetConsoleTextAttribute(handle, main_window->color1);
					std::cout << " >";
					size++;
				}
				else if (size < max_name_size && button == 32 && size != 0 && name[size - 1] != 32)
				{
					name += ' ';
					SetConsoleTextAttribute(handle, main_window->color2);
					std::cout << "\b\b";
					SetConsoleTextAttribute(handle, main_window->color1);
					std::cout << " >";
					size++;
				}
				else if (button == 8 && size > 0)
				{
					name.erase(size - 1, 1);
					SetConsoleTextAttribute(handle, main_window->color1);
					std::cout << "\b\b\b   \b\b>";
					size--;
				}

			} while (button != 13);
			for (int i = 0; i < size + 4; i++)
				std::cout << "\b";
			for (int i = 0; i < size + 4; i++)
				std::cout << " ";
			if (name.size() == 0)
				name = "Racer";
			break;
		}
		case 1://Number of ais
		{
			std::vector<std::string> text;

			for (int i = 0; i <= Possible_AIs(); i++)
				text.push_back(std::to_string(i));

			ais = Text::Choose::Horizontal(text, ais, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
			break;
		}
		case 2://choosing tour
		{
			int i = tours_pos;
			tour_path = tours[tours_pos = Text::Choose::Horizontal(tours, tours_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
			if (i != tours_pos)
			{
				cars = GetCarNames(tour_path);
				cars_pos = 0;
				car_path = cars[cars_pos];
				car_parameters.clear();
				params1 = GetCarParameters(car_path);
				for (int i = 0; i < params1.size(); i++)
				{
					car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");
					car_parameters.push_back(std::to_string(params1[i]) + "   ");
				}
				Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 22, *main_window);
				break;
			}
			break;
		}
		case 3://choosing car
		{
			cars = GetCarNames(tour_path);
			if (cars.size() == 0)
				MessageBoxA(0, "Cannot load any car from selected tour", "error", 0);
			else
			{
				car_path = cars[cars_pos = Text::Choose::Horizontal(cars, cars_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
				car_parameters.clear();
				params1 = GetCarParameters(car_path);
				for (int i = 0; i < params1.size(); i++)
				{
					car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");
					car_parameters.push_back(std::to_string(params1[i]) + "   ");
				}
				Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 22, *main_window);
				break;
			}
		}
		case 4://choosing tires
		{
			tire_path = tires[tires_pos = Text::Choose::Horizontal(tires, tires_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
			tire_parameters.clear();
			params2 = GetTireParameters(tire_path);
			for (int i = 0; i < params2.size(); i++)
			{
				tire_parameters.push_back(Modifiers::tire_modifiers[i] + ": ");
				tire_parameters.push_back(params2[i] + "   ");
			}
			Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 40, *main_window);
			break;
		}
		}
	}
	//clearing menu afterwards
	for (short i = 0; i < options.size(); i++)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::TextAlign::center / 2 * (float)options[i].size()), starting_point.Y + i * (short)spacing });
		for (int j = 0; j < options[i].size(); j++)
			std::cout << " ";
	}

	//clearing params segment
	Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 40, *main_window, true);
	Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 22, *main_window, true);

	//loading player

	//loading clients/ais
	GetParticipants(name, ais, tour_path, car_path, tire_path);
}
int SinglePlayer::Ranking(bool clear)
{
	std::vector<std::string> text;
	text.push_back("PLACE");
	text.push_back("RACER");
	text.push_back("SCORE");
	int ret = 0;

	if (clear)
	{
		for (int i = 0; i < 8; i++)
		{
			text.push_back(" ");
			text.push_back("                             ");
			text.push_back("         ");

		}
	}
	else
	{
		auto ranking_info = GetRankingInfo();
		for (int i = 0; i < ranking_info.size(); i++)
		{
			text.push_back(std::to_string(i + 1));
			text.push_back(ranking_info[i].second);
			text.push_back(std::to_string(ranking_info[i].first));
			text[text.size() - 1] = text[text.size() - 1].substr(0, text[text.size() - 1].size() - 4);
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

	std::string possible_actions[5] = {
		"You can speed up by (0 to " + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::max_accelerating])) + ")   ",
		"You can slow down by (0 to " + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::max_braking])) + ")   ",
		"Hand brake value is - " + std::to_string(static_cast<int>(participants[0].car_modifiers[CarModifiers::hand_brake_value])) + "   ",
		"You can just wait if you're moving...",
		"Remember only weak ones surrenders" };

	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << " You're racing as: ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << participants[0].name;
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << " Your current speed is: ";
	SetConsoleTextAttribute(window, main_window->color2);

	std::string speed = std::to_string(participants[0].current_speed);
	std::string durability = std::to_string(participants[0].current_durability);
	speed = speed.substr(0, speed.size() - 4);
	durability = durability.substr(0, durability.size() - 4);

	std::cout << speed << "     ";
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 20 });
	std::cout << " Your vehice has ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << durability << " durability   ";

	SetConsoleTextAttribute(window, 8);
	for (int i = 0; i < 5; i++)
	{
		SetConsoleCursorPosition(window, { 2, static_cast<short>(40 + i * 2) });
		std::cout << possible_actions[i];
	}
	SetConsoleCursorPosition(window, { 0, static_cast<short>(main_window->GetHeight() - 15) });
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << "                          Infobox                         " << std::endl;
	std::cout << "----------------------------------------------------------";
	SetConsoleCursorPosition(window, { 0, static_cast<short>(main_window->GetHeight() - 1) });
	std::cout << "----------------------------------------------------------";

	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 34), static_cast<short>(main_window->GetHeight() - 20) });
	std::cout << "Attack Box";
	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 19) });
	std::cout << "---------------------------------------------";
	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 1) });
	std::cout << "---------------------------------------------";

	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 39), static_cast<short>(main_window->GetHeight() - 31) });
	std::cout << "Chance of succeeding";
	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 30) });
	std::cout << "---------------------------------------------";
	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), static_cast<short>(main_window->GetHeight() - 22) });
	std::cout << "---------------------------------------------";
}
bool SinglePlayer::VisionBox(int turn)
{
	auto visible_tour = GetTourParameters(turn, visibility);
	int ret = true;

	if (visible_tour.size() > 0)
	{
		current_field = visible_tour[0];
	}
	else
	{
		current_field = "0";
		ret = false;
	}

	HANDLE window = main_window->GetHandle();
	std::string helper;
	std::string Distance[] = { "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ",
								"At a considerable distance: ", "Far ahead: ", "Barely noticeable: " };

	for (int i = 0; i < participants[0].car_modifiers[CarModifiers::visibility] && i < 7; i++)
	{
		if (visible_tour.size() == i)
		{
			SetConsoleCursorPosition(window, { 1,static_cast<short>(24 + 2 * i) });
			SetConsoleTextAttribute(window, main_window->color1);
			std::cout << Distance[i];
			SetConsoleTextAttribute(window, main_window->color2);
			std::cout << "META                                              ";
			SetConsoleCursorPosition(window, { 1,static_cast<short>(26 + 2 * i) });
			std::cout << "                                                   ";
			break;
		}

		helper = visible_tour[i];

		SetConsoleTextAttribute(window, main_window->color1);
		SetConsoleCursorPosition(window, { 1,static_cast<short>(24 + 2 * i) });
		std::cout << Distance[i];
		SetConsoleTextAttribute(window, main_window->color2);

		switch (visible_tour[i][0])
		{
		case '0':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on asphalt - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight road, terrain is asphalt" << "                    ";
			break;
		}
		case '1':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on grass - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight grassy road" << "                    ";
			break;
		}
		case '2':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on gravel - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight road, gravel" << "                    ";
			break;
		}
		case '3':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on sand - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Linear part of tour, but sandy" << "                    ";
			break;
		}
		case '4':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on mud - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Muddy road, without turn" << "                    ";
			break;
		}
		case '5':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on ice - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Slippery road with ice" << "                    ";
			break;
		}
		}
	}
	return ret;
}