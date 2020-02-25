#pragma once
#include  "MeridorMultiplayer.h"

namespace MeridorMultiplayer 
{
	class Host 
	{
	protected:
		std::vector<std::pair<SOCKET, sockaddr_in>> clients;
		std::vector<sockaddr_in> black_list;
		bool broadcast_running = false;
		bool accepting_running = false;
		bool handling_connection = false;

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

		template <class T>
		bool HandleConnection(void(T::*MsgHandling)(std::string, int), T* object);
	};
	template <class T>
	bool Host::HandleConnection(void(T::*MsgHandling)(std::string, int), T* object)
	{
		handling_connection = true;

		for (int i = 0; i < static_cast<int>((clients).size()); ++i)
		{
			if (!send(clients[i].first, "start", Constants::buffer_size, 0))
			{
				MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
				return false;
			}
		}
		for (int i = 0; i < static_cast<int>((clients).size()); ++i)
		{
			recv_threads.push_back(std::thread(&Host::RecvFunction<T>, this, i, MsgHandling, object));
		}
		return true;
	}
	template <class T>
	void  Host::RecvFunction(const int client_id, void(T::*MsgHandling)(std::string, int), T* object)
	{
		char buffer[Constants::buffer_size] = "";
		while (handling_connection)
		{
			if (!GeneralMultiPlayer::Recv(clients[client_id].first, buffer, Constants::buffer_size, 0))
			{
				MessageBox(0, std::to_string(WSAGetLastError()).c_str(), ErrorTitle::winsock.c_str(), 0);
				closesocket(clients[client_id].first);
				break;
			}
			else
			{
				std::invoke(MsgHandling, object, buffer, client_id);
			}
		}
	}
}
