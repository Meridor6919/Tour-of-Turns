#pragma once
#include "GeneralConnector.h"
#include "BroadcastReceiver.h"

class NetworkConnectorClient
{
	SOCKET host = INVALID_SOCKET;
	bool connected = false;
	std::string GetIPFromHostID(std::string host_id);

public:
	MeridorNetwork::BroadcastReceiver broadcast_receiver;

	NetworkConnectorClient();
	bool Connect(const std::string host_id);
	bool GetConnectionFlag();
	void SendRequest(std::string request);
	std::string GetResponse();
	void CloseAllConnections();
};



