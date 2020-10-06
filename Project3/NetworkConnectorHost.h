#pragma once
#include "NetworkConnectorGeneral.h"

//Singleton class that uses template to pass any class method to handle connection between host and client
template<class T, void (T::* Method)(std::string, int)>
class NetworkConnectorHost
{
	struct ClientContainer
	{
		unsigned int id;
		SOCKET socket;
		sockaddr_in address;
		bool handling_connection = true;
		std::unique_ptr<std::thread> handling_thread;
	};

	T* main_object_ptr;
	int next_client_id = 0;
	std::vector<ClientContainer> clients;
	std::vector<int> disconnected_clients;
	std::vector<sockaddr_in> blacklist;
	bool broadcasting = false;
	bool accepting = false;
	std::unique_ptr<std::thread> broadcasting_thread;
	std::unique_ptr<std::thread> accepting_thread;
	std::mutex network_mutex;

	void Broadcast(const bool hamachi);
	void AcceptingClients(int max);
	void Handling(int unmapped_id);
	int GetClientIndexFromUnmappedID(int unmapped_id, bool lock = true);
	void RemoveClientConnection(int id, bool join_thread = true, bool lock = true);

public:
	NetworkConnectorHost(T* main_object_ptr);
	void StartBroadcast(const bool hamachi);
	void StopBroadcast();
	void StartAcceptingClients(int max);
	void StopAcceptingClients();
	void CloseAllConnections();
	void BanClient(int unmapped_id);
	void BanClient(sockaddr_in address);
	void UnbanClient(sockaddr_in address);
	std::vector<int> GetDisconnectedClientsUnmappedID();
	static std::string GetIP(const sockaddr_in sock_addr);
	const std::vector<sockaddr_in> GetBlacklist();
	const std::vector<std::string> GetClientsNames();
};

template<class T, void(T::* Method)(std::string, int)>
inline NetworkConnectorHost<T, Method>::NetworkConnectorHost(T* main_object_ptr)
{
	if(!NetworkConnector::network_initialized)
	{
		WSAData wsa_data;
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
		NetworkConnector::network_initialized = true;
		this->main_object_ptr = main_object_ptr;
	}
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::StartBroadcast(const bool hamachi)
{
	if (!broadcasting)
	{
		//Broadcast method sets broadcasting to true
		broadcasting_thread = std::make_unique<std::thread>(&NetworkConnectorHost::Broadcast, this, hamachi);
	}
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::Broadcast(const bool hamachi)
{
	//UDP protocol to broadcast messages to all addresses in local network and virtual local network if flag is set
	SOCKET broadcast_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in sock_addr = {};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(NetworkConnector::Constants::port_number);
	broadcasting = true;

	if (broadcast_socket < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	char option = '1';
	if (setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &option, sizeof(int)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}

	char host_name[50];
	if (gethostname(host_name, sizeof(host_name)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	while (broadcasting)
	{
		bool result = false;
		for (int i = 0; i < 255; ++i)
		{
			sock_addr.sin_addr.s_addr = inet_addr(("192.168." + std::to_string(i) + ".255").c_str());
			if (!(sendto(broadcast_socket, host_name, sizeof(host_name), 0, (sockaddr*)&sock_addr, sizeof(sock_addr)) < 0))
			{
				result = true;
			}
		}
		if (hamachi)
		{
			sock_addr.sin_addr.s_addr = inet_addr("25.255.255.255");
			if (!(sendto(broadcast_socket, host_name, sizeof(host_name), 0, (sockaddr*)&sock_addr, sizeof(sock_addr)) < 0))
			{
				result = true;
			}
		}
		if (!result)
		{
			MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
			WSACleanup();
			exit(0);
		}
		std::chrono::milliseconds ms(NetworkConnector::Constants::ms_delay);
		std::this_thread::sleep_for(ms);

	}
	closesocket(broadcast_socket);
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::Handling(int unmapped_id)
{
	network_mutex.lock();
	int client_index = GetClientIndexFromUnmappedID(unmapped_id, false);
	bool* handling = &clients[client_index].handling_connection;
	SOCKET client_socket = clients[client_index].socket;
	network_mutex.unlock();

	while (*handling)
	{
		char buffer[NetworkConnector::Constants::buffer_size];

		//TODO add disconnect checks
		if (recv(client_socket, buffer, NetworkConnector::Constants::buffer_size, 0) != SOCKET_ERROR)
		{
			std::string msg = buffer;
			std::invoke(Method, main_object_ptr, msg, 0);
		}
		else
		{
			network_mutex.lock();
			client_index = GetClientIndexFromUnmappedID(unmapped_id, false);
			disconnected_clients.emplace_back(unmapped_id);
			RemoveClientConnection(client_index, false, false);
			network_mutex.unlock();
		}

	}
}

template<class T, void(T::* Method)(std::string, int)>
inline int NetworkConnectorHost<T, Method>::GetClientIndexFromUnmappedID(int unmapped_id, bool lock)
{
	if (lock)
	{
		network_mutex.lock();
	}
	size_t upper_bound = clients.size();
	size_t lower_bound = 0;
	size_t index = (lower_bound + upper_bound) / 2;
	while (true)
	{
		if (clients[index].id == unmapped_id)
		{
			if (lock)
			{
				network_mutex.unlock();
			}
			return index;
		}
		else if (clients[index].id < unmapped_id)
		{
			lower_bound = index;
		}
		else
		{
			upper_bound = index;
		}
		//Checking if there is any client with matching unmapped_id.
		if ((lower_bound + upper_bound) / 2 != index)
		{
			index = (lower_bound + upper_bound) / 2;
		}
		else
		{
			if (lock)
			{
				network_mutex.unlock();
			}
			return -1;
		}
	}
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::RemoveClientConnection(int id, bool join_thread, bool lock)
{
	clients[id].handling_connection = false;
	if (clients[id].handling_thread->joinable() && join_thread)
	{
		clients[id].handling_thread->join();
	}
	closesocket(clients[id].socket);
	if (lock)
	{
		network_mutex.lock();
	}
	clients.erase(clients.begin() + id);
	if (lock)
	{
		network_mutex.unlock();
	}
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::StopBroadcast()
{
	if (broadcasting)
	{
		broadcasting = false;
		broadcasting_thread->join();
	}
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::AcceptingClients(int max)
{
	const char* timeout = "100";
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sock_addr = {};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(NetworkConnector::Constants::port_number);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	int addr_size = sizeof(sockaddr_in);
	accepting = true;

	if (sock < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	if (bind(sock, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sockaddr)) < 0)
	{
		MessageBox(0, std::to_string(WSAGetLastError()).c_str(), NetworkConnector::ErrorTitle::winsock.c_str(), 0);
		WSACleanup();
		exit(0);
	}
	listen(sock, max);

	while (accepting)
	{
		//Creating temporary socket to check if accepted connection is valid
		SOCKET temp = accept(sock, reinterpret_cast<sockaddr*>(&sock_addr), &addr_size);
		if (temp != INVALID_SOCKET && accepting)
		{
			bool playable = true;
			network_mutex.lock();
			if (static_cast<int>(clients.size()) >= max)
			{
				playable = false;
			}
			for (int i = 0; i < static_cast<int>(clients.size()); ++i)
			{
				if (sock_addr.sin_addr.s_addr == clients[i].address.sin_addr.s_addr)
				{
					playable = false;
					break;
				}
			}
			for (int i = 0; i < static_cast<int>(blacklist.size()); ++i)
			{
				if (blacklist[i].sin_addr.s_addr == sock_addr.sin_addr.s_addr)
				{
					playable = false;
					break;
				}
			}
			if (playable)
			{
				ClientContainer new_client;
				new_client.id = next_client_id++;
				new_client.address = sock_addr;
				new_client.socket = temp;
				new_client.handling_thread = std::make_unique<std::thread>(&NetworkConnectorHost::Handling, this, new_client.id);

				clients.push_back(std::move(new_client));
				accept(temp, reinterpret_cast<sockaddr*>(&sock_addr), &addr_size);
			}
			else
			{
				closesocket(temp);
			}
			network_mutex.unlock();
		}
	}
	closesocket(sock);
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::StartAcceptingClients(int max)
{
	if (!accepting)
	{
		accepting = true;
		accepting_thread = std::make_unique<std::thread>(&NetworkConnectorHost::AcceptingClients, this, max);
	}
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::StopAcceptingClients()
{
	if (accepting)
	{
		accepting = false;

		//connecting to myself to un-block accept listener
		SOCKET temp_socket = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in temp_addr;
		memset(&temp_addr, 0, sizeof(temp_addr));
		temp_addr.sin_family = AF_INET;
		temp_addr.sin_port = htons(NetworkConnector::Constants::port_number);
		temp_addr.sin_addr.s_addr = inet_addr(NetworkConnector::Constants::ip_loopback.c_str());
		connect(temp_socket, reinterpret_cast<sockaddr*>(&temp_addr), sizeof(temp_addr));
		closesocket(temp_socket);

		accepting_thread->join();
	}
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::CloseAllConnections()
{
	StopBroadcast();
	StopAcceptingClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		RemoveClientConnection(i);
	}
	clients.clear();
	disconnected_clients.clear();
	blacklist.clear();
	WSACleanup();

}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::BanClient(int unmapped_id)
{
	network_mutex.lock();
	int index = GetClientIndexFromUnmappedID(unmapped_id, false);
	blacklist.push_back(clients[index].address);
	RemoveClientConnection(index, true, false);
	network_mutex.unlock();
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::BanClient(sockaddr_in address)
{
	network_mutex.lock();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].address.sin_addr.s_addr == address.sin_addr.s_addr)
		{
			blacklist.push_back(address);
			RemoveClientConnection(i, true, false);
			break;
		}
	}
	network_mutex.unlock();
}

template<class T, void(T::* Method)(std::string, int)>
inline void NetworkConnectorHost<T, Method>::UnbanClient(sockaddr_in address)
{
	network_mutex.lock();
	for (size_t i = 0; i < blacklist.size(); ++i)
	{
		if (blacklist[i].sin_addr.s_addr == address.sin_addr.s_addr)
		{
			blacklist.erase(blacklist.begin() + i);
			break;
		}
	}
	network_mutex.unlock();
}

template<class T, void(T::* Method)(std::string, int)>
inline std::vector<int> NetworkConnectorHost<T, Method>::GetDisconnectedClientsUnmappedID()
{
	network_mutex.lock();
	std::vector<int> ret = disconnected_clients;
	disconnected_clients.clear();
	network_mutex.unlock();
	return ret;
}

template<class T, void(T::* Method)(std::string, int)>
inline std::string NetworkConnectorHost<T, Method>::GetIP(const sockaddr_in sock_addr)
{
	char helper[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &sock_addr.sin_addr, helper, INET_ADDRSTRLEN);

	return static_cast<std::string>(helper);
}
template<class T, void(T::* Method)(std::string, int)>
inline const std::vector<sockaddr_in> NetworkConnectorHost<T, Method>::GetBlacklist()
{
	return blacklist;
}

template<class T, void(T::* Method)(std::string, int)>
inline const std::vector<std::string> NetworkConnectorHost<T, Method>::GetClientsNames()
{
	network_mutex.lock();
	std::vector<std::string> ret = {};
	for (size_t i = 0; i < clients.size(); ++i)
	{
		ret.push_back(GetIP(clients[i].address));
	}
	network_mutex.unlock();
	return ret;
}
