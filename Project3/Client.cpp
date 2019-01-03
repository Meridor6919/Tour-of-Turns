#include "NetworkRole.h"
#include <map>
#include <string>

Client::Client(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	if (!StartNetwork())
	{
		closesocket(host);
		throw 1;
	}
}
bool Client::StartNetwork()
{
	bool thread_active = true;

	short cursor_pos = 0;
	HANDLE handle = main_window->GetHandle();
	COORD starting_point = { (short)main_window->GetWidth() / 2, 20 };
	std::map<int, std::string>::iterator  it;
	SOCKET intercept_brodcast_socket;

	std::map<int, std::string> name_ip;
	name_ip.insert(std::make_pair(20, "refresh"));
	name_ip.insert(std::make_pair(21, "back"));

	//recv hosts from local network
	std::thread thread([&]() {

		int recv_time = 100;
		intercept_brodcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (intercept_brodcast_socket < 0)
		{
			MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}
		if (setsockopt(intercept_brodcast_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&recv_time), sizeof(recv_time)) < 0)
		{
			MessageBox(0, ("Socket option error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}

		sockaddr_in addr;
		int addr_size = sizeof(addr);
		memset(&addr, 0, addr_size);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(6919);	//port number
		addr.sin_addr.s_addr = INADDR_ANY;

		if (bind(intercept_brodcast_socket, (sockaddr*)&addr, addr_size))
		{
			MessageBox(0, ("Binding error"+ std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			WSACleanup();
			exit(0);
		}

		char recv_buffer[50];

		while (thread_active)
		{
			int recived = recvfrom(intercept_brodcast_socket, recv_buffer, 50, 0, (sockaddr *)&addr, &addr_size);

			in_addr addr2;
			hostent *hostent = gethostbyname(recv_buffer);
			if (hostent != nullptr && recived != -1)
			{
				for (int i = 0; hostent->h_addr_list[i] != 0; ++i)
					memcpy(&addr2, hostent->h_addr_list[i], sizeof(struct in_addr));

				if (name_ip.size() < 10)
				{
					std::string value = (std::string)recv_buffer + " - " + (std::string)inet_ntoa(addr2);
					bool finded = false;
					for (int i = 2; i < static_cast<int>(name_ip.size()); i++)
					{
						if (name_ip.find(i)->second == value)
						{
							finded = true;
							break;
						}
					}

					if (!(finded))
						name_ip.insert(std::make_pair(name_ip.size(), value));
				}
			}
			std::chrono::milliseconds ms(100);
			std::this_thread::sleep_for(ms);
		}
		closesocket(intercept_brodcast_socket);
	});
	auto show_options = [&]() {
		it = name_ip.begin();
		SetConsoleTextAttribute(handle, main_window->color1);
		for (short i = 0; it != name_ip.end(); ++it, i++)
		{
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it->second.size()), starting_point.Y + i * 2 });
			std::cout << it->second;
		}
		it = name_ip.begin();
		std::advance(it, cursor_pos);
		SetConsoleTextAttribute(handle, main_window->color2);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it->second.size()), starting_point.Y + cursor_pos * 2 });
		std::cout << it->second;
	};
	
	char button;
	show_options();
	do
	{
		if (_kbhit())
		{
			button = _getch();

			if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 80)
			{

				cursor_pos += 1;
				if (cursor_pos == name_ip.size())
					cursor_pos = 0;
			}
			else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
			{
				cursor_pos -= 1;
				if (cursor_pos < 0)
					cursor_pos = static_cast<int>(name_ip.size()) - 1;
			}
		}
		show_options();
		Sleep(60);
		if (button == 13)
		{
			short i = 0;
			for (std::map<int, std::string>::iterator it2 = name_ip.begin(); it2 != name_ip.end(); ++it2, i++)
			{
				SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it2->second.size()), starting_point.Y + i * 2 });
				for (int j = 0; j < it2->second.size(); j++)
					std::cout << ' ';
			}

			if (it->first == 20)//refresh
			{
				name_ip.clear();
				name_ip.insert(std::make_pair(20, "refresh"));
				name_ip.insert(std::make_pair(21, "back"));
			}
			else if (it->first == 21)
			{
				thread_active = false;
				thread.join();
				return false;
			}
			else
			{
				it = name_ip.begin();
				std::advance(it, cursor_pos);
				host = socket(AF_INET, SOCK_STREAM, 0);
				if (host == INVALID_SOCKET)
				{
					MessageBox(0, ("Socket error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
					thread_active = false;
					thread.join();
					WSACleanup();
					exit(0);
				}
				sockaddr_in SocketAddress;
				memset(&SocketAddress, 0, sizeof(SocketAddress));
				SocketAddress.sin_family = AF_INET;
				SocketAddress.sin_port = htons(6919);
				SocketAddress.sin_addr.s_addr = inet_addr(it->second.substr(it->second.find_last_of('-') + 2, it->second.size() - it->second.find_last_of('-') - 2).c_str());

				if (connect(host, (sockaddr *)&SocketAddress, sizeof(SocketAddress)))
				{
					MessageBox(0, ("Connection error" + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
					closesocket(host);
					continue;
				}
				char temp[255] = "";
				while ((std::string)temp != (std::string)"start")
				{
					recv(host, temp, 255, 0);
				}
				thread_active = false;
				thread.join();
				return true;
			}
		}
	} while (true);
}
void Client::GetTourNames(std::vector<std::string>&tours)
{
	tours.clear();
	std::string get_tour_name_code = "50";
	send(host, get_tour_name_code.c_str(), 4, 0);
	char temp[255];

	if (recv(host, temp, 255, 0) < 0)
		MessageBox(0, "GetTourNames method failed", "Error", 0);

	tours.push_back(temp);//host will always send only one tour (the tour that he choosed)
}
void Client::GetCarNames(std::vector<std::string>&cars, std::string tour)
{
	std::string get_tour_name_code = "51";
	send(host, get_tour_name_code.c_str(), 4, 0);
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
	send(host, get_tour_name_code.c_str(), 4, 0);
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
	send(host, get_tour_name_code.c_str(), 256, 0);
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
	send(host, get_tour_name_code.c_str(), 256, 0);
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
	send(host, get_tour_name_code.c_str(), 4, 0);
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
void Client::GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour)
{
	//client don't need to know stats of other participants but he need to tell host what have he choosed
	std::string get_tour_name_code = "59";
	send(host, get_tour_name_code.c_str(), 4, 0);
	send(host, participants[0]->name.c_str(), 255, 0);
	send(host, participants[0]->car_path.c_str(), 255, 0);
	send(host, tire_path.c_str(), 255, 0);
	return; 
}
std::vector<std::pair<float, std::string>> Client::GetRankingInfo(std::vector<Participant*> &participants)
{
	
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
bool Client::GetCurrentAtribs(std::vector<Participant*> &participants, std::string field)
{
	return false;
}
void Client::Attack(std::vector<Participant*> &participants, int ais, bool alive)
{
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
void Client::SendInfo(std::string special_text, std::string text)
{

}
void Client::TakeAction(Participant* &participant)
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
			if (participant->current_speed == 0 && position == 1)
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
			value = Text::Choose::Numeric(participant->car_modifiers[CarModifiers::max_accelerating + position], { console_screen_buffer_info.dwCursorPosition.X, console_screen_buffer_info.dwCursorPosition.Y }, true, *main_window);
			SetConsoleCursorPosition(window, { console_screen_buffer_info.dwCursorPosition.X - 2,console_screen_buffer_info.dwCursorPosition.Y });
			std::cout << "  ";
			if (value == 0)
				break;

			get_tour_name_code += std::to_string(position) + std::to_string(value);
			send(host, get_tour_name_code.c_str(), 4, 0);

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
					if (participant->current_speed > 0 || position == 4)
					{
						get_tour_name_code += std::to_string(position);
						send(host, get_tour_name_code.c_str(), 4, 0);
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
void Client::GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string>& tour)
{
}
void Client::SendTarget(int ranking_position)
{

}
int Client::Possible_AIs()
{
	return 0;
}