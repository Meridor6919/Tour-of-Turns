#pragma once
#include "NetworkRole.h"

class Host : public SinglePlayer {

	std::vector<std::pair<SOCKET, sockaddr_in>> clients;
	std::unique_ptr<MeridorMultiplayer::Host> host;

	void HandleClientConnection(std::string msg, int client_id);
	virtual int Possible_AIs() override;
	void ShowClientsInLobby(bool *running);
public:
	//General
	Host(ToT_Window &main_window);

	//Get methods
	virtual void GetParticipants(std::string name, std::string tour, std::string car, std::string tire) override;
	virtual void GetCurrentAttributes() override;

	//Game methods
	virtual bool GameLobby() override;
	virtual void AttackPhase() override;
	virtual void ActionPhase() override;
	virtual void Leaderboard(bool clear) override;
	virtual bool VisionBox(int turn) override;
	virtual void Interface() override;
	virtual void Finish() override;
};