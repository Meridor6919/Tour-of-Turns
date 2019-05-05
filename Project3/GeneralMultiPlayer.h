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

	//recv with disconnect handling
	bool Recv(SOCKET socket, char *buffer, int length, int flags);

	class Host {

	protected:

		//vector of active clients
		std::vector <std::pair<SOCKET, sockaddr_in>> clients;

		//vector of banned addresses
		std::vector <sockaddr_in> black_list;

		//variables responsible for functions with coresponding  names
		bool broadcast_running;
		bool accepting_running;

		//port number
		unsigned short port;

	public:

		Host();

		//gets name of current station
		std::string GetIP(sockaddr_in sock_addr);

		//broadcasts host name within given range
		void Broadcast(unsigned long addr_range, int ms_interval);

		//accepts clients that want to connect
		void AcceptClients(int max);
		template <class T>
		bool HandleConnection(void(T::*MsgHandling)(std::string, int), T* object)
		{
			std::thread *recv_threads;
			recv_threads = new std::thread[static_cast<int>((clients).size())];

			for (int i = 0; i < static_cast<int>((clients).size()); i++)
			{
				if (!send(clients[i].first, "start", 6, 0))
				{
					MessageBox(0, ("Socket error: " + std::to_string(WSAGetLastError())).c_str(), "Error", 0);
					return false;
				}
			}
			auto recv_function = [](int i, void(T::*MsgHandling)(std::string, int), T* object, GeneralMultiPlayer::Host *host)
			{
				char buffer[12] = "";
				bool result;
				while (true)
				{
					if (!GeneralMultiPlayer::Recv(host->clients[i].first, buffer, 24, 0))
					{
						MessageBox(0, ((std::string)"Client " + std::to_string(i) + " disconnected").c_str(), "Message", 0);
						break;
					}
					else
						std::invoke(MsgHandling, object, buffer, i);
				}
			};
			for (int i = 0; i < static_cast<int>((clients).size()); i++)
				recv_threads[i] = std::thread(recv_function, i, MsgHandling, object, this);
		}

		//functions for private data access
		std::vector <std::pair<SOCKET, sockaddr_in>>* GetClientsPtr() { return &clients; }
		std::vector <sockaddr_in>* GetBlackListPtr() { return &black_list; }
		void StopBroadcasting() { broadcast_running = false; }
		void StopAcceptingClients();
	};

	class Client {

	protected:
		//key - time of last msg recived from this host, value - name of the host and his ip separated with " - "
		std::map<int, std::string> current_hosts;

		//socket that will connect to host
		SOCKET *host;

		//host's address
		sockaddr_in addr;

		//port number
		unsigned short port;

		//variable that decides if "RecvBroadcast" function should be stopped
		bool receiving_broadcast;

	public:

		Client(SOCKET *host);

		//recv broadcast msgs from potential hosts and saves it in "current_hosts" container
		bool RecvBroadcast(int max_hosts, int ms_interval);
		//sets connection with host
		bool Connect(std::string ip);

		void FinishBroadcast() { receiving_broadcast = false; }
		std::map<int, std::string> GetCurrentHosts() { return current_hosts; }
		std::string GetIpFromMapValue(std::string value) { return value.substr(value.find_last_of(" ")+1, value.size()- value.find_last_of(" ")-1); }
	};
	
}