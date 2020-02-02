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
class SinglePlayer 
{
protected:
	std::string current_field;
	std::string tour;
	short take_action_position = 0;
	std::vector<Participant> participants;
	std::unique_ptr<GeneralMultiPlayer::RequestHandler> request_handler;
	std::mutex mutex;
	bool timer_running = true;
	std::unique_ptr<VisibleTimer> timer;

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
	void ShowChances(int value, bool reset = false);

public:
	ToT_Window *main_window;

	//General
	SinglePlayer(ToT_Window &main_window);
	
	//Get methods
	virtual void GetParticipants(std::string name, std::string tour, std::string car, std::string tire);
	void SortLeaderboard();
	virtual bool GetCurrentAtribs();
	virtual void GetOthersAction(int turn);
	std::string GetTour();

	//Game methods
	bool GameLobby();
	virtual void Attack();
	virtual void TakeAction();
	virtual int Possible_AIs();
	virtual void Leaderboard(bool clear);
	virtual void Interface();
	virtual bool VisionBox(int turn);
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
	bool GetCurrentAtribs();
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
	bool GetCurrentAtribs();
	void Attack();
	void TakeAction();
	void GetOthersAction(std::vector<std::string> &tour);
	int Possible_AIs();
};

