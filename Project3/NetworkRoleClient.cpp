#include "NetworkRole.h"
#include <map>
#include <string>

Client::Client(ToT_Window &main_window, std::vector<Participant*> *participants) : SinglePlayer(main_window, participants)
{
	this->participants = participants;
	this->main_window = &main_window;
	this->infobox = new InfoBox(10, Text::TextAlign::left, { 0,56 }, 1, main_window);
	if (!StartNetwork())
	{
		closesocket(host);
		throw 1;
	}
	
}
bool Client::StartNetwork()
{
	HANDLE handle = main_window->GetHandle();
	COORD starting_point = { (short)main_window->GetWidth() / 2, 20 };
	short cursor_pos = 0;
	char button = ' ';
	std::map<int, std::string>::iterator  it;
	std::map<int, std::string> current_hosts;


	client = new GeneralMultiPlayer::Client(&host);
	current_hosts = client->GetCurrentHosts();
	current_hosts.insert(std::make_pair(MAXINT, "back"));

	//recv hosts from local network
	std::thread receiving_broadcast([&]() {
		if (!client->RecvBroadcast(8, 200))
		{
			return false;
		}
	});

	auto show_options = [&]() {
		it = current_hosts.begin();
		SetConsoleTextAttribute(handle, main_window->color1);
		for (short i = 0; it != current_hosts.end(); ++it, i++)
		{
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it->second.size()), starting_point.Y + i * 2 });
			std::cout << it->second;
		}
		it = current_hosts.begin();
		std::advance(it, cursor_pos);
		SetConsoleTextAttribute(handle, main_window->color2);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it->second.size()), starting_point.Y + cursor_pos * 2 });
		std::cout << it->second;
	};
	
	show_options();
	do
	{
		if (_kbhit())
		{
			button = _getch();

			if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
			{

				cursor_pos += 1;
				if (cursor_pos == current_hosts.size())
					cursor_pos = 0;
			}
			else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
			{
				cursor_pos -= 1;
				if (cursor_pos < 0)
					cursor_pos = static_cast<int>(current_hosts.size()) - 1;
			}
		}
		current_hosts = client->GetCurrentHosts();
		current_hosts.insert(std::make_pair(MAXINT, "back"));
		show_options();
		Sleep(60);
		if (button == 13)
		{
			client->FinishBroadcast();
			receiving_broadcast.join();
			it = current_hosts.begin();
			for (short i = 0; it != current_hosts.end(); ++it, i++)
			{
				SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it->second.size()), starting_point.Y + i * 2 });
				for (int j = 0; j < it->second.size(); j++)
					std::cout << ' ';
			}
			it = current_hosts.begin();
			std::advance(it, cursor_pos);

			if (it->first == MAXINT)
			{
				//back button selected
				return false;
			}
			else if (!client->Connect(client->GetIpFromMapValue(it->second)))
			{
				//connecton error
				return false;
			}
			else
			{
				//succesful multiplayer initialization
				return true;
			}
		}
	} while (true);
}
void Client::GetTourNames(std::vector<std::string>&tours)
{
	tours.clear();
	std::string get_tour_name_code = "50";
	send(host, get_tour_name_code.c_str(), 3, 0);
	char temp[255];

	if (recv(host, temp, 255, 0) < 0)
		MessageBox(0, "GetTourNames method failed", "Error", 0);

	tours.push_back(temp);//host will always send only one tour (the tour that he choosed)
}
void Client::GetCarNames(std::vector<std::string>&cars, std::string tour)
{
	std::string get_tour_name_code = "51";
	send(host, get_tour_name_code.c_str(), 3, 0);
	char temp[255];
	cars.clear();

	while (true)
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetCarNames method failed", "Error", 0);
		
		if ((std::string)temp != "exit")
			cars.push_back((std::string)temp);
		else
			break;
	} 
}
void Client::GetTireNames(std::vector<std::string>&tires)
{
	std::string get_tour_name_code = "52";
	send(host, get_tour_name_code.c_str(), 3, 0);
	char temp[255];
	tires.clear();

	while (true)
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetTireNames method failed", "Error", 0);

		if ((std::string)temp != "exit")
			tires.push_back((std::string)temp);
		else
			break;
	} 
}
std::vector<int> Client::GetCarParameters(std::string path)
{
	std::string get_tour_name_code = "54"+path;

	send(host, get_tour_name_code.c_str(), get_tour_name_code.size()+1, 0);
	char temp[255];

	std::vector<int> ret;
	for (int i = 0; i < 8; ++i)
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetCarParameters method failed", "Error", 0);

		ret.push_back(std::atoi(((std::string)temp).c_str()));
	}

	return ret;
}
std::vector<std::string> Client::GetTireParameters(std::string path)
{
	std::string get_tour_name_code = "55"+path;
	send(host, get_tour_name_code.c_str(), get_tour_name_code.size(), 0);
	char temp[255];

	std::vector<std::string> ret;

	for (int i = 0; i < 6; i++)
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetTireParameters method failed", "Error", 0);

		ret.push_back(temp);
	}
	tire_path = path;
	return ret;
}
std::vector<std::string> Client::GetTourParameters(std::string path)
{

	std::string get_tour_name_code = "53";
	send(host, get_tour_name_code.c_str(), 3, 0);
	char temp[255];
	std::vector<std::string>ret;

	while (true)
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetTourParameter method failed", "Error", 0);

		if ((std::string)temp != "exit")
			ret.push_back((std::string)temp);
		else
			break;
	}
	return ret;
}
void Client::GetOtherParticipants(int ais, std::string tour)
{
	//client don't need to know stats of other participants but he need to tell host what have he choosed
	std::string get_tour_name_code = "59";
	send(host, get_tour_name_code.c_str(), 4, 0);
	send(host, (*participants)[0]->name.c_str(), 255, 0);
	send(host,(* participants)[0]->car_path.c_str(), 255, 0);
	send(host, tire_path.c_str(), 255, 0);
	return; 
}
std::vector<std::pair<float, std::string>> Client::GetRankingInfo()
{
	if (!(*participants)[0]->current_durability)
	{
		std::vector<std::pair<float, std::string>> ret;
		ret.clear();
		return ret;
	}
	std::string get_tour_name_code = "61";
	send(host, get_tour_name_code.c_str(), 4, 0);
	char temp[255];
	float fhelper;
	std::vector<std::pair<float, std::string>> ret = {};

	while (true)
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetRankingInfo method failed", "Error", 0);

		if ((std::string)temp != "exit")
		{
			fhelper = atof(static_cast<std::string>(temp).c_str());
			if (!recv(host, temp, 255, 0) < 0)
				MessageBox(0, "GetRankingInfo method failed", "Error", 0);
			ret.push_back(std::make_pair(fhelper, (std::string)temp));
		}
		else
			break;
	}
	return ret;
}
bool Client::GetCurrentAtribs(int ais, std::string field)
{
	std::string get_tour_name_code = "63";
	send(host, get_tour_name_code.c_str(), 4, 0);
	char temp[255];

		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
		(*participants)[0]->current_speed = atof(temp);

		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
		(*participants)[0]->current_durability = atof(temp);

		if (!(*participants)[0]->current_durability)
		{
			send(host, "", 1, 0);
			closesocket(host);
			return false;
		}

		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
		(*participants)[0]->score = atof(temp);


		while (true)
		{
			if (!recv(host, temp, 255, 0) < 0)
				MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);

			if ((std::string)temp != "exit")
			{
				std::string helper = ((std::string)temp).c_str();
				if (!recv(host, temp, 255, 0) < 0)
					MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
				(*infobox).Push(helper, (std::string)temp);
			}
			else
				break;
		}
		return true;
}
void Client::Attack(int ais)
{
	if (!(*participants)[0]->current_durability)
		return;
	std::string get_tour_name_code = "62";
	send(host, get_tour_name_code.c_str(), 4, 0);
	char temp[255];
	std::vector<std::string> id;
	std::vector<std::string> options;

	while (true)
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetTireNames method failed", "Error", 0);

		if ((std::string)temp != "exit")
		{
			options.push_back((std::string)temp);
			if (!recv(host, temp, 255, 0) < 0)
				MessageBox(0, "GetTireNames method failed", "Error", 0);
			id.push_back((std::string)temp);
		}
		else
			break;
	}

	int i = Text::Choose::Veritcal(options, 0, { static_cast<short>(main_window->GetWidth() - 28), 51 }, 2, Text::TextAlign::center, true, *main_window);	
	send(host, id[i].c_str(), 4, 0);
}
void Client::TakeAction()
{
	std::string get_tour_name_code = "7";
	char temp[255];

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
			value = Text::Choose::Numeric((*participants)[0]->car_modifiers[CarModifiers::max_accelerating + position], { console_screen_buffer_info.dwCursorPosition.X, console_screen_buffer_info.dwCursorPosition.Y }, true, *main_window);
			SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 2,console_screen_buffer_info.dwCursorPosition.Y });
			std::cout << "  ";
			if (value == 0)
				break;

			get_tour_name_code += std::to_string(position) + std::to_string(value);
			send(host, get_tour_name_code.c_str(), 8, 0);

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
					if ((*participants)[0]->current_speed > 0 || position == 4)
					{
						get_tour_name_code += std::to_string(position);
						send(host, get_tour_name_code.c_str(), 8, 0);
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
void Client::GetOthersAction(int ais, std::vector<std::string>& tour)
{

}
int Client::Possible_AIs()
{
	return 0;
}