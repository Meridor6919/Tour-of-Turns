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
	COORD starting_point = { (short)main_window->GetWidth() / 2, 15 };
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
					cursor_pos = name_ip.size() - 1;
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

	do
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetCarNames method failed", "Error", 0);

		cars.push_back(temp);

	} while ((std::string)temp != "exit");
	
}
void Client::GetTireNames(std::vector<std::string>&tires)
{
	std::string get_tour_name_code = "52";
	send(host, get_tour_name_code.c_str(), 4, 0);
	char temp[255];

	do
	{
		if (!recv(host, temp, 255, 0) < 0)
			MessageBox(0, "GetTireNames method failed", "Error", 0);

		tires.push_back(temp);

	} while ((std::string)temp != "exit");
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
			MessageBox(0, "GetTireNames method failed", "Error", 0);

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
			MessageBox(0, "GetTireNames method failed", "Error", 0);

		ret.push_back(temp);
	}
	return ret;
}
std::vector<std::string> Client::GetTourParameters(std::string path)
{
	std::string get_tour_name_code = "53";

	std::vector<std::string>x = {""};
	return x;
}
void Client::GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour)
{

}
std::vector<std::pair<float, std::string>> Client::GetRankingInfo(std::vector<Participant*> &participants)
{
	std::vector<std::pair<float, std::string>> temp = {};
	return temp;
}
void Client::GetInfobox(InfoBox *infobox)
{

}
void Client::GetCurrentAtribs(float &durability, float current_speed)
{

}
void Client::Attack(std::vector<Participant*> &participants, int ais)
{
	
}
void Client::SendInfo(std::string special_text, std::string text)
{

}
std::pair<int, int> Client::TakeAction()
{
	return std::make_pair(1, 4);
}
void Client::SendTarget(int ranking_position)
{

}
int Client::Possible_AIs()
{
	return 0;
}