#include "NetworkRoleClient.h"

Client::Client(ToT_Window& main_window) : SinglePlayer(main_window)
{
	this->participants = participants;
	this->main_window = &main_window;
	if (!StartNetwork())
	{
		initiazlized = false;
		client_connector->CloseAllConnections();
	}
}
bool Client::StartNetwork()
{
	HANDLE output_handle = main_window->GetOutputHandle();
	COORD starting_point = { (short)main_window->GetCharactersPerRow() / 2, 25 };
	constexpr short spacing = 2;
	std::string selected_game = "";
	bool return_value = false;
	client_connector = std::make_unique<NetworkConnectorClient>();

	client_connector->broadcast_receiver.Start(main_window->GetHamachiConnectionFlag());

	while (true)
	{
		Text::TextInfo text_info(LanguagePack::text[LanguagePack::multiplayer_client_lobby], 0, starting_point, TextAlign::center, spacing, false);
		int option = Text::Choose::Vertical(text_info, *main_window->GetWindowInfo());
		if (option == Multiplayer::active_games)
		{
			std::vector<std::string>active_games = { LanguagePack::text[LanguagePack::multiplayer_client_lobby][Multiplayer::back] };
			{
				auto temp_vector = client_connector->broadcast_receiver.GetHostVector();
				active_games.insert(active_games.begin() + 1, temp_vector.begin(), temp_vector.end());
			}
			const COORD submenu_position = { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_client_lobby][Multiplayer::active_games].size()) / 2 + 1,
												starting_point.Y };
			Text::TextInfo text_info(active_games, 0, submenu_position, TextAlign::left, 0, true);
			if (int target = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo()))
			{
				selected_game = active_games[target];
				HighlightSelectedGame(selected_game, false);
			}
		}
		else if (option == Multiplayer::join)
		{
			if (selected_game != "")
			{
				if (client_connector->Connect(selected_game))
				{
					return_value = true;
					break;
				}
				else
				{
					HighlightSelectedGame(selected_game, true);
					selected_game = "";
				}
			}
		}
		else if (option == Multiplayer::refresh)
		{
			client_connector->broadcast_receiver.ResetHostVector();
			HighlightSelectedGame(selected_game, true);
			selected_game = "";
		}
		else if (option == Multiplayer::back)
		{
			break;
		}
	}
	HighlightSelectedGame(selected_game, true);
	client_connector->broadcast_receiver.Stop();
	for (short i = 0; i < static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_client_lobby].size()); ++i)
	{
		const short text_size = static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_client_lobby][i].size());
		SetConsoleCursorPosition(output_handle, { starting_point.X - text_size / 2, starting_point.Y + i * spacing });
		for (int j = 0; j < text_size; ++j)
		{
			std::cout << ' ';
		}
	}
	return return_value;
}
void Client::HighlightSelectedGame(std::string game, bool clear)
{
	HANDLE output_handle = main_window->GetOutputHandle();
	if (clear)
	{
		for (int i = 0; i < static_cast<int>(game.size()); ++i)
		{
			game[i] = ' ';
		}
	}
	SetConsoleCursorPosition(output_handle, { 0,0 });
	SetColor(output_handle, *main_window->main_color);
	std::cout << game;
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








	client_connector->CloseAllConnections();
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