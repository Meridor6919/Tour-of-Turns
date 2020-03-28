#include "NetworkRoleClient.h"

Client::Client(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->participants = participants;
	this->main_window = &main_window;
	if (!StartNetwork())
	{
		closesocket(host);
		throw 1;
	}
}
bool Client::StartNetwork()
{
	HANDLE handle = main_window->GetHandle();
	COORD starting_point = { (short)main_window->GetWidth() / 2, 25 };
	short cursor_pos = 0;
	char button = ' ';

	client = std::make_unique<MeridorMultiplayer::Client>(&host);
	std::vector<std::string> current_hosts = client->GetCurrentHosts();
	int selected_game = Multiplayer::back;
	bool return_value = false;

	//recv hosts from local network	
	std::thread receiving_broadcast([&]() {
		if (!client->RecvBroadcast(8, 200))
		{
			return false;
		}
	});
	while (true)
	{

		int option = Text::Choose::Veritcal(LanguagePack::text[LanguagePack::multiplayer_lobby], 0, starting_point, 2, Text::TextAlign::center, true, *main_window);
		if(option == Multiplayer::active_games)
		{
			//choose game
			//horizontal choose
			//highlight choosen game
		}
		else if(option == Multiplayer::join)
		{
			if (selected_game != Multiplayer::back)
			{
				client->Connect(client->GetIpFromMapValue(current_hosts[selected_game]));
				return_value = true;
				break;
			}
		}
		else if(option == Multiplayer::refresh)
		{
			current_hosts = client->GetCurrentHosts();
		}
		else if(option == Multiplayer::back)
		{
			break;
		}
	}
	client->FinishBroadcast();
	receiving_broadcast.join();
	return return_value;
}
void Client::ValidateAttack(int target, int participant)
{
}
void Client::ValidateAction(std::pair<int, int> action, int participant)
{
}
int Client::Possible_AIs()
{
	return 0;
}
void Client::GetParticipants(std::string name, std::string tour, std::string car, std::string tire)
{
}
void Client::GetCurrentAttributes()
{
}
std::string Client::GetTour()
{
	return std::string();
}
bool Client::GameLobby()
{
	return false;
}

void Client::AttackPhase()
{
}
void Client::ActionPhase()
{
}
void Client::Leaderboard(bool clear)
{
}
bool Client::VisionBox(int turn)
{
	return false;
}
void Client::Interface()
{
}
void Client::Finish()
{
}