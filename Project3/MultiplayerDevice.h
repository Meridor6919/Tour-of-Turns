#pragma once
#include <iostream>
#include "Participant.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

class Host;

class MultiplayerDevice
{
	std::vector<std::pair<SOCKET, sockaddr_in>> *clients_sockets;
	std::vector<Participant*> *clients;
	std::vector<int> client_current_game_stage;
	int current_stage;
	std::string tour;
	Host *host;

	bool ValidateClientAction(std::string message, int client_id);

public:

	MultiplayerDevice(std::vector<Participant*> *clients, std::vector<std::pair<SOCKET, sockaddr_in>> *clients_sockets, Host *host, int current_stage);
	void NextStage() { current_stage++; }
	void HandleClientConnection();
	bool ClientsReadyForNewStage();
};