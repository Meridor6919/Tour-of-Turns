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

		//functions for private data access
		std::vector <std::pair<SOCKET, sockaddr_in>>* GetClientsPtr() { return &clients; }
		std::vector <sockaddr_in>* GetBlackListPtr() { return &black_list; }
		void StopBroadcasting() { broadcast_running = false; }
		void StopAcceptingClients();
	};

	class Client {

		/*EXCEPTION CODES
		1. socket error
		2. socket opt error
		3. socket connection error
		4. socket bind error
		10. target stopped hosting*/

	protected:
		//map that contains key - indicates order, and string that discribes name of the host and his ip
		std::map<int, std::string> name_ip;

		//socket that will connect to host
		SOCKET host;

		//host's address
		sockaddr_in addr;

		//port number
		unsigned short port;

		//variable that decides if "RecvBroadcast" function should be stopped
		bool receiving_broadcast;

	public:

		Client();

		//recv broadcast msgs from potential hosts and saves it in "name_ip" container
		void RecvBroadcast(int max_hosts, int ms_interval);
		void FinishBroadcast() { receiving_broadcast = false; }
		//sets connection with host
		void Connect(std::string ip, int recv_time);
	};
}