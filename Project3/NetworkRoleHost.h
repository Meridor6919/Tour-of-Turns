#pragma once
#include <map>

#include "NetworkRole.h"

class Host : public SinglePlayer {

	int lobby_size;
	int player_init = 0;

	void HandleClientConnection(std::string msg, int client_id);
	virtual int Possible_AIs() override;
	void ShowClientsInLobby(const COORD starting_position, bool *running);

	void SetLobbySize();

	std::map<int, int> client_map;
	std::unique_ptr<NetworkConnectorHost<Host, &Host::HandleClientConnection>> network_connector;
public:
	//General
	Host(ToT_Window &main_window);

	//Get methods
	virtual void GetParticipants(std::string name, std::string tour, std::string car, std::string tire) override;

	//Game methods
	virtual bool GameLobby() override;
	virtual void Finish() override;

	friend class NetworkConnectorHost<Host, &Host::HandleClientConnection>;
};