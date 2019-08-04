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
	bool Recv(SOCKET socket, char *buffer, const int length, const int flags);

	//Container that saves messages received from clients to process them when the time comes
	class RequestHandler {
		int number_of_clients;
		std::vector<std::vector<std::string>> requests;

	public:
		RequestHandler(const int number_of_clients);
		void SaveMsg(const std::string msg, const int client_id);
		std::vector<std::string>* GetMsgsPtr(const int client_id);
	};

	class Host {

	protected:
		std::vector <std::pair<SOCKET, sockaddr_in>> clients;
		std::vector <sockaddr_in> black_list;
		bool broadcast_running;
		bool accepting_running;
		bool handling_connection;
		const unsigned short port = 6919;;

		//Handling recived msgs. 
		//Takes function pointer as an argument that will handle logic behind messages with given key code
		template <class T>
		void RecvFunction(const int client_id, void(T::*MsgHandling)(std::string, int), T* object);
		std::vector<std::thread> recv_threads;

	public:
		std::string GetThisIp(const sockaddr_in sock_addr);
		void Broadcast(const unsigned long addr_range, int ms_interval);
		void AcceptClients(const int max);
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
		const unsigned short port = 6919;;

		//Flag that stands for whether or not Recv Broadcast should be stopped
		bool receiving_broadcast;

	public:
		Client(SOCKET *host);
		bool RecvBroadcast(const int max_hosts, int ms_interval);
		bool Connect(const std::string ip);
		std::string GetIpFromMapValue(const std::string value);
		void FinishBroadcast() { receiving_broadcast = false; }
		std::map<int, std::string> GetCurrentHosts() { return current_hosts; }
	};
}

template <class T>
bool GeneralMultiPlayer::Host::HandleConnection(void(T::*MsgHandling)(std::string, int), T* object)
{
	handling_connection = true;

	for (int i = 0; i < static_cast<int>((clients).size()); ++i)
	{
		if (!send(clients[i].first, "start", 6, 0))
		{
			MessageBox(0, ("Socket error: " + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
			return false;
		}
	}
	for (int i = 0; i < static_cast<int>((clients).size()); ++i)
	{
		recv_threads.push_back(std::thread(&GeneralMultiPlayer::Host::RecvFunction<T>, this, i, MsgHandling, object));
	}
	return true;
}
template <class T>
void  GeneralMultiPlayer::Host::RecvFunction(const int client_id, void(T::*MsgHandling)(std::string, int), T* object)
{
	char buffer[254] = "";
	while (handling_connection)
	{
		if (!GeneralMultiPlayer::Recv(clients[client_id].first, buffer, 254, 0))
		{
			MessageBox(0, ((std::string)"Client " + std::to_string(client_id) + " disconnected").c_str(), "Message", 0);
			closesocket(clients[client_id].first);
			break;
		}
		else
		{
			std::invoke(MsgHandling, object, buffer, client_id);
		}
	}
}