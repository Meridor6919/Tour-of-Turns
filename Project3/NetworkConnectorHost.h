#pragma once
#include "NetworkConnectorGeneral.h"
#include "BroadcastSender.h"
#define TEMPLATE_NetworkConnectorHost template<class T, void (T::* Method)(std::string, int)>
//Singleton class that uses template to pass any class method to handle connection between host and client
TEMPLATE_NetworkConnectorHost class NetworkConnectorHost
{
	struct ClientContainer;
	class ClientConnector;
	class ErrorHandler;
	
	T* main_object_ptr;
	std::vector<ClientContainer> clients;
	std::vector<sockaddr_in> disconnected_clients;
	std::vector<sockaddr_in> disconnected_clients_error_handling;
	std::mutex network_mutex;
	ErrorHandler error_handler;

	int unsigned GetClientContainerIndexFromAddress(sockaddr_in address);
	void ProcessingMessages(sockaddr_in address);

public:
	NetworkConnector::BroadcastSender broadcast_sender;
	ClientConnector client_connector;

	NetworkConnectorHost(T* main_object_ptr);
	void RemoveClient(sockaddr_in address);
	void CloseAllConnections();
	const std::vector<std::string> GetClientNames();
	const std::vector<sockaddr_in> GetClientAddresses();

	static std::string GetIP(const sockaddr_in sock_addr);
};
//-------------------------------------------------NESTED CLASS DECLARATIONS--------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
TEMPLATE_NetworkConnectorHost struct NetworkConnectorHost<T, Method>::ClientContainer
{
	SOCKET socket;
	sockaddr_in address;
	bool handling_connection = true;
	std::thread handling_thread;
};
TEMPLATE_NetworkConnectorHost class NetworkConnectorHost<T, Method>::ClientConnector
{
	NetworkConnectorHost<T, Method>* network_connector_host_ptr;
	std::vector<sockaddr_in> blacklist;
	std::thread main_thread;
	bool thread_active;
	int max_clients;
	void Accepting();

public:
	void Initialize(NetworkConnectorHost<T, Method>* network_connector_host_ptr);
	void Start(int max_clients);
	void Stop();
	void BanClient(sockaddr_in address);
	void UnbanClient(sockaddr_in address);
	const std::vector<sockaddr_in>& GetBlacklist();
};
TEMPLATE_NetworkConnectorHost class NetworkConnectorHost<T, Method>::ErrorHandler
{
	std::thread main_thread;
	bool thread_active;
	NetworkConnectorHost<T, Method>* network_connector_host_ptr;

	void Handling();

public:
	void Initialize(NetworkConnectorHost<T, Method>* network_connector_host_ptr);
	void Start();
	void Stop();
};

//-----------------------------------------------------MAIN CLASS METHODS-----------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
TEMPLATE_NetworkConnectorHost inline NetworkConnectorHost<T, Method>::NetworkConnectorHost(T* main_object_ptr)
{
	if (!NetworkConnector::network_initialized)
	{
		WSAData wsa_data;
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
		NetworkConnector::network_initialized = true;
		this->main_object_ptr = main_object_ptr;
		client_connector.Initialize(this);
		error_handler.Initialize(this);
	}
}
TEMPLATE_NetworkConnectorHost inline unsigned int NetworkConnectorHost<T, Method>::GetClientContainerIndexFromAddress(sockaddr_in address)
{
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].address.sin_addr.s_addr == address.sin_addr.s_addr)
		{
			return static_cast<unsigned int>(i);
		}
	}
	return -1;
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ProcessingMessages(sockaddr_in address)
{
	network_mutex.lock();
	int client_index = GetClientContainerIndexFromAddress(address);
	if (client_index < 0)
	{
		abort();
	}
	bool* handling = &clients[client_index].handling_connection;
	SOCKET client_socket = clients[client_index].socket;
	network_mutex.unlock();

	while (*handling)
	{
		char buffer[NetworkConnector::Constants::buffer_size];

		if (recv(client_socket, buffer, NetworkConnector::Constants::buffer_size, 0) > 0)
		{
			std::string msg = buffer;
			std::invoke(Method, main_object_ptr, msg, 0);
		}
		else
		{
			network_mutex.lock();
			disconnected_clients.emplace_back(address);
			disconnected_clients_error_handling.emplace_back(address);
			network_mutex.unlock();
		}
	}
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::RemoveClient(sockaddr_in address)
{
	network_mutex.lock();
	unsigned int target = GetClientContainerIndexFromAddress(address);
	if (target < 0)
	{
		MessageBox(0, NetworkConnector::ErrorMsg::address_missing.c_str(), NetworkConnector::ErrorTitle::address_missing.c_str(), 0);
	}
	else
	{
		clients[target].handling_connection = false;
		if (clients[target].handling_thread.joinable())
		{
			clients[target].handling_thread.join();
		}
		closesocket(clients[target].socket);
		clients.erase(clients.begin() + target);
	}
	network_mutex.unlock();
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::CloseAllConnections()
{
	broadcast_sender.Stop();
	client_connector.Stop();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		RemoveClient(clients[i].address);
	}
	clients.clear();
	disconnected_clients.clear();
	disconnected_clients_error_handling.clear();
	WSACleanup();
}
TEMPLATE_NetworkConnectorHost inline const std::vector<std::string> NetworkConnectorHost<T, Method>::GetClientNames()
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
TEMPLATE_NetworkConnectorHost inline const std::vector<sockaddr_in> NetworkConnectorHost<T, Method>::GetClientAddresses()
{
	network_mutex.lock();
	std::vector<sockaddr_in> ret = {};
	for (size_t i = 0; i < clients.size(); ++i)
	{
		ret.push_back(clients[i].address);
	}
	network_mutex.unlock();
	return ret;
}
TEMPLATE_NetworkConnectorHost inline std::string NetworkConnectorHost<T, Method>::GetIP(const sockaddr_in sock_addr)
{
	char helper[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &sock_addr.sin_addr, helper, INET_ADDRSTRLEN);

	return static_cast<std::string>(helper);
}

//-------------------------------------------------------CLIENT CONNECTOR-----------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::Accepting()
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sock_addr = {};
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(NetworkConnector::Constants::port_number);
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	int addr_size = sizeof(sockaddr_in);

	NetworkConnector::Validate(sock < 0);
	NetworkConnector::Validate(bind(sock, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sockaddr)) < 0);
	listen(sock, max_clients);

	while (thread_active)
	{
		//Creating temporary socket to check if accepted connection is valid
		SOCKET temp = accept(sock, reinterpret_cast<sockaddr*>(&sock_addr), &addr_size);
		if (temp != INVALID_SOCKET && thread_active)
		{
			bool playable = true;
			network_connector_host_ptr->network_mutex.lock();
			if (static_cast<int>(network_connector_host_ptr->clients.size()) >= max_clients)
			{
				playable = false;
			}
			for (size_t i = 0; i < network_connector_host_ptr->clients.size(); ++i)
			{
				if (sock_addr.sin_addr.s_addr == network_connector_host_ptr->clients[i].address.sin_addr.s_addr)
				{
					playable = false;
					break;
				}
			}
			for (size_t i = 0; i < blacklist.size(); ++i)
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
				new_client.address = sock_addr;
				new_client.socket = temp;
				new_client.handling_thread = std::thread(&NetworkConnectorHost::ProcessingMessages, network_connector_host_ptr, sock_addr);

				network_connector_host_ptr->clients.push_back(std::move(new_client));
				accept(temp, reinterpret_cast<sockaddr*>(&sock_addr), &addr_size);
			}
			else
			{
				closesocket(temp);
			}
			network_connector_host_ptr->network_mutex.unlock();
		}
	}
	closesocket(sock);
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::Initialize(NetworkConnectorHost<T, Method>* network_connector_host_ptr)
{
	this->network_connector_host_ptr = network_connector_host_ptr;
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::Start(int max_clients)
{
	thread_active = true;
	this->max_clients = max_clients;
	main_thread = std::thread(&NetworkConnectorHost::ClientConnector::Accepting, this);	
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::Stop()
{
	if(thread_active)
	{
		thread_active = false;

		//connecting to myself to un-block accept listener
		SOCKET temp_socket = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in temp_addr = {};
		temp_addr.sin_family = AF_INET;
		temp_addr.sin_port = htons(NetworkConnector::Constants::port_number);
		temp_addr.sin_addr.s_addr = inet_addr(NetworkConnector::Constants::ip_loopback.c_str());
		connect(temp_socket, reinterpret_cast<sockaddr*>(&temp_addr), sizeof(temp_addr));
		closesocket(temp_socket);

		if (main_thread.joinable())
		{
			main_thread.join();
		}
	}
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::BanClient(sockaddr_in address)
{
	network_connector_host_ptr->network_mutex.lock();
	blacklist.push_back(address);
	network_connector_host_ptr->network_mutex.unlock();
	network_connector_host_ptr->RemoveClient(address);
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::UnbanClient(sockaddr_in address)
{
	network_connector_host_ptr->network_mutex.lock();
	for (size_t i = 0; i < network_connector_host_ptr->clients.size(); ++i)
	{
		if (blacklist[i].sin_addr.s_addr == address.sin_addr.s_addr)
		{
			blacklist.erase(blacklist.begin() + i);
			network_connector_host_ptr->network_mutex.unlock();
			return;
		}
	}
	MessageBox(0, NetworkConnector::ErrorMsg::address_missing.c_str(), NetworkConnector::ErrorTitle::address_missing.c_str(), 0);
	network_connector_host_ptr->network_mutex.unlock();

}
TEMPLATE_NetworkConnectorHost inline const std::vector<sockaddr_in>& NetworkConnectorHost<T, Method>::ClientConnector::GetBlacklist()
{
	return blacklist;
}

//-------------------------------------------------------ERROR HANDLING-------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ErrorHandler::Handling()
{
	while (thread_active)
	{
		network_connector_host_ptr->network_mutex.lock();
		for (size_t i = 0; i < network_connector_host_ptr->disconnected_clients_error_handling.size(); ++i)
		{
			const unsigned int index = network_connector_host_ptr->GetClientContainerIndexFromAddress(network_connector_host_ptr->disconnected_clients_error_handling[i]);
			shutdown(network_connector_host_ptr->clients[index].socket, SD_BOTH);
			network_connector_host_ptr->clients[index].handling_connection = false;
			network_connector_host_ptr->clients[index].handling_thread.join();
			network_connector_host_ptr->clients.erase(network_connector_host_ptr->clients.begin() + index);
		}
		network_connector_host_ptr->disconnected_clients_error_handling.clear();
		network_connector_host_ptr->network_mutex.unlock();
		std::chrono::milliseconds ms(NetworkConnector::Constants::ms_delay);
		std::this_thread::sleep_for(ms);
	}
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ErrorHandler::Initialize(NetworkConnectorHost<T, Method>* network_connector_host_ptr)
{
	this->network_connector_host_ptr = network_connector_host_ptr;
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ErrorHandler::Start()
{
	thread_active = true;
	main_thread = std::thread(&ErrorHandler::Handling, this);
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ErrorHandler::Stop()
{
	thread_active = false;
	if (main_thread.joinable())
	{
		main_thread.join();
	}
}

