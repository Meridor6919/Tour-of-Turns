#pragma once
#include "GeneralMultiPlayer.h"
#include "ToT_Window.h"
#include <fstream>
#include <utility>
#include <set>
#include <vector>
#include <exception>
#include "Infobox.h"
#include "Participant.h"

class SinglePlayer {

protected:
	std::vector<Participant*> *participants;

public:

	ToT_Window *main_window;
	InfoBox *infobox;

	SinglePlayer(ToT_Window &main_window, std::vector<Participant*> *participants);
	virtual void CloseConnection() {};
	virtual void GetTourNames(std::vector<std::string>&tours);
	virtual void GetCarNames(std::vector<std::string>&cars, std::string tour);
	virtual void GetTireNames(std::vector<std::string>&tires);
	virtual std::vector<int> GetCarParameters(std::string path);
	virtual std::vector<std::string> GetTireParameters(std::string path);
	virtual std::vector<std::string> GetTourParameters(std::string path);
	virtual void GetOtherParticipants(int ais, std::string tour);
	virtual std::vector<std::pair<float, std::string>> GetRankingInfo();
	virtual bool GetCurrentAtribs(int real_players, std::string field);
	virtual void Attack(int ais);
	virtual void TakeAction();
	virtual void GetOthersAction(int ais, std::vector<std::string> &tour);
	virtual int Possible_AIs();
};

class Host : public SinglePlayer {

	GeneralMultiPlayer::Host *host;
	GeneralMultiPlayer::RequestHandler *request_handler;
	std::vector<std::pair<SOCKET, sockaddr_in>> *clients;
	bool StartNetwork();
	std::string tour;
	int stage;

public:

	void MsgHandling(std::string msg, int client_id);
	Host(ToT_Window &main_window, std::vector<Participant*> *participants);
	void CloseConnection();
	void GetOtherParticipants(int ais, std::string tour);
	std::vector<std::pair<float, std::string>> GetRankingInfo();
	bool GetCurrentAtribs(int real_players, std::string field);
	void Attack(int ais);
	void TakeAction();
	void GetOthersAction(int ais, std::vector<std::string> &tour);

	int Possible_AIs();
};

class Client : public SinglePlayer {

	GeneralMultiPlayer::Client *client;
	SOCKET host;
	bool StartNetwork();
	std::string tire_path; //needed for communication with host
	int stage;

public:

	Client(ToT_Window &main_window, std::vector<Participant*> *participants);
	void CloseConnection();
	void GetTourNames(std::vector<std::string>&tours);
	void GetCarNames(std::vector<std::string>&cars, std::string tour);
	void GetTireNames(std::vector<std::string>&tires);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	std::vector<std::string> GetTourParameters(std::string path);
	void GetOtherParticipants(int ais, std::string tour);
	std::vector<std::pair<float, std::string>> GetRankingInfo();
	bool GetCurrentAtribs(int real_players, std::string field);
	void Attack(int ais);
	void TakeAction();
	void GetOthersAction(int ais, std::vector<std::string> &tour);
	int Possible_AIs();
};

