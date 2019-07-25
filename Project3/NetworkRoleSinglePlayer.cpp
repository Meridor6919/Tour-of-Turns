#include "NetworkRole.h"

SinglePlayer::SinglePlayer(ToT_Window &main_window, std::vector<Participant*> *participants)
{
	this->participants = participants;
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
void SinglePlayer::GetOtherParticipants(int ais, std::string tour)
{
	for (int i = 0; i < ais; i++)
		(*participants).emplace_back(new Participant((*participants)[i]->network_role, tour));
}
std::vector<std::string> SinglePlayer::GetTourParameters(std::string path)
{
	std::vector<std::string> ret;
	std::fstream fvar;
	std::string helper;

	fvar.open(path.c_str());
	do
	{ 
		std::getline(fvar, helper);
	}
	while (helper != "");
	while (std::getline(fvar, helper))
	{
		ret.push_back(helper);
	}
	fvar.close();

	return ret;
}
std::vector<std::pair<float, std::string>> SinglePlayer::GetRankingInfo(std::string current_field)
{
	std::vector<std::pair<float, std::string>> ret;
	std::vector<float> scores;
	std::vector<std::string> names;

	this->current_field = current_field;

	for (int i = 0; i < (*participants).size(); i++)
	{
		if((*participants)[i]->alive)
			ret.push_back(std::make_pair((*participants)[i]->score, (*participants)[i]->name));
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
bool SinglePlayer::GetCurrentAtribs(int real_players)
{
	for (int i = 0; i < (*participants).size(); i++)
		(*participants)[i]->Test(current_field, i < real_players);

	for (int i = (*participants).size() - 1; i >= 0; i--)
	{
		if ((*participants)[i]->current_durability <= 0.0f && (*participants)[i]->alive)
		{
			infobox->Push("RIP, " + (*participants)[i]->name + " dezintegrated his vehichle...", "");
			(*participants)[i]->alive = false;
			if (i == 0)
			{
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color1);
				SetConsoleCursorPosition(main_window->GetHandle(), { 0, 20 });
				std::cout << " Your vehice has ";
				SetConsoleTextAttribute(main_window->GetHandle(), main_window->color2);
				std::cout << static_cast<int>((*participants)[0]->current_durability) << " durability   ";
				return false;
			}
		}
	}
	return true;
}
void SinglePlayer::Attack(int ais)
{
	//TODO make it more profitable to be first - being able attack from more distance
	//TODO change attack mechanic in straight sections
	//TODO there should be a drawback when you attack
	std::vector<std::string> rival_name;
	std::vector<int> rival_id;
	HANDLE handle = main_window->GetHandle();

	rival_name.push_back("Don't attack");
	rival_id.push_back(10);
	(*participants)[0]->attacked = 0;
	for (int i = 1; i < (*participants).size(); i++)
	{
		if ((*participants)[i]->score < (*participants)[0]->score + 5 && (*participants)[i]->score >(*participants)[0]->score - 5 && (*participants)[i]->alive)
		{
			rival_name.push_back((*participants)[i]->name);
			rival_id.push_back(i);
		}
		(*participants)[i]->attacked = 0;
	}
	if (rival_id.size() != 1 && (*participants)[0]->alive)
	{
		int i = Text::Choose::Veritcal(rival_name, 0, { static_cast<short>(main_window->GetWidth() - 28), static_cast<short>(main_window->GetHeight()-17) }, 2, Text::TextAlign::center, true, *main_window);
		if (rival_id[i] != 10)
			(*participants)[rival_id[i]]->attacked++;
	}

	for (int i = static_cast<int>((*participants).size()) - ais; i < static_cast<int>((*participants).size()); i++)
	{
		rival_id.clear();
		rival_id.push_back(10);
		for (int j = 0; j < (*participants).size(); j++)
		{
			if ((*participants)[j]->score < (*participants)[i]->score + 5 && (*participants)[j]->score >(*participants)[i]->score - 5 && (*participants)[j]->alive)
			{
				if ((*participants)[i] != (*participants)[j])
					rival_id.push_back(j);
			}
		}

		int random = rand() % rival_id.size();
		if(rival_id[random] != 10)
			(*participants)[rival_id[random]]->attacked++;
	}
}
void SinglePlayer::TakeAction()
{
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
				if ((*participants)[0]->current_speed == 0 && position == 1)
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
						else if (value * 10 + button - 48 > (*participants)[0]->car_modifiers[CarModifiers::max_accelerating + position])
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
						double speed_estimation = ((*participants)[0]->current_speed + value * (position > 0 ? -1 : 1));
						if (speed_estimation > static_cast<double>((*participants)[0]->car_modifiers[CarModifiers::max_speed])*1.25)
							speed_estimation = static_cast<double>((*participants)[0]->car_modifiers[CarModifiers::max_speed])*1.25;
						else if (speed_estimation < 0)
							speed_estimation = 0;

						ShowChances((int)(100 - (*participants)[0]->EvaluateChance(current_field, speed_estimation*0.9, false)),
							(100 / (1 + speed_estimation*9.0f / 36.0f)),
							CalculateBurning(speed_estimation - (*participants)[0]->car_modifiers[CarModifiers::max_speed]));
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
					(*participants)[0]->current_speed -= value;
					if ((*participants)[0]->current_speed < 0)
						(*participants)[0]->current_speed = 0;
				}
				else
				{
					(*participants)[0]->current_speed += value;
				}
				if ((*participants)[0]->current_speed > static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed]))
				{
					if ((*participants)[0]->current_speed > static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed])*1.25)
						(*participants)[0]->current_speed = static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed]*1.25);

					(*participants)[0]->current_durability -= CalculateBurning((*participants)[0]->current_speed - (*participants)[0]->car_modifiers[CarModifiers::max_speed]);
				}
				(*participants)[0]->current_speed = (*participants)[0]->current_speed*0.9f;
				return;		
			}
		case 2:
		case 3:
		case 4:
		{
			while (true)
			{
				if ((*participants)[0]->current_speed <= 0)
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
					double speed_estimation = (*participants)[0]->current_speed - (*participants)[0]->car_modifiers[CarModifiers::hand_brake_value];
					if (speed_estimation < 0)
						speed_estimation = 0;

					ShowChances((int)(100 - (*participants)[0]->EvaluateChance(current_field, speed_estimation*0.9, (*participants)[0]->current_speed > 40 && current_field.size() > 1)),
						(*participants)[0]->current_speed > 40 && current_field.size()>1 ? 1.5 : 100 / (1 + speed_estimation*9.0f / 36.0f),
						CalculateBurning(speed_estimation - (*participants)[0]->car_modifiers[CarModifiers::max_speed]));
				}
				else if (position == 3)
				{
					double speed_estimation = (*participants)[0]->current_speed;
					if (speed_estimation < 0)
						speed_estimation = 0;

					ShowChances((int)(100 - (*participants)[0]->EvaluateChance(current_field, (*participants)[0]->current_speed*0.9, false)),
						(100 / (1 + speed_estimation*9.0f / 36.0f)),
						CalculateBurning(speed_estimation - (*participants)[0]->car_modifiers[CarModifiers::max_speed]));
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
						(*participants)[0]->current_durability = 0;
						return;
					}
					else
					{
						if (position == 2)
						{
							if ((*participants)[0]->current_speed > 40 && current_field.size() > 1)
								(*participants)[0]->drift = true;
							(*participants)[0]->current_speed -= static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::hand_brake_value]);
							if ((*participants)[0]->current_speed < 0)
								(*participants)[0]->current_speed = 0.0f;
						}
						if ((*participants)[0]->current_speed > static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed]))
						{
							if ((*participants)[0]->current_speed > static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed])*1.25)
								(*participants)[0]->current_speed = static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed] * 1.25);
							(*participants)[0]->current_durability -= CalculateBurning((*participants)[0]->current_speed - (*participants)[0]->car_modifiers[CarModifiers::max_speed]);
						}
						(*participants)[0]->current_speed = (*participants)[0]->current_speed*0.9f;
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
void SinglePlayer::GetOthersAction(int ais, std::vector<std::string> &tour)
{
	int safe_speed = INT32_MAX;
	std::string helper;

	for (int i = static_cast<int>((*participants).size()) - ais; i < static_cast<int>((*participants).size()); i++)
	{
		for (int j = 0; j < (*participants)[i]->car_modifiers[CarModifiers::visibility] && j < tour.size(); j++)
		{
			if (tour[j].size() > 1)
			{
				if (atoi(tour[j].substr(1, tour[j].size() - 1).c_str()) + (j*(*participants)[i]->car_modifiers[CarModifiers::max_braking]) < safe_speed)
					safe_speed = atoi(tour[j].substr(1, tour[j].size() - 1).c_str()) + (j*(*participants)[i]->car_modifiers[CarModifiers::max_braking]);
			}
		}
		(*participants)[i]->TakeAction(safe_speed, (bool)tour[0].size > 1);
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
	double raw = value / (*participants)[0]->car_modifiers[CarModifiers::max_speed];

	if (raw > 0.25)
	{
		raw = 0.25;
		value = (*participants)[0]->car_modifiers[CarModifiers::max_speed] * 0.25;
	}

	double result = 0;
	for (int i = 1; i < raw / 0.05 + 3; i++)
	{
		result += i * value;
	}
	return result/10;
}