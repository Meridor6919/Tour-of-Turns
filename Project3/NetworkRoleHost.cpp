#include "NetworkRoleHost.h"

Host::Host(ToT_Window &main_window) : SinglePlayer(main_window)
{
	this->main_window = &main_window;
	SetLobbySize();
	network_connector = std::make_unique<NetworkConnectorHost<Host, &Host::HandleClientConnection>>(this);
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
	const short border_size = static_cast<short>(GameConstants::box_width);
	const short title_size = static_cast<short>(LanguagePack::text[LanguagePack::other_strings][OtherStrings::players_in_lobby].size());
	const short box_size = (lobby_size < 4 ? 4 : lobby_size);

	SetConsoleTextAttribute(handle, *main_window->secondary_color);
	SetConsoleCursorPosition(handle, { starting_position.X + border_size/2 - title_size/2, starting_position.Y });
	std::cout << LanguagePack::text[LanguagePack::other_strings][OtherStrings::players_in_lobby];
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y+1 });
	std::cout << GetMonoCharacterString(GameConstants::box_width, '_');
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y + 3 + static_cast<short>(box_size) * 2 });
	std::cout << GetMonoCharacterString(GameConstants::box_width, '_');
	mutex.unlock();
	while (*running)
	{
		const std::vector<std::string> clients_names = network_connector->GetClientNames();
		mutex.lock();
		for (short i = 0; i < lobby_size; ++i)
		{
			SetConsoleCursorPosition(handle, { 0,i+1 });
			SetConsoleTextAttribute(handle, *main_window->main_color);
			for (short j = 0; j < NetworkConnector::Constants::max_ip_size; ++j)
			{
				SetConsoleCursorPosition(handle, { starting_position.X + 1 + j, starting_position.Y + 2*(i+1) + 2 });
				std::cout << ' ';
			}
		}
		for (short i = 0; i < static_cast<short>(clients_names.size()); ++i)
		{
			
			SetConsoleCursorPosition(handle, { starting_position.X + 1, starting_position.Y + 2 * (i + 1) + 2 });
			SetConsoleTextAttribute(handle, *main_window->main_color);
			std::cout << clients_names[i];
		}
		mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	mutex.lock();
	for (short i = 0; i < lobby_size; ++i)
	{
		for (short j = 0; j < static_cast<short>(NetworkConnector::Constants::ip_loopback.size()); ++j)
		{
			SetConsoleCursorPosition(handle, { starting_position.X + 1 + j, starting_position.Y + 2 * (i + 1) + 2 });
			std::cout << ' ';
		}
	}
	SetConsoleCursorPosition(handle, { starting_position.X + border_size / 2 - title_size / 2, starting_position.Y });
	std::cout << Spaces(title_size);
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y + 1 });
	std::cout << Spaces(border_size);
	SetConsoleCursorPosition(handle, { starting_position.X, starting_position.Y + 3 + static_cast<short>(lobby_size) * 2 });
	std::cout << Spaces(border_size);
	mutex.unlock();
}
void Host::SetLobbySize()
{
	std::vector<std::string> horizontal_menu_text;

	for (int i = 1; i <= SinglePlayer::Possible_AIs(); ++i)
	{
		horizontal_menu_text.push_back(std::to_string(i));
	}
	COORD starting_point = { static_cast<short>(main_window->GetCharactersPerRow() + LanguagePack::text[LanguagePack::multiplayer_menu][0].size()) / 2 + 1, 25 };
	SetConsoleCursorPosition(main_window->GetHandle(), starting_point);
	std::string text = " : " + LanguagePack::text[LanguagePack::other_strings][OtherStrings::lobby_size];
	std::cout << text;
	Text::TextInfo text_info = { horizontal_menu_text, 0, { starting_point.X + static_cast<short>(text.size()) + 2, starting_point.Y }, TextAlign::left, 0, true };
	MultithreadingData  multithreading_data= { &mutex, &timer_running };
	lobby_size = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data) + 1;
	SetConsoleCursorPosition(main_window->GetHandle(), starting_point);
	std::cout << Spaces(static_cast<short>(text.size()));
}
void Host::GetParticipants(std::string name, std::string tour, std::string car, std::string tire)
{
	SinglePlayer::GetParticipants(name, tour, car, tire);
	int human_players = static_cast<int>(network_connector->GetClientNames().size());
	if (human_players == 0)
	{
		multiplayer_flag = false;
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
	network_connector->broadcast_sender.Start(main_window->GetHamachiConnectionFlag());
	network_connector->client_connector.Start(lobby_size);

	std::thread show_clients_in_lobby(&Host::ShowClientsInLobby, this, client_box_starting_pos, &show_clients);
	const HANDLE handle = main_window->GetHandle();
	COORD starting_point = { static_cast<short>(main_window->GetCharactersPerRow()) / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	std::string name = main_window->GetName();
	int timer_settings = main_window->GetTimerSettings();
	std::vector<std::string> tours = GetTourNames();
	std::vector<std::string> tires = GetTireNames();
	std::vector<std::string> cars = GetCarNames(tours[0]);
	RemoveExtension(tours, ExtName::tour);
	RemoveExtension(tires, ExtName::tire);
	RemoveExtension(cars, ExtName::car);
	int ais = (main_window->GetAIs() > Possible_AIs() ? Possible_AIs() : main_window->GetAIs());
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;

	ShowCarParameters(cars[cars_pos] + ExtName::car, false, car_box_starting_pos);
	ShowTiresParameters(tires[tires_pos] + ExtName::tire, false, tire_box_starting_pos);
	ShowTourParameters(tours[tours_pos] + ExtName::tour);

	while (true)
	{
		Text::TextInfo text_info = { LanguagePack::text[LanguagePack::game_options_multiplayer], main_menu_position, starting_point, TextAlign::center, spacing, false };
		MultithreadingData  multithreading_data = { &mutex, &timer_running };
		main_menu_position = Text::Choose::Veritcal(text_info, *main_window->GetWindowInfo(), multithreading_data);
		if (!timer_running)
		{
			break;
		}
		const COORD starting_local_pos = { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::game_options_multiplayer][main_menu_position].size()) / 2 + spacing, starting_point.Y + main_menu_position * spacing };
		switch (main_menu_position)
		{
		case 0://choosing name
		{
			name = StringSelection(name, GameConstants::maximum_name_length, { starting_point.X + static_cast<short>(LanguagePack::text[LanguagePack::game_options_multiplayer][0].size()), 25 });
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
			Text::TextInfo text_info = { text, ais, starting_local_pos, TextAlign::left, 0, true };
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			ais = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			ShowRankingParameters(tours[tours_pos] + ExtName::ranking, true);
			main_window->SetAIs(ais);
			ShowRankingParameters(tours[tours_pos] + ExtName::ranking);
			break;
		}
		case 2://ban players
		{
			const std::vector<sockaddr_in> active_connections = network_connector->GetClientAddresses();
			std::vector<std::string> text = network_connector->GetClientNames();
			text.insert(text.begin(), LanguagePack::text[LanguagePack::multiplayer_client_lobby][Multiplayer::back]);

			Text::TextInfo text_info = { text, 0, starting_local_pos, TextAlign::left, 0, true };
			MultithreadingData  multithreading_data = { &mutex, &timer_running };

			if (int target = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data))
			{
				network_connector->client_connector.BanClient(active_connections[target-1]);
			}
			break;
		}
		case 3://blacklist
		{
			std::vector<std::string> unban_options = { LanguagePack::text[LanguagePack::multiplayer_client_lobby][Multiplayer::back] };
			const std::vector<sockaddr_in> banned_addresses = network_connector->client_connector.GetBlacklist();
			for (size_t i = 0; i < banned_addresses.size(); ++i)
			{
				unban_options.push_back(network_connector->GetIP(banned_addresses[i]));
			}

			Text::TextInfo text_info = { unban_options, 0, starting_local_pos, TextAlign::center, 0, true };
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			if (int target = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data))
			{
				network_connector->client_connector.UnbanClient(banned_addresses[target - 1]);
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

			Text::TextInfo text_info = { timer_values, timer_settings, starting_local_pos, TextAlign::left, 0, true };
			MultithreadingData  multithreading_data = { &mutex, &timer_running };
			timer_settings = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			main_window->SetTimerSettings(timer_settings);
			break;
		}
		case 5://choosing tour
		{
			int i = tours_pos;
			Text::TextInfo text_info = { tours, tours_pos, starting_local_pos, TextAlign::left, 0, true };
			MultithreadingData multithreading_data = { &mutex, &timer_running };
			tours_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			if (i != tours_pos)
			{
				ShowCarParameters(cars[cars_pos] + ExtName::car, true, car_box_starting_pos);
				cars = GetCarNames(tours[tours_pos] + ExtName::tour);
				RemoveExtension(cars, ExtName::car);
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
			Text::TextInfo text_info = { cars, cars_pos, starting_local_pos, TextAlign::left, 0, true };
			MultithreadingData multithreading_data = { &mutex, &timer_running };
			cars_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
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
			Text::TextInfo text_info = { tires, tires_pos, starting_local_pos, TextAlign::left, 0, true };
			MultithreadingData multithreading_data = { &mutex, &timer_running };
			tires_pos = Text::Choose::Horizontal(text_info, *main_window->GetWindowInfo(), multithreading_data);
			if (i != tires_pos)
			{
				ShowTiresParameters(tires[i] + ExtName::tire, true, tire_box_starting_pos);
				ShowTiresParameters(tires[tires_pos] + ExtName::tire, false, tire_box_starting_pos);
			}
			break;
		}
		case 9://Back
		{
			SaveWindowConfig(main_window->GetToTConfig(), *main_window->main_color, *main_window->secondary_color);
			show_clients = false;
			show_clients_in_lobby.join();
			network_connector->CloseAllConnections();
			return false;
		}
		}
		if (main_menu_position == 8)//Next
		{
			break;
		}
	}
	for (int i = 0; i < static_cast<int>(LanguagePack::text[LanguagePack::game_options_multiplayer].size()); ++i)
	{
		mutex.lock();
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>(static_cast<float>(TextAlign::center) / 2.0f * static_cast<float>(LanguagePack::text[LanguagePack::game_options_multiplayer][i].size())), starting_point.Y + static_cast<short>(i * spacing) });
		std::cout << Spaces(static_cast<int>(LanguagePack::text[LanguagePack::game_options_multiplayer][i].size()));
		mutex.unlock();
	}
	SaveWindowConfig(main_window->GetToTConfig(), *main_window->main_color, *main_window->secondary_color);
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
		COORD coord = { 1,1 };
		timer_multithreading_data = { &mutex, &timer_working, std::chrono::milliseconds(50) };
		timer = std::make_unique<VisibleTimer>(coord, main_window->GetWindowInfo(), &timer_multithreading_data);
		auto seconds_per_turn = std::chrono::seconds(main_window->GetTimerSettings() * 10);
		timer_working = true;
		timer->SetTimer(seconds_per_turn, &timer_running);
		timer->StartShowingTimer();
	}
	SortLeaderboard();
	network_connector->broadcast_sender.Stop();
	network_connector->client_connector.Stop();
	show_clients = false;
	show_clients_in_lobby.join();
	return true;
}
void Host::Finish()
{
	SinglePlayer::Finish();
	network_connector->CloseAllConnections();
}
