#include "NetworkRole.h"

Host::Host(Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	StartNetwork();
}
void Host::StartNetwork()
{
	bool broadcasting = true;

	std::thread broadcast([&]() {

		std::chrono::seconds s(1);
		char broadcast = 'a';
		SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (sock == INVALID_SOCKET)
		{
			MessageBox(0, "Socket hostname error", "Error", 0);
			WSACleanup();
			exit(0);
		}
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
		{
			MessageBox(0, "Socket broadcast error", "Error", 0);
			closesocket(sock);
			WSACleanup();
			exit(0);
		}
		sockaddr_in SocketAddress;
		memset(&SocketAddress, 0, sizeof(SocketAddress));
		SocketAddress.sin_family = AF_INET;
		SocketAddress.sin_port = htons(6919);
		SocketAddress.sin_addr.s_addr = inet_addr("192.168.x.x");
		if (main_window->GetHamachiConnectionFlag())
			SocketAddress.sin_addr.s_addr |= inet_addr("25.x.x.x");
		
		//---------------------------------------------------------------------------Getting Host name -----------------------------------------------------------------------//
		char HostName[50];

		if (gethostname(HostName, sizeof(HostName)) == SOCKET_ERROR)
		{
			MessageBox(0, "Socket hostname error", "Error", 0);
			WSACleanup();
			closesocket(sock);
			exit(0);
		}
		//-------------------------------------------------------------------------------Broadcasting----------------------------------------------------------------------//
		while (broadcasting)
		{
			if (sendto(sock, HostName, sizeof(HostName), 0, (sockaddr *)&SocketAddress, sizeof(SocketAddress)) < 0)
			{
				MessageBox(0, "Socket send error", "Error", 0);
				WSACleanup();
				closesocket(sock);
				exit(0);
			}
			std::this_thread::sleep_for(s);
		}
	});
	std::cin.get();
	broadcasting = false;
	broadcast.join();


}
void Host::GetOtherParticipants(std::vector<Participant*> &participants, int ais)
{

}
void Host::GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores)
{

}
void Host::GetInfobox(InfoBox *infobox)
{

}
void Host::GetCurrentAtribs(float &durability, float current_speed)
{

}
void Host::GetTargetList(std::vector<std::string>&names)
{

}
void Host::SendInfo(std::string special_text, std::string text)
{

}
std::pair<int, int> Host::TakeAction()
{
	return std::make_pair(1, 4);
}
void Host::SendTarget(int ranking_position)
{

}
int Host::Possible_AIs()
{
	return 7 - (int)clients.size();
}