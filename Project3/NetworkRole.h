#pragma once
#include <fstream>
#include <utility>
#include "Infobox.h"
#include "Participant.h"

class SinglePlayer {

protected:

	Window *main_window;

public:
	int id;

	SinglePlayer(Window &main_window);
	virtual void GetTourNames(std::vector<std::string>&tours);
	virtual void GetCarNames(std::vector<std::string>&cars, std::string tour);
	virtual void GetTireNames(std::vector<std::string>&tires);
	virtual std::vector<int> GetCarParameters(std::string path);
	virtual std::vector<std::string> GetTireParameters(std::string path);
	virtual void GetTourParameters(std::string path);
	virtual void GetOtherParticipants(std::vector<Participant*> &participants, int ais);

	virtual void GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores);
	virtual void GetInfobox(InfoBox *infobox);
	virtual void GetCurrentAtribs(float &durability, float current_speed);
	virtual void GetTargetList(std::vector<std::string>&names);

	virtual void SendInfo(std::string special_text, std::string text);
	virtual std::pair<int, int> TakeAction();
	virtual void SendTarget(int ranking_position);

	virtual int Possible_AIs();
};

class Host : public SinglePlayer {

	std::vector<SOCKET> clients;
	void StartNetwork();

public:

	Host(Window &main_window);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	void GetTourParameters(std::string path);
	void GetOtherParticipants(std::vector<Participant*> &participants, int ais);

	void GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores);
	void GetInfobox(InfoBox *infobox);
	void GetCurrentAtribs(float &durability, float current_speed);
	void GetTargetList(std::vector<std::string>&names);

	void SendInfo(std::string special_text, std::string text);
	std::pair<int, int> TakeAction();
	void SendTarget(int ranking_position);

	int Possible_AIs();
};

class Client : public SinglePlayer {

	SOCKET host;
	void StartNetwork();

public:

	Client(Window &main_window);
	void GetTourNames(std::vector<std::string>&tours);
	void GetCarNames(std::vector<std::string>&cars, std::string tour);
	void GetTireNames(std::vector<std::string>&tires);
	std::vector<int> GetCarParameters(std::string path);
	std::vector<std::string> GetTireParameters(std::string path);
	void GetTourParameters(std::string path);
	void GetOtherParticipants(std::vector<Participant*> &participants, int ais);

	void GetRankingInfo(std::vector<std::string>&names, std::vector<int>&scores);
	void GetInfobox(InfoBox *infobox);
	void GetCurrentAtribs(float &durability, float current_speed);
	void GetTargetList(std::vector<std::string>&names);

	void SendInfo(std::string special_text, std::string text);
	std::pair<int, int> TakeAction();
	void SendTarget(int ranking_position);

	int Possible_AIs();
};

