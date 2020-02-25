#pragma once
#include "NetworkRole.h"

class Client : public SinglePlayer {

	SOCKET host;
	std::unique_ptr<MeridorMultiplayer::Client> client;

	bool StartNetwork();
	virtual void ValidateAttack(int target, int participant)override;
	virtual void ValidateAction(std::pair<int, int> action, int participant)override;
	virtual int Possible_AIs()override;
public:
	ToT_Window *main_window;

	//General
	Client(ToT_Window &main_window);

	//Get methods
	virtual void GetParticipants(std::string name, std::string tour, std::string car, std::string tire)override;
	virtual void GetCurrentAttributes()override;
	virtual std::string GetTour()override;

	//Game methods
	virtual bool GameLobby() override;
	virtual void AttackPhase()override;
	virtual void ActionPhase()override;
	virtual void Leaderboard(bool clear)override;
	virtual bool VisionBox(int turn)override;
	virtual void Interface()override;
	virtual void Finish()override;
};