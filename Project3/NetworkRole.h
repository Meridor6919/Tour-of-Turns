#pragma once
#include "GeneralMultiPlayer.h"
#include "ToT_Window.h"
#include <fstream>
#include <set>
#include <vector>
#include <exception>
#include "Participant.h"
#include <mutex>
#include "VisibleTimer.h"
#include "AIConnector.h"

class SinglePlayer 
{
protected:
	std::string current_field;
	std::string tour;
	short take_action_position = 0;
	std::vector<Participant> participants;
	std::unique_ptr<GeneralMultiPlayer::RequestHandler> request_handler;
	std::unique_ptr<AIConnector> ai_connector;
	std::mutex mutex;
	bool timer_running = true;
	std::unique_ptr<VisibleTimer> timer;
	int ai_init = 0;

	//Selection methods
	int NumericalSelection(const COORD coords);
	int BinarySelection(const COORD coords);
	std::string StringSelection(const std::string current_name, const int max_size, const COORD coords);

	//Showing methods
	void ShowCarParameters(const std::string tire_path, bool clear = false);
	void ShowTiresParameters(const std::string tire_path, bool clear = false);
	void ShowTourParameters(const std::string tire_path, bool clear = false);
	void ShowRankingParameters(const std::string tire_path, bool clear = false);
	void ShowLobbyInformation(const std::string title, const std::vector<std::pair<std::string, std::string>> text, const COORD base_position, const short paragraph_size, const short spacing, const bool clear);
	void ShowLeaderboard(const std::vector<std::string> text, short pos, int color, const bool clear = false);
	void ShowLoadingAI(const bool clear = false);
	void ShowChances(int value, bool reset = false);
	void ShowIndicator(int participant, bool clear = false);

	//Game logic
	void ValidateAttack(int target, int participant);
	void ValidateAction(std::pair<int, int> action, int participant);
	int PerformAttack();
	void HandleAIConnection(std::string msg_received);
	std::pair<int, int> PerformAction(); //action, value
	virtual int Possible_AIs();


public:
	ToT_Window *main_window;

	//General
	SinglePlayer(ToT_Window &main_window);
	
	//Get methods
	virtual void GetParticipants(std::string name, std::string tour, std::string car, std::string tire);
	void SortLeaderboard();
	virtual void GetCurrentAttributes();
	std::string GetTour();

	//Game methods
	bool GameLobby();
	virtual void AttackPhase();
	virtual void ActionPhase();
	virtual void Leaderboard(bool clear);
	virtual bool VisionBox(int turn);
	virtual void Interface();
	virtual void Finish();
};












































class Host : public SinglePlayer {

	GeneralMultiPlayer::Host *host;
	std::vector<std::pair<SOCKET, sockaddr_in>> *clients;
	bool StartNetwork();
	int stage = 0;

public:

	void MsgHandling(std::string msg, int client_id);
	Host(ToT_Window &main_window);
	~Host();
	void GetParticipants(std::string name, std::string tour, std::string car, std::string tire);
	void SortLeaderboard();
	void GetCurrentAttributes();
	void Attack();
	void TakeAction();
	void GetOthersAction(std::vector<std::string> &tour);

	int Possible_AIs();
};
class Client : public SinglePlayer {

	GeneralMultiPlayer::Client *client;
	SOCKET host;
	bool StartNetwork();
	std::string tire_path; //needed for communication with host
	int stage;

public:

	Client(ToT_Window &main_window);
	~Client();
	void GetTourNames(std::vector<std::string>&tours);
	void GetCarNames(std::vector<std::string>&cars, std::string tour);
	void GetTireNames(std::vector<std::string>&tires);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	std::vector<std::string> GetTourParameters(std::string path);
	void GetParticipants(std::string name, std::string tour, std::string car, std::string tire);
	void SortLeaderboard();
	void GetCurrentAttributes();
	void Attack();
	void TakeAction();
	void GetOthersAction(std::vector<std::string> &tour);
	int Possible_AIs();
};

