#include "NetworkRole.h"
#include <map>
#include <string>

Client::Client(Window &main_window) : SinglePlayer(main_window)
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
	std::map<int, std::string> name_ip;
	std::chrono::milliseconds ms(100);
	bool thread_active = true;
	HANDLE handle = main_window->GetHandle();
	int color1 = main_window->color1;
	int color2 = main_window->color2;
	char button;
	short pos = 0;
	COORD starting_point = { (short)main_window->GetWidth() / 2, 15 };
	std::map<int, std::string>::iterator  it;
	SOCKET intercept_brodcast_socket;

	name_ip.insert(std::make_pair(20, "refresh"));
	name_ip.insert(std::make_pair(21, "back"));

	//recv hosts from local network
	std::thread thread([&]() {

		int recv_time = 100;
		intercept_brodcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (intercept_brodcast_socket == INVALID_SOCKET)
		{
			MessageBox(0, "Socket error", "Error", 0);
			WSACleanup();
			closesocket(intercept_brodcast_socket);
			exit(0);
		}
		if (setsockopt(intercept_brodcast_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&recv_time), sizeof(recv_time)) < 0)
		{
			MessageBox(0, "Socket option error", "Error", 0);
			WSACleanup();
			closesocket(intercept_brodcast_socket);
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
			MessageBox(0, "binding error", "Error", 0);
			WSACleanup();
			closesocket(intercept_brodcast_socket);
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
			std::this_thread::sleep_for(ms);
		}
	});
	auto show_options = [&]() {
		it = name_ip.begin();
		SetConsoleTextAttribute(handle, color1);
		for (short i = 0; it != name_ip.end(); ++it, i++)
		{
			SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it->second.size()), starting_point.Y + i * 2 });
			std::cout << it->second;
		}
		it = name_ip.begin();
		std::advance(it, pos);
		SetConsoleTextAttribute(handle, color2);
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::center / 2 * (float)it->second.size()), starting_point.Y + pos * 2 });
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

				pos += 1;
				if (pos == name_ip.size())
					pos = 0;
			}
			else if ((GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0) && button == 72)
			{
				pos -= 1;
				if (pos < 0)
					pos = name_ip.size() - 1;
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
				std::advance(it, pos);
				host = socket(AF_INET, SOCK_STREAM, 0);
				if (host == INVALID_SOCKET)
				{
					std::cout << "error: " << WSAGetLastError();
					MessageBox(0, "Socket error", "Error", 0);
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
					MessageBox(0, "Chosen user stopped hosting", "Error", 0);
					closesocket(host);
					continue;
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

}
void Client::GetCarNames(std::vector<std::string>&cars, std::string tour)
{

}
void Client::GetTireNames(std::vector<std::string>&tires)
{

}
std::vector<int> Client::GetCarParameters(std::string path)
{
	std::vector<int> temp;
	return temp;
}
std::vector<std::string> Client::GetTireParameters(std::string path)
{
	std::vector<std::string> temp;
	return temp;
}
void Client::GetTourParameters(std::string path)
{

}
void Client::GetOtherParticipants(std::vector<Participant*> &participants, int ais)
{

}
void Client::GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores)
{

}
void Client::GetInfobox(InfoBox *infobox)
{

}
void Client::GetCurrentAtribs(float &durability, float current_speed)
{

}
void Client::GetTargetList(std::vector<std::string>&names)
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