#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>
#include <string>
#include <map>

#pragma comment(lib, "Ws2_32.lib")
namespace GeneralMultiPlayer {

	//Winsock recv function but can handle disconnects
	bool Recv(SOCKET socket, char *buffer, int length, int flags);

	//Container that saves messages received from clients to process them when the time comes
	class RequestHandler {

		unsigned int number_of_clients;
		std::vector<std::vector<std::string>> requests;

	public:
		RequestHandler(int number_of_clients);
		void SaveMsg(std::string msg, int client_id);
		std::vector<std::string>* GetMsgsPtr(int client_id);
	};

	class Host {

	protected:
		std::vector <std::pair<SOCKET, sockaddr_in>> clients;
		std::vector <sockaddr_in> black_list;
		bool broadcast_running;
		bool accepting_running;
		bool handling_connection;
		unsigned short port;

		//Handling recived msgs. 
		//Takes function pointer as an argument that will handle logic behind messages with given key code
		template <class T>
		void RecvFunction(int client_id, void(T::*MsgHandling)(std::string, int), T* object);
		std::vector<std::thread> recv_threads;

	public:
		Host();
		std::string GetThisIp(sockaddr_in sock_addr);
		void Broadcast(unsigned long addr_range, int ms_interval);
		void AcceptClients(int max);
		std::vector<std::pair<SOCKET, sockaddr_in>>* GetClientsPtr() { return &clients; }
		std::vector<sockaddr_in>* GetBlackListPtr() { return &black_list; }
		void StopBroadcasting() { broadcast_running = false; }
		void StopAcceptingClients();
		void CloseActiveConnections();

		//Sends start message to connected clients.	
		//Initiates threads with instances of RecvFunction
		template <class T>
		bool HandleConnection(void(T::*MsgHandling)(std::string, int), T* object);
	};

	class Client {

	protected:
		//key is the time of last message received from this host
		//std::string is describing name of the host and his ip separated with " - "
		std::map<int, std::string> current_hosts;
		SOCKET *host;
		sockaddr_in addr;
		unsigned short port;

		//Flag that stands for whether or not Recv Broadcast should be stopped
		bool receiving_broadcast;

	public:
		Client(SOCKET *host);
		bool RecvBroadcast(int max_hosts, int ms_interval);
		bool Connect(std::string ip);
		void FinishBroadcast() { receiving_broadcast = false; }
		std::map<int, std::string> GetCurrentHosts() { return current_hosts; }
		std::string GetIpFromMapValue(std::string value) { return value.substr(value.find_last_of(" ")+1, value.size()- value.find_last_of(" ")-1); }
	};
}

template <class T>
bool GeneralMultiPlayer::Host::HandleConnection(void(T::*MsgHandling)(std::string, int), T* object)
{
	handling_connection = true;

	for (int i = 0; i < static_cast<int>((clients).size()); i++)
	{
		if (!send(clients[i].first, "start", 6, 0))
		{
			MessageBox(0, ("Socket error: " + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			return false;
		}
	}
	for (int i = 0; i < static_cast<int>((clients).size()); i++)
		recv_threads.push_back(std::thread(&GeneralMultiPlayer::Host::RecvFunction<T>, this, i, MsgHandling, object));
}
template <class T>
void  GeneralMultiPlayer::Host::RecvFunction(int client_id, void(T::*MsgHandling)(std::string, int), T* object)
{
	char buffer[254] = "";
	bool result;
	while (handling_connection)
	{
		if (!GeneralMultiPlayer::Recv(clients[client_id].first, buffer, 254, 0))
		{
			MessageBox(0, ((std::string)"Client " + std::to_string(client_id) + " disconnected").c_str(), "Message", 0);
			closesocket(clients[client_id].first);
			break;
		}
		else
			std::invoke(MsgHandling, object, buffer, client_id);
	}
}