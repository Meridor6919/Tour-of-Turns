#include "NetworkRole.h"

SinglePlayer::SinglePlayer(ToT_Window &main_window, std::vector<Participant*> *participants)
{
	this->participants = participants;
	this->main_window = &main_window;
	this->infobox = new InfoBox(10, Text::TextAlign::left, { 0,56 }, 1, main_window);
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
std::vector<std::pair<float, std::string>> SinglePlayer::GetRankingInfo()
{
	std::vector<std::pair<float, std::string>> ret;

	std::vector<float> scores;
	std::vector<std::string> names;

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
bool SinglePlayer::GetCurrentAtribs(int real_players, std::string field)
{
	for (int i = 0; i < (*participants).size(); i++)
		(*participants)[i]->Test(field, i < real_players);

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
		int i = Text::Choose::Veritcal(rival_name, 0, { static_cast<short>(main_window->GetWidth() - 28), 51 }, 2, Text::TextAlign::center, true, *main_window);
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
	static int position = 0;
	HANDLE window = main_window->GetHandle();
	char button;
	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
	
	while (true)
	{
		int value = 0;
		switch (position = Text::Choose::Veritcal(actions, position, { 1,39 }, 2, Text::TextAlign::left, false, *main_window))
		{
			case 0:
			case 1:
			{
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
				GetConsoleScreenBufferInfo(window, &console_screen_buffer_info);
				value = Text::Choose::Numeric((*participants)[0]->car_modifiers[CarModifiers::max_accelerating + position], {console_screen_buffer_info.dwCursorPosition.X, console_screen_buffer_info.dwCursorPosition.Y}, true, *main_window);
				SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 2,console_screen_buffer_info.dwCursorPosition.Y });
				std::cout << "  ";
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
					if ((*participants)[0]->current_speed > static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed]))
						(*participants)[0]->current_speed = static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::max_speed]);
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
				std::cout << " - Do you really want to do this ? (Y/N) ";
				button = _getch();
				GetConsoleScreenBufferInfo(window, &console_screen_buffer_info);
				SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 41,console_screen_buffer_info.dwCursorPosition.Y });
				std::cout << "                                         ";
				SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 41,console_screen_buffer_info.dwCursorPosition.Y });
				if (button == 'y' || button == 'Y')
				{
					if (position == 4)
					{
						(*participants)[0]->current_durability = 0;
						return;
					}


					if ((*participants)[0]->current_speed > 0)
					{
						if (position == 2)
						{
							if ((*participants)[0]->current_speed > 40)
								(*participants)[0]->drift = true;
							(*participants)[0]->current_speed -= static_cast<float>((*participants)[0]->car_modifiers[CarModifiers::hand_brake_value]);
							if ((*participants)[0]->current_speed < 0)
								(*participants)[0]->current_speed = 0.0f;
						}
						(*participants)[0]->current_speed = (*participants)[0]->current_speed*0.9f;
						return;
					}
					else
					{
						std::cout << " - You can't do this because you aren't moving...";
						main_window->Pause(1500);
						GetConsoleScreenBufferInfo(window, &console_screen_buffer_info);
						SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 49,console_screen_buffer_info.dwCursorPosition.Y });
						std::cout << "                                                 ";
						break;
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
		for (int j = 0; j < (*participants)[i]->car_modifiers[CarModifiers::visibility]; j++)
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