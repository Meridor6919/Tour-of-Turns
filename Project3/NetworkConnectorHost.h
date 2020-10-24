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
	std::unique_ptr<std::thread> handling_thread;
};
TEMPLATE_NetworkConnectorHost class NetworkConnectorHost<T, Method>::ClientConnector
{
	NetworkConnectorHost<T, Method>* enclosing_class_ptr;
	std::vector<sockaddr_in> blacklist;
	std::unique_ptr<std::thread> main_thread;
	bool thread_active;
	int max_clients;
	void Accepting();

public:
	void Initialize(NetworkConnectorHost<T, Method>* enclosing_class_ptr);
	void Start(int max_clients);
	void Stop();
	void BanClient(sockaddr_in address);
	void UnbanClient(sockaddr_in address);
	const std::vector<sockaddr_in>& GetBlacklist();
};
TEMPLATE_NetworkConnectorHost class NetworkConnectorHost<T, Method>::ErrorHandler
{
	std::unique_ptr<std::thread> main_thread;
	bool thread_active;

	void Handling();

public:
	void Start();
	void Stop();
};

//-----------------------------------------------------MAIN CLASS METHODS-----------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
TEMPLATE_NetworkConnectorHost inline NetworkConnectorHost<T, Method>::NetworkConnectorHost(T* main_object_ptr)
{
	if (!NetworkConnector::network_initialized)//CHECK IN DEBUG or compile time
	{
		WSAData wsa_data;
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
		NetworkConnector::network_initialized = true;
		this->main_object_ptr = main_object_ptr;
		client_connector.Initialize(this);
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
			network_mutex.unlock();
		}
	}
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::RemoveClient(sockaddr_in address)
{
	network_mutex.lock();
	unsigned int target = GetClientContainerIndexFromAddress(address);
	clients[target].handling_connection = false;
	if (clients[target].handling_thread->joinable())
	{
		clients[target].handling_thread->join();
	}
	closesocket(clients[target].socket);
	clients.erase(clients.begin() + target);
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
	const char* timeout = "100";
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
			enclosing_class_ptr->network_mutex.lock();
			if (static_cast<int>(enclosing_class_ptr->clients.size()) >= max_clients)
			{
				playable = false;
			}
			for (int i = 0; i < static_cast<int>(enclosing_class_ptr->clients.size()); ++i)
			{
				if (sock_addr.sin_addr.s_addr == enclosing_class_ptr->clients[i].address.sin_addr.s_addr)
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
				new_client.address = sock_addr;
				new_client.socket = temp;
				new_client.handling_thread = std::make_unique<std::thread>(&NetworkConnectorHost::ProcessingMessages, enclosing_class_ptr, sock_addr);

				enclosing_class_ptr->clients.push_back(std::move(new_client));
				accept(temp, reinterpret_cast<sockaddr*>(&sock_addr), &addr_size);
			}
			else
			{
				closesocket(temp);
			}
			enclosing_class_ptr->network_mutex.unlock();
		}
	}
	closesocket(sock);
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::Initialize(NetworkConnectorHost<T, Method>* enclosing_class_ptr)
{
	this->enclosing_class_ptr = enclosing_class_ptr;
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::Start(int max_clients)
{
	if (!thread_active)//CHECK IN DEBUG or compile time
	{
		thread_active = true;
		this->max_clients = max_clients;
		main_thread = std::make_unique<std::thread>(&NetworkConnectorHost::ClientConnector::Accepting, this);
	}
	
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::Stop()
{
	if (thread_active)//CHECK IN DEBUG or compile time
	{
		thread_active = false;

		//connecting to myself to un-block accept listener
		SOCKET temp_socket = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in temp_addr;
		memset(&temp_addr, 0, sizeof(temp_addr));
		temp_addr.sin_family = AF_INET;
		temp_addr.sin_port = htons(NetworkConnector::Constants::port_number);
		temp_addr.sin_addr.s_addr = inet_addr(NetworkConnector::Constants::ip_loopback.c_str());
		connect(temp_socket, reinterpret_cast<sockaddr*>(&temp_addr), sizeof(temp_addr));
		closesocket(temp_socket);

		if (main_thread->joinable())
		{
			main_thread->join();
		}
	}
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::BanClient(sockaddr_in address)
{
	enclosing_class_ptr->network_mutex.lock();
	blacklist.push_back(address);
	enclosing_class_ptr->network_mutex.unlock();
	enclosing_class_ptr->RemoveClient(address);
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ClientConnector::UnbanClient(sockaddr_in address)
{
	enclosing_class_ptr->network_mutex.lock();
	for (size_t i = 0; i < enclosing_class_ptr->clients.size(); ++i)
	{
		if (blacklist[i].sin_addr.s_addr == address.sin_addr.s_addr)
		{
			blacklist.erase(blacklist.begin() + i);
			enclosing_class_ptr->network_mutex.unlock();
			return;
		}
	}
	//CHECK IN DEBUG or compile time
	DebugBreak();
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
		//TODO
		//implement this
	}
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ErrorHandler::Start()
{
	if (thread_active)//CHECK IN DEBUG or compile time
	{
		thread_active = false;
		if (main_thread->joinable())
		{
			main_thread->join();
		}
	}
}
TEMPLATE_NetworkConnectorHost inline void NetworkConnectorHost<T, Method>::ErrorHandler::Stop()
{
	if (!thread_active)//CHECK IN DEBUG or compile time
	{
		thread_active = true;
		main_thread = std::make_unique<std::thread>(&ErrorHandler::Handling, this);
	}
}

