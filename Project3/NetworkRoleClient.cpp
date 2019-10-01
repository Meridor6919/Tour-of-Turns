#include "NetworkRole.h"
#include <map>
#include <string>

Client::Client(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->participants = participants;
	this->main_window = &main_window;
	COORD infobox_position = { 0,static_cast<short>(main_window.GetHeight() - 12) };
	this->infobox = std::make_shared<InfoBox>(10, Text::TextAlign::left, infobox_position, 1, main_window);
	stage = 0;
	if (!StartNetwork())
	{
		closesocket(host);
		throw 1;
	}
}
Client::~Client()
{
	//TODO disconnects
	char buffer[254] = "60";
	send(host, buffer, 254, 0);
	while ((std::string)buffer != "exit")
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetTireNames method failed", "Error", 0);
			return;
		}
	}
	closesocket(host);
}
bool Client::StartNetwork()
{
	HANDLE handle = main_window->GetHandle();
	COORD starting_point = { (short)main_window->GetWidth() / 2, 25 };
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
	char buffer[254] = "01";
	send(host, buffer, 254, 0);

	if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
	{
		MessageBox(0, "GetTourNames method failed", "Error", 0);
		return;
	}
	tours.push_back(buffer);//host will always send only one tour (the tour that he choosed)
}
void Client::GetCarNames(std::vector<std::string>&cars, std::string tour)
{
	char buffer[254] = "02";
	send(host, buffer, 254, 0);
	cars.clear();

	while (true)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetCarNames method failed", "Error", 0);
			return;
		}
		if ((std::string)buffer != "exit")
			cars.push_back((std::string)buffer);
		else
			break;
	} 
}
void Client::GetTireNames(std::vector<std::string>&tires)
{
	char buffer[254] = "03";
	send(host, buffer, 254, 0);
	tires.clear();

	while (true)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetTireNames method failed", "Error", 0);
			return;
		}
		if ((std::string)buffer != "exit")
			tires.push_back((std::string)buffer);
		else
			break;
	} 
}
std::vector<std::string> Client::GetTourParameters(std::string path)
{

	char buffer[254] = "04";
	send(host, buffer, 254, 0);
	std::vector<std::string>ret;

	while (true)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetTourParameter method failed", "Error", 0);
			ret.clear();
			return ret;
		}
		if ((std::string)buffer != "exit")
			ret.push_back((std::string)buffer);
		else
			break;
	}
	return ret;
}
std::vector<int> Client::GetCarParameters(std::string path)
{
	char buffer[254];

	strcpy(buffer, ("05" + path).c_str());
	send(host, buffer, 254, 0);

	std::vector<int> ret;
	for (int i = 0; i < 8; ++i)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetCarParameters method failed", "Error", 0);
			ret.clear();
			return ret;
		}
		ret.push_back(std::atoi(((std::string)buffer).c_str()));
	}
	return ret;
}
std::vector<std::string> Client::GetTireParameters(std::string path)
{
	char buffer[254];
	
	strcpy(buffer, ("06" + path).c_str());
	send(host, buffer, 254, 0);

	std::vector<std::string> ret;

	for (int i = 0; i < 6; i++)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetTireParameters method failed", "Error", 0);
			ret.clear();
			return ret;
		}
		ret.push_back(buffer);
	}
	tire_path = path;
	return ret;
}
std::vector<std::pair<float, std::string>> Client::GetRankingInfo(std::string current_field)
{
	this->current_field = current_field;
	char buffer[254] = "0";
	float fhelper;
	std::vector<std::pair<float, std::string>> ret = {};
	std::chrono::milliseconds ms(30);
	
	while (atoi(buffer) < stage + 1)
	{
		std::this_thread::sleep_for(ms);
		strcpy(buffer, "10");
		send(host, buffer, 254, 0);
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetRankingInfo method failed", "Error", 0);
			ret.clear();
			return ret;
		}
	}
	stage++;
	strcpy(buffer, "07");
	send(host, buffer, 254, 0);

	while (true)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetRankingInfo method failed", "Error", 0);
			ret.clear();
			return ret;
		}
		if ((std::string)buffer != "exit")
		{
			fhelper = atof(static_cast<std::string>(buffer).c_str());
			if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
			{
				MessageBox(0, "GetRankingInfo method failed", "Error", 0);
				ret.clear();
				return ret;
			}
			ret.push_back(std::make_pair(fhelper, (std::string)buffer));
		}
		else
		{
			return ret;
		}
	}
}
void Client::Attack()
{
	if (!participants[0].alive)
		return;
	char buffer[254] = "08";
	send(host, buffer, 254, 0);
	std::vector<std::string> id;
	std::vector<std::string> options;

	while (true)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetTireNames method failed", "Error", 0);
			return;
		}
		if ((std::string)buffer != "exit")
		{
			options.push_back((std::string)buffer);
			if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
			{
				MessageBox(0, "GetTireNames method failed", "Error", 0);
				return;
			}
			id.push_back((std::string)buffer);
		}
		else
			break;
	}
	
	short i = Text::Choose::Veritcal(options, 0, { static_cast<short>(main_window->GetWidth() - 28), static_cast<short>(main_window->GetHeight() - 17) }, 2, Text::TextAlign::center, true, *main_window);
	strcpy(buffer, ("54" + id[i]).c_str());
	send(host, buffer, 254, 0);
}
bool Client::GetCurrentAtribs()
{

	char buffer[254] = "0";
	std::chrono::milliseconds ms(30);

	while (atoi(buffer) < stage + 1)
	{
		std::this_thread::sleep_for(ms);
		strcpy(buffer, "10");
		send(host, buffer, 254, 0);
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
			return false;
		}
	}

	strcpy(buffer, "09");
	send(host, buffer, 254, 0);

	if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
	{
		MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
		return false;
	}
	participants[0].current_speed = atof(buffer);

	if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
	{
		MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
		return false;
	}
	participants[0].current_durability = atof(buffer);


	if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
	{
		MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
		return false;
	}
	participants[0].score = atof(buffer);

	(*infobox).info.clear();
	while (true)
	{
		if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
		{
			MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
			return false;
		}
		
		if ((std::string)buffer != "exit")
		{
			std::string helper = ((std::string)buffer).c_str();
			if (!GeneralMultiPlayer::Recv(host, buffer, 254, 0))
			{
				MessageBox(0, "GetCurrentAtribs method failed", "Error", 0);
				return false;
			}
			(*infobox).Push(helper, (std::string)buffer);
		}
		else
			break;
	}
	if (participants[0].current_durability <= 0.0f)
	{
		participants[0].alive = false;
		return false;
	}
	else
	{
		return true;
	}
}
void Client::GetParticipants(std::string name, std::string tour, std::string car, std::string tire)
{
	SinglePlayer::GetParticipants(name, tour, car, tire);

	//client don't need to know stats of other participants but he need to tell host what have he choosed
	char buffer[254];
	strcpy(buffer, ("51" + participants[0].name).c_str());
	send(host, buffer, 254, 0);

	strcpy(buffer, ("52" + participants[0].car_path).c_str());
	send(host, buffer, 254, 0);

	strcpy(buffer, ("53" + tire_path).c_str());
	send(host, buffer, 254, 0);
	return; 
}
void Client::TakeAction()
{
	if (!participants[0].alive)
		return;

	char buffer[254];

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
			if (participants[0].current_speed == 0 && position == 1)
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
			value = Text::Choose::Numeric(participants[0].car_modifiers[CarModifiers::max_accelerating + position], { console_screen_buffer_info.dwCursorPosition.X, console_screen_buffer_info.dwCursorPosition.Y }, true, *main_window);
			SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 2,console_screen_buffer_info.dwCursorPosition.Y });
			std::cout << "  ";
			if (value == 0)
				break;

			strcpy(buffer, ("55" + std::to_string(position) + std::to_string(value)).c_str());
			send(host, buffer, 254, 0);

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
					if (participants[0].current_speed > 0 || position == 4)
					{
						strcpy(buffer, ("55" + std::to_string(position)).c_str());
						send(host, buffer, 254, 0);
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
void Client::GetOthersAction(std::vector<std::string>& tour)
{
	return;
}
int Client::Possible_AIs()
{
	return 0;
}