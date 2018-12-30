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

protected:

	ToT_Window *main_window;

public:
	SinglePlayer(ToT_Window &main_window);
	virtual void GetTourNames(std::vector<std::string>&tours);
	virtual void GetCarNames(std::vector<std::string>&cars, std::string tour);
	virtual void GetTireNames(std::vector<std::string>&tires);
	virtual std::vector<int> GetCarParameters(std::string path);
	virtual std::vector<std::string> GetTireParameters(std::string path);
	virtual std::vector<std::string> GetTourParameters(std::string path);
	virtual void GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour);

	virtual std::vector<std::pair<float, std::string>> GetRankingInfo(std::vector<Participant*> &participants);
	virtual void GetInfobox(InfoBox *infobox);
	virtual void GetCurrentAtribs(Participant* &participant);
	virtual void Attack(std::vector<Participant*> &participants, int ais);

	virtual void SendInfo(std::string special_text, std::string text);
	virtual void TakeAction(Participant* &participants);
	virtual void GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string> &tour);
	virtual void SendTarget(int ranking_position);

	virtual int Possible_AIs();
};

class Host : public SinglePlayer {

	std::vector<std::pair<SOCKET, sockaddr_in>> clients;
	MultiplayerDevice *network_device;
	bool StartNetwork(std::vector<Participant*> *participants);

public:

	Host(ToT_Window &main_window, std::vector<Participant*> *participants);
	void GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour);

	std::vector<std::pair<float, std::string>> GetRankingInfo(std::vector<Participant*> &participants);
	void GetInfobox(InfoBox *infobox);
	void GetCurrentAtribs(Participant* &participant);
	void Attack(std::vector<Participant*> &participants, int ais);

	void SendInfo(std::string special_text, std::string text);
	void TakeAction(Participant* &participants);
	void GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string> &tour);
	void SendTarget(int ranking_position);

	int Possible_AIs();
};

class Client : public SinglePlayer {

	SOCKET host;
	bool StartNetwork();

public:

	Client(ToT_Window &main_window);
	void GetTourNames(std::vector<std::string>&tours);
	void GetCarNames(std::vector<std::string>&cars, std::string tour);
	void GetTireNames(std::vector<std::string>&tires);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	std::vector<std::string> GetTourParameters(std::string path);
	void GetOtherParticipants(std::vector<Participant*> &participants, int ais, std::string tour);

	std::vector<std::pair<float, std::string>> GetRankingInfo(std::vector<Participant*> &participants);
	void GetInfobox(InfoBox *infobox);
	void GetCurrentAtribs(Participant* &participant);
	void Attack(std::vector<Participant*> &participants, int ais);

	void SendInfo(std::string special_text, std::string text);
	void TakeAction(Participant* &participants);
	void GetOthersAction(std::vector<Participant*>& participants, int ais, std::vector<std::string> &tour);
	void SendTarget(int ranking_position);

	int Possible_AIs();
};

