#pragma once
#include "NetworkConnectorGeneral.h"
#include "BroadcastReceiver.h"

class NetworkConnectorClient
{
	SOCKET host = INVALID_SOCKET;
	bool connected = false;

	static bool Recv(SOCKET socket, char* buffer, int len, const int flags);
	std::string GetIPFromHostID(std::string host_id);
	std::mutex network_mutex;
public:
	NetworkConnector::BroadcastReceiver broadcast_receiver;

	NetworkConnectorClient();
	bool Connect(const std::string host_id);
	void SendRequest(const std::string request);
	bool GetConnectionFlag();
	std::string GetResponse();
	void CloseAllConnections();
};



