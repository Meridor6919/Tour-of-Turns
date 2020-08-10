#pragma once
#include "MeridorMultiplayerClient.h"
#include "MeridorMultiplayerHost.h"
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
	std::string current_field = "0";
	std::string tour;
	short take_action_position = 0;
	std::vector<Participant> participants;
	std::unique_ptr<AIConnector> ai_connector;
	std::mutex mutex;
	bool attack_phase = false;
	bool timer_running = true;
	std::unique_ptr<VisibleTimer> timer;
	int ai_init = 0;
	bool initiazlized = false;

	//Selection methods
	int NumericalSelection(const COORD coords);
	int BinarySelection(const COORD coords);
	std::string StringSelection(const std::string current_name, const int max_size, const COORD coords);

	//Showing methods
	void ShowCarParameters(const std::string tire_path, bool clear = false, const COORD starting_pos = { 0,19 });
	void ShowTiresParameters(const std::string tire_path, bool clear = false, const COORD starting_pos = { 0,38 });
	void ShowTourParameters(const std::string tire_path, bool clear = false);
	void ShowRankingParameters(const std::string tire_path, bool clear = false);
	void ShowLobbyInformation(const std::string title, const std::vector<std::pair<std::string, std::string>> text, const COORD base_position, const short paragraph_size, const short spacing, const bool clear);
	void ShowLeaderboard(const std::vector<std::string> text, short pos, int color, const bool clear = false);
	void ShowLoading(std::string title, int &init, int condition, bool clear = false);
	void ShowChances(int value, bool reset = false);
	void ShowIndicator(int participant, bool clear = false);

	//Game logic
	virtual void ValidateAttack(int target, int participant);
	virtual void ValidateAction(std::pair<int, int> action, int participant);
	int PerformAttack();
	void HandleAIConnection(std::string msg_received);
	std::pair<int, int> PerformAction(); //action, value
	virtual int Possible_AIs();

public:
	ToT_Window *main_window;

	//General
	SinglePlayer(ToT_Window &main_window);
	bool isInit();
	
	//Get methods
	virtual void GetParticipants(std::string name, std::string tour, std::string car, std::string tire);
	void SortLeaderboard();
	virtual void GetCurrentAttributes();
	virtual std::string GetTour();

	//Game methods
	virtual bool GameLobby();
	virtual void AttackPhase();
	virtual void ActionPhase();
	virtual void Leaderboard(bool clear);
	virtual bool VisionBox(int turn);
	virtual void Interface();
	virtual void Finish();
};