#include "NetworkRoleHost.h"

Host::Host(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	host = std::make_unique<MeridorMultiplayer::Host>();


	std::vector<std::string> horizontal_menu_text;

	for (int i = 1; i <= SinglePlayer::Possible_AIs(); ++i)
	{
		horizontal_menu_text.push_back(std::to_string(i));
	}
	COORD starting_point = { static_cast<short>(main_window.GetWidth() + LanguagePack::text[LanguagePack::multiplayer_before_game_lobby][0].size()) / 2 + 1, 25 };
	SetConsoleCursorPosition(main_window.GetHandle(), starting_point);
	std::string text = " : " + LanguagePack::text[LanguagePack::other_strings][OtherStrings::lobby_size];
	std::cout << text;

	lobby_size = Text::Choose::Horizontal(horizontal_menu_text, 0, {starting_point.X + static_cast<short>(text.size()) + 2, starting_point.Y}, Text::TextAlign::left, true, main_window, &mutex, &timer_running) + 1;
	SetConsoleCursorPosition(main_window.GetHandle(), starting_point);
	Text::Spaces(static_cast<short>(text.size()));
}
void Host::HandleClientConnection(std::string msg, int client_id)
{
}
int Host::Possible_AIs()
{
	return SinglePlayer::Possible_AIs() - lobby_size;
}
void Host::ShowClientsInLobby(const COORD starting_position, bool *running)
{
	const HANDLE handle = main_window->GetHandle();
	
	mutex.lock();
	const short border_size = static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::border].size());
	const short title_size = static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::players_in_lobby].size());
	const short box_size = (lobby_size < 4 ? 4 : lobby_size);

	SetConsoleTextAttribute(handle, main_window->color2);
	SetConsoleCursorPosition(handle, { starting_position.X + border_size/2 - title_size/2, starting_position.Y });
	std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::players_in_lobby];
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y+1 });
	std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y + 3 + static_cast<short>(box_size) * 2 });
	std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::border];
	mutex.unlock();

	while (*running)
	{
		const std::vector<std::pair<SOCKET, sockaddr_in>> clients = *(host->GetClientsPtr());
		mutex.lock();
		for (short i = 0; i < lobby_size; ++i)
		{
			SetConsoleCursorPosition(handle, { 0,i+1 });
			SetConsoleTextAttribute(handle, main_window->color1);
			for (short j = 0; j < MeridorMultiplayer::Constants::max_ip_size; ++j)
			{
				SetConsoleCursorPosition(handle, { starting_position.X + 1 + j, starting_position.Y + 2*(i+1) + 2 });
				std::cout << ' ';
			}
		}
		for (short i = 0; i < static_cast<short>(clients.size()); ++i)
		{
			
			SetConsoleCursorPosition(handle, { starting_position.X + 1, starting_position.Y + 2 * (i + 1) + 2 });
			SetConsoleTextAttribute(handle, main_window->color1);
			std::cout << host->GetThisIp(clients[i].second);
		}
		mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	mutex.lock();
	for (short i = 0; i < lobby_size; ++i)
	{
		for (short j = 0; j < static_cast<short>(MeridorMultiplayer::Constants::ip_loopback.size()); ++j)
		{
			SetConsoleCursorPosition(handle, { starting_position.X + 1 + j, starting_position.Y + 2 * (i + 1) + 2 });
			std::cout << ' ';
		}
	}
	SetConsoleCursorPosition(handle, { starting_position.X + border_size / 2 - title_size / 2, starting_position.Y });
	Text::Spaces(title_size);
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y + 1 });
	Text::Spaces(border_size);
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y + 3 + static_cast<short>(lobby_size) * 2 });
	Text::Spaces(border_size);
	mutex.unlock();
}
void Host::GetParticipants(std::string name, std::string tour, std::string car, std::string tire)
{
	SinglePlayer::GetParticipants(name, tour, car, tire);
	int human_players = static_cast<int>(player_init, host->GetClientsPtr()->size());
	if (human_players == 0)
	{
		main_window->SetMultiplayer(false);
	}
	ShowLoading(LanguagePack::text[LanguagePack::other_strings][OtherStrings::loadingplayer_title], player_init, human_players);
	ShowLoading(LanguagePack::text[LanguagePack::other_strings][OtherStrings::loadingplayer_title], player_init, human_players, true);
}
bool Host::GameLobby()
{

	short box_shift = 2 * (lobby_size - 4);
	if (box_shift < 0)
	{
		box_shift = 0;
	}
	const COORD client_box_starting_pos = { 0, 1 };
	const COORD car_box_starting_pos = { 0, 19 + box_shift };
	const COORD tire_box_starting_pos = { 0, 38 + box_shift };

	bool show_clients = true;
	std::thread broadcast(&MeridorMultiplayer::Host::Broadcast, host.get(), main_window->GetHamachiConnectionFlag(), 200);
	std::thread accept_clients(&MeridorMultiplayer::Host::AcceptClients, host.get(), lobby_size);
	std::thread show_clients_in_lobby(&Host::ShowClientsInLobby, this, client_box_starting_pos, &show_clients);
	const HANDLE handle = main_window->GetHandle();
	COORD starting_point = { static_cast<short>(main_window->GetWidth()) / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	std::string name = main_window->GetName();
	int timer_settings = main_window->GetTimerSettings();
	std::vector<std::string> tours = main_window->GetTourNames();
	std::vector<std::string> tires = main_window->GetTireNames();
	std::vector<std::string> cars = main_window->GetCarNames(tours[0]);
	main_window->RemoveExtension(tours, ExtName::tour);
	main_window->RemoveExtension(tires, ExtName::tire);
	main_window->RemoveExtension(cars, ExtName::car);
	int ais = (main_window->GetAIs() > Possible_AIs() ? Possible_AIs() : main_window->GetAIs());
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;

	ShowCarParameters(cars[cars_pos] + ExtName::car, false, car_box_starting_pos);
	ShowTiresParameters(tires[tires_pos] + ExtName::tire, false, tire_box_starting_pos);
	ShowTourParameters(tours[tours_pos] + ExtName::tour);

	while (true)
	{
		main_menu_position = Text::Choose::Veritcal(LanguagePack::text[LanguagePack::multiplayer_menu_options], main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window, &mutex, &timer_running);
		if (!timer_running)
		{
			break;
		}
		const COORD starting_local_pos = { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_menu_options][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing };
		switch (main_menu_position)
		{
		case 0://choosing name
		{
			name = StringSelection(name, GameConstants::maximum_name_length, { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::multiplayer_menu_options][0].size()), 25 });
			main_window->SetName(name);
			break;
		}
		case 1://Number of ais
		{
			std::vector<std::string> text;

			for (int i = 0; i <= Possible_AIs(); ++i)
			{
				text.push_back(std::to_string(i));
			}
			ais = Text::Choose::Horizontal(text, ais, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
			ShowRankingParameters(tours[tours_pos] + ExtName::ranking, true);
			main_window->SetAIs(ais);
			ShowRankingParameters(tours[tours_pos] + ExtName::ranking);
			break;
		}
		case 2://ban players
		{
			std::vector<std::string> text = { LanguagePack::text[LanguagePack::multiplayer_lobby][Multiplayer::back] };
			const std::vector<std::pair<SOCKET, sockaddr_in>> clients = *(host->GetClientsPtr());
			for (int i = 0; i < static_cast<int>(clients.size()); ++i)
			{
				text.push_back(host->GetThisIp(clients[i].second));
			}
			int target = Text::Choose::Horizontal(text, 0, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
			if (target > 0)
			{
				host->BanPlayer(clients[target - 1].second);
			}
			break;
		}
		case 3://blacklist
		{
			std::vector<std::string> text = { LanguagePack::text[LanguagePack::multiplayer_lobby][Multiplayer::back] };
			const std::vector<sockaddr_in> blacklist = *(host->GetBlackListPtr());
			for (int i = 0; i < static_cast<int>(blacklist.size()); ++i)
			{
				text.push_back(host->GetThisIp(blacklist[i]));
			}
			int target = Text::Choose::Horizontal(text, 0, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
			if (target > 0)
			{
				host->UnbanPlayer(blacklist[target - 1]);
			}
			break;

		}
		case 4: //timer
		{
			std::vector<std::string> timer_values = { LanguagePack::text[LanguagePack::on_off][1] };
			for (int i = 1; i <= GameConstants::maximum_timer; ++i)
			{
				timer_values.push_back((i < 60 ? "0" : "") + std::to_string(i / 6) + ':' + std::to_string(i % 6) + '0');
			}
			timer_settings = Text::Choose::Horizontal(timer_values, timer_settings, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
			main_window->SetTimerSettings(timer_settings);
			break;
		}
		case 5://choosing tour
		{
			int i = tours_pos;
			tours_pos = Text::Choose::Horizontal(tours, tours_pos, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
			if (i != tours_pos)
			{
				ShowCarParameters(cars[cars_pos] + ExtName::car, true, car_box_starting_pos);
				cars = main_window->GetCarNames(tours[tours_pos] + ExtName::tour);
				main_window->RemoveExtension(cars, ExtName::car);
				cars_pos = 0;
				ShowCarParameters(cars[cars_pos] + ExtName::car, false, car_box_starting_pos);
				ShowTourParameters(tours[i] + ExtName::tour, true);
				ShowTourParameters(tours[tours_pos] + ExtName::tour);
			}

			break;
		}
		case 6://choosing car
		{
			int i = cars_pos;
			cars_pos = Text::Choose::Horizontal(cars, cars_pos, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
			if (i != cars_pos)
			{
				ShowCarParameters(cars[i] + ExtName::car, true, car_box_starting_pos);
				ShowCarParameters(cars[cars_pos] + ExtName::car, false, car_box_starting_pos);
			}
			break;
		}
		case 7://choosing tires
		{
			int i = tires_pos;
			tires_pos = Text::Choose::Horizontal(tires, tires_pos, starting_local_pos, Text::TextAlign::left, true, *main_window, &mutex, &timer_running);
			if (i != tires_pos)
			{
				ShowTiresParameters(tires[i] + ExtName::tire, true, tire_box_starting_pos);
				ShowTiresParameters(tires[tires_pos] + ExtName::tire, false, tire_box_starting_pos);
			}
			break;
		}
		case 9://Back
		{
			main_window->SaveAtributes();
			host->StopBroadcasting();
			host->StopAcceptingClients();
			show_clients = false;
			broadcast.join();
			accept_clients.join();
			show_clients_in_lobby.join();
			host->CloseActiveConnections();
			return false;
		}
		}
		if (main_menu_position == 8)//Next
		{
			break;
		}
	}
	for (int i = 0; i < static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_menu_options].size()); ++i)
	{
		mutex.lock();
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(Text::TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::text[LanguagePack::multiplayer_menu_options][i].size())), starting_point.Y + static_cast<short>(i * spacing) });
		Text::Spaces(static_cast<int>(LanguagePack::text[LanguagePack::multiplayer_menu_options][i].size()));
		mutex.unlock();
	}
	main_window->SaveAtributes();
	ShowTiresParameters(tires[tires_pos] + ExtName::tire, true, tire_box_starting_pos);
	ShowCarParameters(cars[cars_pos] + ExtName::car, true, car_box_starting_pos);
	ShowTourParameters(tours[tours_pos] + ExtName::tour, true);
	GetParticipants(name, tours[tours_pos] + ExtName::tour, cars[cars_pos] + ExtName::car, tires[tires_pos] + ExtName::tire);
	for (int i = 0; i < static_cast<int>(participants.size()); ++i)
	{
		participants[i].Init(tour);
	}
	if (timer_settings)
	{
		COORD coord = { 0,0 };
		timer = std::make_unique<VisibleTimer>(coord, main_window->GetHandle(), &timer_running, main_window->color1, &mutex);
		timer->StartTimer(timer_settings);
	}
	SortLeaderboard();
	host->StopBroadcasting();
	host->StopAcceptingClients();
	show_clients = false;
	broadcast.join();
	accept_clients.join();
	show_clients_in_lobby.join();
	return true;
}
void Host::Finish()
{
	SinglePlayer::Finish();
	host->CloseActiveConnections();
}
