#pragma once
#include "MultiplayerDevice.h"
#include "ToT_Window.h"
#include <fstream>
#include <utility>
#include <set>
#include <vector>
#include <exception>
#include "Infobox.h"
#include "Participant.h"

class SinglePlayer {

public:

	ToT_Window_ *main_window;
	InfoBox *infobox;

	SinglePlayer(ToT_Window_ &main_window);
	virtual void GetTourNames(std::vector<std::string>&tours);
	virtual void GetCarNames(std::vector<std::string>&cars, std::string tour);
	virtual void GetTireNames(std::vector<std::string>&tires);
	virtual std::vector<int> GetCarParameters(std::string path);
	virtual std::vector<std::string> GetTireParameters(std::string path);
	virtual std::vector<std::string> GetTourParameters(std::string path);
	virtual void GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour);
	virtual std::vector<std::pair<float, std::string>> GetRankingInfo(std::vector<Participant*> &participants);
	virtual bool GetCurrentAtribs(std::vector<Participant*> &participants, int ais, std::string field);
	virtual void Attack(std::vector<Participant*> &participants, int ais, bool alive);
	virtual void TakeAction(Participant* &participants);
	virtual void GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string> &tour);

	virtual int Possible_AIs();
};

class Host : public SinglePlayer {

	std::vector<std::pair<SOCKET, sockaddr_in>> clients;
	MultiplayerDevice *network_device;
	bool StartNetwork(std::vector<Participant*> *participants);
	int stage;

public:

	Host(ToT_Window_ &main_window, std::vector<Participant*> *participants);
	void GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour);
	std::vector<std::pair<float, std::string>> GetRankingInfo(std::vector<Participant*> &participants);
	bool GetCurrentAtribs(std::vector<Participant*> &participants, int ais, std::string field);
	void Attack(std::vector<Participant*> &participants, int ais, bool alive);
	void TakeAction(Participant* &participants);
	void GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string> &tour);

	int Possible_AIs();
};

class Client : public SinglePlayer {

	SOCKET host;
	bool StartNetwork();
	std::string tire_path; //needed for communication with host

public:

	Client(ToT_Window_ &main_window);
	void GetTourNames(std::vector<std::string>&tours);
	void GetCarNames(std::vector<std::string>&cars, std::string tour);
	void GetTireNames(std::vector<std::string>&tires);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	std::vector<std::string> GetTourParameters(std::string path);
	void GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour);
	std::vector<std::pair<float, std::string>> GetRankingInfo(std::vector<Participant*> &participants);
	bool GetCurrentAtribs(std::vector<Participant*> &participants, int ais, std::string field);
	void Attack(std::vector<Participant*> &participants, int ais, bool alive);
	void TakeAction(Participant* &participants);
	void GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string> &tour);
	int Possible_AIs();
};

