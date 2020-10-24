#pragma once
#include "NetworkConnectorGeneral.h"
class NetworkConnectorClient
{
	SOCKET host = INVALID_SOCKET;
	std::vector<std::string> hosts_broadcasting;
	std::unique_ptr<std::thread> broadcast_searching;
	bool broadcast_active = false;
	bool connected = false;
	std::mutex network_mutex;

	static bool Recv(SOCKET socket, char* buffer, int len, const int flags);
	std::string GetIPFromHostID(std::string host_id);
	void BroadcastSearch(bool hamachi);
public:
	NetworkConnectorClient();
	bool Connect(const std::string host_id);
	void StartLookingForHosts(bool hamachi);
	void StopLookingForHosts();
	void SendRequest(const std::string request);
	bool GetConnectionFlag();
	std::string GetResponse();
	std::vector<std::string> GetHostsBroadcasting();
	void ResetHostsBroadcastingVector();
	void CloseAllConnections();
};


