#include "NetworkRoleClient.h"

Client::Client(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->participants = participants;
	this->main_window = &main_window;
	if (!StartNetwork())
	{
		closesocket(host);
		initiazlized = false;
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
	const short spacing = 2;
	std::string selected_game = "";
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
		mutex.lock();
		int option = Text::Choose::Veritcal(LanguagePack::text[LanguagePack::multiplayer_lobby], 0, starting_point, spacing, Text::TextAlign::center, false, *main_window);
		mutex.unlock();
		if(option == Multiplayer::active_games)
		{
			std::vector<std::string>active_games = current_hosts;
			active_games.insert(active_games.begin(), LanguagePack::text[LanguagePack::multiplayer_lobby][Multiplayer::back]);
			mutex.lock();
			int i = Text::Choose::Horizontal(active_games, 0, {starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_lobby][Multiplayer::active_games].size())/2 + 1, starting_point.Y}, Text::TextAlign::left, true, *main_window);
			mutex.unlock();
			HighlightSelectedGame(selected_game, true);
			if (i != 0)
			{
				selected_game = current_hosts[i-1];
			}
			else
			{
				selected_game = "";
			}
			HighlightSelectedGame(selected_game, false);
		}
		else if(option == Multiplayer::join)
		{
			if (selected_game != "")
			{
				if (client->Connect(client->GetIpFromMapValue(selected_game)))
				{
					return_value = true;
					break;
				}
				else
				{
					HighlightSelectedGame(selected_game, true);
					current_hosts = client->GetCurrentHosts();
					selected_game = "";
				}
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
	for (short i = 0; i < static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_lobby].size()); ++i)
	{
		const short text_size = static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_lobby][i].size());
		SetConsoleCursorPosition(handle, { starting_point.X - text_size/2, starting_point.Y + i * spacing });
		for (int j = 0; j < text_size; ++j)
		{
			std::cout << ' ';
		}
	}
	return return_value;
}
void Client::HighlightSelectedGame(std::string game, bool clear)
{
	HANDLE handle = main_window->GetHandle();
	if (clear)
	{
		for (int i = 0; i < static_cast<int>(game.size()); ++i)
		{
			game[i] = ' ';
		}
	}
	mutex.lock();
	SetConsoleCursorPosition(handle, { 0,0 });
	SetConsoleTextAttribute(handle, main_window->color1);
	std::cout << game;
	mutex.unlock();
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