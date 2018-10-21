#pragma once
#include <WinSock2.h>
#include <vector>

#pragma comment(lib, "ws_2.lib")

class MultiplayerDevice
{
	std::vector<SOCKET> ClientSockets;
	//std::vector<Player*> players;

	std::string tour;

	virtual void SendTourNames(int client);
	virtual void SendCarNames(int client, std::string tour_path);
	virtual void SendTireNames(int client);
	virtual void SendCarParameters(int client, std::string car_path);
	virtual void SendTireParameters(int client, std::string tire_path);
	virtual void SendTourParameters(int client, std::string tour_path);

	virtual void SendRankingInfo(int client);
	virtual void SendInfobox(int client);
	virtual void SendCurrentAtribs(int client);
	virtual void SendTargetList(int client);

public:

	MultiplayerDevice(std::vector<SOCKET> &ClientSockets);
};