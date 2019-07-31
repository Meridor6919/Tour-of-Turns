#include "NetworkRole.h"

SinglePlayer::SinglePlayer(ToT_Window &main_window)
{
	this->main_window = &main_window;
	this->infobox = new InfoBox(10, Text::TextAlign::left, { 0,static_cast<short>(main_window.GetHeight()-12) }, 1, main_window);
}
void SinglePlayer::GetTourNames(std::vector<std::string>&tours)
{
	std::fstream fvar;
	std::string helper;
	fvar.open("Tour.txt");
	while (getline(fvar, helper))
		tours.push_back(helper);
}
void SinglePlayer::GetCarNames(std::vector<std::string>&cars, std::string tour)
{
	cars.clear();
	std::fstream fvar;
	std::string helper;
	fvar.open(tour.c_str());
	while (getline(fvar, helper) && helper != "")
		cars.push_back(helper);
}
void SinglePlayer::GetTireNames(std::vector<std::string>&tires)
{
	std::fstream fvar;
	std::string helper;
	fvar.open("Tire.txt");
	while (getline(fvar, helper))
		tires.push_back(helper);
}
std::vector<int> SinglePlayer::GetCarParameters(std::string path)
{
	std::fstream fvar;
	std::string helper;
	std::vector<int> car_parameters;
	fvar.open(path.c_str(), std::ios::in);
	while (std::getline(fvar, helper))
		car_parameters.push_back(std::atoi(helper.c_str()));

	return car_parameters;
}
std::vector<std::string> SinglePlayer::GetTireParameters(std::string path)
{
	std::fstream fvar;
	std::string helper;
	std::vector<std::string> tire_parameters;
	fvar.open(path.c_str(), std::ios::in);
	while (std::getline(fvar, helper))
		tire_parameters.push_back(helper);

	return tire_parameters;
}
void SinglePlayer::GetParticipants(std::string name, int ais, std::string tour, std::string car, std::string tire)
{
	this->tour = tour;
	this->ais = ais;
	participants.emplace_back(new Participant(name, car, tire, *this));

	for (int i = 0; i < ais; i++)
		participants.emplace_back(new Participant(this, tour));
}
std::vector<std::string> SinglePlayer::GetTourParameters(int position, int visibility)
{
	std::vector<std::string> ret = {};
	std::fstream fvar;
	std::string helper;

	fvar.open(tour.c_str());
	do
	{ 
		std::getline(fvar, helper);
	}
	while (helper != "");
	for (int i = 0; i < position; i++)
	{
		std::getline(fvar, helper);
	}
	for (int i = 0; i < visibility; i++)
	{
		if (!std::getline(fvar, helper))
			break;
		ret.push_back(helper);
	}
	fvar.close();

	return ret;
}
std::vector<std::pair<float, std::string>> SinglePlayer::GetRankingInfo()
{
	std::vector<std::pair<float, std::string>> ret;
	std::vector<float> scores;
	std::vector<std::string> names;

	for (int i = 0; i < participants.size(); i++)
	{
		if(participants[i]->alive)
			ret.push_back(std::make_pair(participants[i]->score, participants[i]->name));
	}

	if (ret.size() < 2)
		return ret;

	bool flag = true;
	float fhelper;
	std::string shelper;

	while (flag)
	{
		flag = false;
		for (int i = 0; i < ret.size() - 1; i++)
		{
			if (ret[i].first > ret[i+1].first)
			{
				flag = true;
				fhelper = ret[i].first;
				shelper = ret[i].second;
				ret[i].first = ret[i+1].first;
				ret[i].second = ret[i+1].second;
				ret[i+1].first = fhelper;
				ret[i+1].second = shelper;
			}
		}
	}
	return ret;
}
bool SinglePlayer::GetCurrentAtribs()
{
	int real_players = static_cast<int>(participants.size()) - ais;

	for (int i = 0; i < participants.size(); i++)
		participants[i]->Test(current_field, i < real_players);

	for (int i = participants.size() - 1; i >= 0; i--)
	{
		if (participants[i]->current_durability <= 0.0f && participants[i]->alive)
		{
			infobox->Push("RIP, " +participants[i]->name + " dezintegrated his vehichle...", "");
			participants[i]->alive = false;
			if (i == 0)
			{
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color1);
				SetConsoleCursorPosition(main_window->GetHandle(), { 0, 20 });
				std::cout << " Your vehice has ";
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color2);
				std::cout << static_cast<int>(participants[0]->current_durability) << " durability   ";
				return false;
			}
		}
	}
	return true;
}
void SinglePlayer::Attack()
{
	//TODO make it more profitable to be first - being able attack from more distance
	//TODO change attack mechanic in straight sections
	//TODO there should be a drawback when you 
	int ais = static_cast<int>(participants.size()) - static_cast<int>(participants[0]->alive);
	std::vector<std::string> rival_name;
	std::vector<int> rival_id;
	HANDLE handle = main_window->GetHandle();

	rival_name.push_back("Don't attack");
	rival_id.push_back(10);
	participants[0]->attacked = 0;
	for (int i = 1; i < participants.size(); i++)
	{
		if (participants[i]->score < participants[0]->score + 5 && participants[i]->score >participants[0]->score - 5 && participants[i]->alive)
		{
			rival_name.push_back(participants[i]->name);
			rival_id.push_back(i);
		}
		participants[i]->attacked = 0;
	}
	if (rival_id.size() != 1 && participants[0]->alive)
	{
		int i = Text::Choose::Veritcal(rival_name, 0, { static_cast<short>(main_window->GetWidth() - 28), static_cast<short>(main_window->GetHeight()-17) }, 2, Text::TextAlign::center, true, *main_window);
		if (rival_id[i] != 10)
			participants[rival_id[i]]->attacked++;
	}

	for (int i = static_cast<int>(participants.size()) - ais; i < static_cast<int>(participants.size()); i++)
	{
		rival_id.clear();
		rival_id.push_back(10);
		for (int j = 0; j < participants.size(); j++)
		{
			if (participants[j]->score < participants[i]->score + 5 &&participants[j]->score >participants[i]->score - 5 && participants[j]->alive)
			{
				if (participants[i] != participants[j])
					rival_id.push_back(j);
			}
		}

		int random = rand() % rival_id.size();
		if(rival_id[random] != 10)
			participants[rival_id[random]]->attacked++;
	}
}
void SinglePlayer::TakeAction()
{
	if (!participants[0]->alive)
		return;

	//player
	std::vector<std::string> actions = { "Speed up","Slow down","Hand-Brake","Do nothing","Abaddon Race" };
	static short position = 0;
	HANDLE window = main_window->GetHandle();
	char button;
	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
	
	while (true)
	{
		int value = 0;
		ShowChances(0, 0, 0, true);
		switch (position = Text::Choose::Veritcal(actions, position, { 1,39 }, 2, Text::TextAlign::left, false, *main_window))
		{
			case 0:
			case 1:
			{
				//TODO durablility should affect max speed
				//TODO add an option to bypass max speed by burning durability
				if (participants[0]->current_speed == 0 && position == 1)
				{
					std::cout << " - You can't do this because you aren't moving...";
					main_window->Pause(1500);
					GetConsoleScreenBufferInfo(window, &console_screen_buffer_info);
					SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 49,console_screen_buffer_info.dwCursorPosition.Y });
					std::cout << "                                                 ";
					break;
				}
				
				std::cout << ": ";
				char button;
				short pos = 0;
				while (true)//while player presses enter
				{
					button = _getch();
					SetConsoleCursorPosition(window, {(short)actions[position].size()+3 + pos,39+2*position});
					if (button >= '0' && button <= '9')
					{
						if (button == '0' && pos == 0)
							continue;
						else if (value * 10 + button - 48 > participants[0]->car_modifiers[CarModifiers::max_accelerating + position])
							continue;

						std::cout << button;
						value = value * 10 + button - 48;
						pos++;
					}
					else if (button == '\b' && pos != 0)
					{
						std::cout << "\b \b";
						value /= 10;
						pos--;
					}
					else if (button == 13)
					{
						break;
					}

					if (value > 0)
					{
						double speed_estimation = (participants[0]->current_speed + value * (position > 0 ? -1 : 1));
						if (speed_estimation > static_cast<double>(participants[0]->car_modifiers[CarModifiers::max_speed])*1.25)
							speed_estimation = static_cast<double>(participants[0]->car_modifiers[CarModifiers::max_speed])*1.25;
						else if (speed_estimation < 0)
							speed_estimation = 0;

						ShowChances((int)(100 - participants[0]->EvaluateChance(current_field, speed_estimation*0.9, false)),
							(100 / (1 + speed_estimation*9.0f / 36.0f)),
							CalculateBurning(speed_estimation - participants[0]->car_modifiers[CarModifiers::max_speed]));
					}
					else
					{
						ShowChances(0, 0, 0, true);
					}
				}
				SetConsoleCursorPosition(window, { (short)actions[position].size()+1,39 + 2 * position });
				for (int i = 0; i < pos+2 ; i++)
					std::cout << " ";

				if (value == 0)
					break;
				
				if (position)
				{
					participants[0]->current_speed -= value;
					if (participants[0]->current_speed < 0)
						participants[0]->current_speed = 0;
				}
				else
				{
					participants[0]->current_speed += value;
				}
				if (participants[0]->current_speed > static_cast<float>(participants[0]->car_modifiers[CarModifiers::max_speed]))
				{
					if (participants[0]->current_speed > static_cast<float>(participants[0]->car_modifiers[CarModifiers::max_speed])*1.25)
						participants[0]->current_speed = static_cast<float>(participants[0]->car_modifiers[CarModifiers::max_speed]*1.25);

					participants[0]->current_durability -= CalculateBurning(participants[0]->current_speed - participants[0]->car_modifiers[CarModifiers::max_speed]);
				}
				participants[0]->current_speed = participants[0]->current_speed*0.9f;
				return;		
			}
		case 2:
		case 3:
		case 4:
		{
			while (true)
			{
				if (participants[0]->current_speed <= 0)
				{
					std::cout << " - You can't do this because you aren't moving...";
					main_window->Pause(1500);
					GetConsoleScreenBufferInfo(window, &console_screen_buffer_info);
					SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 49,console_screen_buffer_info.dwCursorPosition.Y });
					std::cout << "                                                 ";
					break;
				}
				if (position == 2)
				{
					double speed_estimation = participants[0]->current_speed - participants[0]->car_modifiers[CarModifiers::hand_brake_value];
					if (speed_estimation < 0)
						speed_estimation = 0;

					ShowChances((int)(100 - participants[0]->EvaluateChance(current_field, speed_estimation*0.9, participants[0]->current_speed > 40 && current_field.size() > 1)),
						participants[0]->current_speed > 40 && current_field.size()>1 ? 1.5 : 100 / (1 + speed_estimation*9.0f / 36.0f),
						CalculateBurning(speed_estimation - participants[0]->car_modifiers[CarModifiers::max_speed]));
				}
				else if (position == 3)
				{
					double speed_estimation = participants[0]->current_speed;
					if (speed_estimation < 0)
						speed_estimation = 0;

					ShowChances((int)(100 - participants[0]->EvaluateChance(current_field, participants[0]->current_speed*0.9, false)),
						(100 / (1 + speed_estimation*9.0f / 36.0f)),
						CalculateBurning(speed_estimation - participants[0]->car_modifiers[CarModifiers::max_speed]));
				}
				SetConsoleCursorPosition(window, { (short)actions[position].size() + 1,39 + 2 * position });
				std::cout << " - Do you really want to do this ? (Y/N) ";
				button = _getch();
				SetConsoleCursorPosition(window, { (short)actions[position].size() + 1,39 + 2 * position });
				std::cout << "                                         ";
				if (button == 'y' || button == 'Y')
				{
					if (position == 4)
					{
						participants[0]->current_durability = 0;
						return;
					}
					else
					{
						if (position == 2)
						{
							if (participants[0]->current_speed > 40 && current_field.size() > 1)
								participants[0]->drift = true;
							participants[0]->current_speed -= static_cast<float>(participants[0]->car_modifiers[CarModifiers::hand_brake_value]);
							if (participants[0]->current_speed < 0)
								participants[0]->current_speed = 0.0f;
						}
						if (participants[0]->current_speed > static_cast<float>(participants[0]->car_modifiers[CarModifiers::max_speed]))
						{
							if (participants[0]->current_speed > static_cast<float>(participants[0]->car_modifiers[CarModifiers::max_speed])*1.25)
								participants[0]->current_speed = static_cast<float>(participants[0]->car_modifiers[CarModifiers::max_speed] * 1.25);
							participants[0]->current_durability -= CalculateBurning(participants[0]->current_speed - participants[0]->car_modifiers[CarModifiers::max_speed]);
						}
						participants[0]->current_speed = participants[0]->current_speed*0.9f;
						return;
					}
				}
				else if (button == 13 || button == 27 || button == 'n' || button == 'N')
				{
					break;
				}
			}
		}
		}
	}
}
void SinglePlayer::GetOthersAction(int turn)
{
	auto tour = GetTourParameters(turn, 7);
	int safe_speed = INT32_MAX;
	std::string helper;

	for (int i = static_cast<int>(participants.size()) - ais; i < static_cast<int>(participants.size()); i++)
	{
		for (int j = 0; j < participants[i]->car_modifiers[CarModifiers::visibility] && j < tour.size(); j++)
		{
			if (tour[j].size() > 1)
			{
				if (atoi(tour[j].substr(1, tour[j].size() - 1).c_str()) + (j*participants[i]->car_modifiers[CarModifiers::max_braking]) < safe_speed)
					safe_speed = atoi(tour[j].substr(1, tour[j].size() - 1).c_str()) + (j*participants[i]->car_modifiers[CarModifiers::max_braking]);
			}
		}
		participants[i]->TakeAction(safe_speed, (bool)tour[0].size > 1);
	}
}
int SinglePlayer::Possible_AIs()
{
	return 7;
}

void SinglePlayer::ShowChances(double chance_to_succeed, double estimated_time, double burned_durability, bool reset)
{
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
	double raw = value / participants[0]->car_modifiers[CarModifiers::max_speed];

	if (raw > 0.25)
	{
		raw = 0.25;
		value = participants[0]->car_modifiers[CarModifiers::max_speed] * 0.25;
	}

	double result = 0;
	for (int i = 1; i < raw / 0.05 + 3; i++)
	{
		result += i * value;
	}
	return result/10;
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
			if (ranking_info[i].second == participants[0]->name && ranking_info[i].first == participants[0]->score)
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
		"You can speed up by (0 to " + std::to_string(static_cast<int>(participants[0]->car_modifiers[CarModifiers::max_accelerating])) + ")   ",
		"You can slow down by (0 to " + std::to_string(static_cast<int>(participants[0]->car_modifiers[CarModifiers::max_braking])) + ")   ",
		"Hand brake value is - " + std::to_string(static_cast<int>(participants[0]->car_modifiers[CarModifiers::hand_brake_value])) + "   ",
		"You can just wait if you're moving...",
		"Remember only weak ones surrenders" };

	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << " You're racing as: ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << participants[0]->name;
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << " Your current speed is: ";
	SetConsoleTextAttribute(window, main_window->color2);

	std::string speed = std::to_string(participants[0]->current_speed);
	std::string durability = std::to_string(participants[0]->current_durability);
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

	for (int i = 0; i < participants[0]->car_modifiers[CarModifiers::visibility] && i < 7; i++)
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