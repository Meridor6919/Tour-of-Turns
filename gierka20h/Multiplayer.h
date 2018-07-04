#pragma once
#include "ToT.h"
#include "Player.h"

extern bool BroadcastingWorking;

class MultiPlayer : public Player
{
	std::vector<std::string> message1;
	std::vector<std::string> message2;

public:

	void Init1(SOCKET Client, std::string file[], std::string ext[]);
	void Init(SOCKET host, int width, int color, int color2);
	void VisionBox(SOCKET host, int color, int color2);
	void SendArr(SOCKET Client, std::vector<std::string> Tour, int position);
	bool Action(float &value, int &position, int color, int color2);
	void AssignAction(int position, int value);
	bool Test(std::string field, bool show, int color, int color2);
	void SendData(SOCKET client, int PlayersAlive);
	void RecvData(SOCKET host, int color, int color2);
};

struct HostDataBase
{
	int MemoryAllocated;
	int UsedSpace;
	std::string*Hosts;
	std::string *Addresses;

	HostDataBase(int HowMany);
	void ChangeSize(int Howmany);
	int AddNewHost(std::string Name, std::string IP);
};

void Broadcasting();
int ClientsLobby(HostDataBase &HDB, short width, int color, int color2);
SOCKET ConnectingWithHost(std::string HostAddress);
int HostsLobby(SOCKET ClientsConnected[], char ClientNames[][50], int width, int color, int color2);
