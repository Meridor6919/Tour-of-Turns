#pragma once
#include "GeneralMultiPlayer.h"
#include "ToT_Window.h"
#include <fstream>
#include <set>
#include <vector>
#include <exception>
#include "Infobox.h"
#include "Participant.h"

class SinglePlayer {

public:

	ToT_Window *main_window;
	InfoBox *infobox;
	std::string current_field;
	std::vector<Participant*> participants;
	std::string tour;
	int ais;

	SinglePlayer(ToT_Window &main_window);
	virtual void CloseConnection() {};
	virtual void GetTourNames(std::vector<std::string>&tours);
	virtual void GetCarNames(std::vector<std::string>&cars, std::string tour);
	virtual void GetTireNames(std::vector<std::string>&tires);

	virtual std::vector<int> GetCarParameters(std::string path);
	virtual std::vector<std::string> GetTireParameters(std::string path);
	virtual std::vector<std::string> GetTourParameters(int position, int visibility);

	virtual void GetParticipants(std::string name, int ais, std::string tour, std::string car, std::string tire);

	virtual std::vector<std::pair<float, std::string>> GetRankingInfo();
	virtual bool GetCurrentAtribs();
	virtual void Attack();
	virtual void TakeAction();
	virtual void GetOthersAction(int turn);
	virtual int Possible_AIs();
	virtual int Ranking(bool clear);
	virtual void Interface();
	virtual bool VisionBox(int turn);
	void ShowChances(double chance_to_succeed, double estimated_time, double burned_durability, bool reset=false);
	double CalculateBurning(double value);
};

class Host : public SinglePlayer {

	GeneralMultiPlayer::Host *host;
	GeneralMultiPlayer::RequestHandler *request_handler;
	std::vector<std::pair<SOCKET, sockaddr_in>> *clients;
	bool StartNetwork();
	int stage;

public:

	void MsgHandling(std::string msg, int client_id);
	Host(ToT_Window &main_window);
	void CloseConnection();
	void GetParticipants(std::string name, int ais, std::string tour, std::string car, std::string tire);
	std::vector<std::pair<float, std::string>> GetRankingInfo(std::string current_field);
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
	void CloseConnection();
	void GetTourNames(std::vector<std::string>&tours);
	void GetCarNames(std::vector<std::string>&cars, std::string tour);
	void GetTireNames(std::vector<std::string>&tires);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	std::vector<std::string> GetTourParameters(std::string path);
	void GetParticipants(std::string name, int ais, std::string tour, std::string car, std::string tire);
	std::vector<std::pair<float, std::string>> GetRankingInfo(std::string current_field);
	bool GetCurrentAtribs();
	void Attack();
	void TakeAction();
	void GetOthersAction(std::vector<std::string> &tour);
	int Possible_AIs();
};

