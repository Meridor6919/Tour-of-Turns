#include "ToT.h"

void Credits(ToT_Window &main_window)
{
	HANDLE handle = main_window.GetHandle();

	std::vector<std::string> credits = {
		"Hello I am Michal Balicki and I am really happy that you are playing my game.",
		"Tour of Turns was my first real project in my programming way and I hope you like it.",
		"I would like to thank people who helped me with this project.",
		"Krystian Grzegorzewicz ",
		"without him the game wouldn't be created.",
		"He was patiently listening what problems do I have with ToT and the most important he gave me a main idea of the game.",
		"Marek Loek ",
		"for making an icon image and inspiring me to hard work.",
		"Marcin Marzec ",
		"for giving me inspiration.",
		"I admire how much work he put in Pan Zniszczenia map for HoMM3 based on Diablo 2 universum.",
		"Miroslaw Zelent ",
		"for getting me interested in programming"
	};
	std::vector<Text::OrdinaryText_atributes> credits_atribute = { Text::endl, Text::endl, Text::endl, Text::color2, Text::endl,
												Text::endl, Text::color2, Text::endl, Text::color2, Text::endl,
												Text::endl, Text::color2, Text::endl };

	OrdinaryText(credits, credits_atribute, Text::TextAlign::center, 3, 25, main_window);
	_getch();
	OrdinaryText(credits, credits_atribute, Text::TextAlign::center, 3, 25, main_window, true);
}
void Options(ToT_Window &main_window)
{
	std::vector<std::string> options_text = { "Main Color", "Secondary Color", "Music playing", "Hamachi connection", "Back" };
	int main_menu_position = 0;
	COORD starting_point = { (short)main_window.GetWidth() / 2+1, 25 };
	const short spacing = 3;
	bool loop = true;

	while (loop)
	{
		//text allign is also needed in clearing so beware if changing them
		switch (main_menu_position = Text::Choose::Veritcal(options_text, main_menu_position, starting_point, spacing, Text::TextAlign::center, false, main_window))
		{
			case 0:
			{

				//list of colours from 2 to 17
				std::vector<std::string> local_text = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green",
						"light cyan","light red","light magenta","yellow","white" };
				COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size() / 2 + 3) , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				
				//erasing color that is already used by second colour
				//program crashes when local_text.begin() is equal to local.text.end() -1 so inline if is nessesary. Substracting by 2 to synchronizewith list.
				auto text_to_erase = main_window.color2 < 15 ? local_text.begin() + main_window.color2 - 2 : local_text.end() - 1;
				local_text.erase(text_to_erase);

				//setting new colour
				main_window.color1 = Text::Choose::Horizontal(local_text, main_window.color1 - 2 - (main_window.color1 > main_window.color2), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color1 >= main_window.color2)
					main_window.color1++;
				main_window.Title({ (short)main_window.GetWidth() / 2, 0 }, Text::TextAlign::center);
				break;
			}
			case 1:
			{
				//list of colours from 2 to 17
				std::vector<std::string> local_text = { "green","cyan","red","magenta","brown","light gray","dark gray","blue","light green",
						"light cyan","light red","light magenta","yellow","white" };
				COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size() / 2 + 3) , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				
				//erasing color that is already used by second colour
				//program crashes when local_text.begin() is equal to local.text.end() -1 so inline if is nessesary. Substracting by 2 to synchronizewith list.
				auto text_to_erase = main_window.color1 < 15 ? local_text.begin() + main_window.color1 - 2 : local_text.end()-1;
				local_text.erase(text_to_erase);

				//setting new colour
				main_window.color2 = Text::Choose::Horizontal(local_text, main_window.color2 - 2 - (main_window.color2 > main_window.color1), local_starting_point, Text::TextAlign::left, true, main_window) + 2;
				if (main_window.color2 >= main_window.color1)
					main_window.color2++;
				main_window.Title({ (short)main_window.GetWidth() / 2, 0 }, Text::TextAlign::center);
				break;
			}
			case 2:
			{
				static bool flag = true;
				std::vector<std::string> local_text = { "on", "off" };
				COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size()/2 +3) , starting_point.Y + static_cast<short>(main_menu_position * spacing) };
				
				main_window.SetMusic("test.wav", !(flag = Text::Choose::Horizontal(local_text, flag, local_starting_point, Text::TextAlign::left, true, main_window)));
				break;
			}
			case 3:
			{
				static bool flag = false;
				std::vector<std::string> local_text = { "on", "off" };
				COORD local_starting_point = { starting_point.X + static_cast<short>(options_text[main_menu_position].size()/2 + 3) , starting_point.Y + static_cast<short>(main_menu_position * spacing) };

				main_window.SetHamachiConnectionFlag(!(flag = Text::Choose::Horizontal(local_text, flag, local_starting_point, Text::TextAlign::left, true, main_window)));
				break;
			}
			case 4:
			{
				HANDLE handle = main_window.GetHandle();

				//clearing options
				for (short i = 0; i < options_text.size(); i++)
				{
					SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::TextAlign::center / 2 * (float)options_text[i].size()), starting_point.Y + i * spacing });
					for (int j = 0; j < options_text[i].size(); j++)
						std::cout << " ";
				}
				loop = false;
			}
		}
	}
}
void Ranking(ToT_Window &main_window)
{
	//TODO ranking should show players accuracy in decision making instead of showing lucky and stupid ones that scores the highest
}
void Game(bool multiplayer, ToT_Window &main_window)
{
	std::shared_ptr<SinglePlayer> network_role;

	if (multiplayer)
	{
		std::vector<std::string> options = { "Host game", "Search game", "Back" };

		switch (Text::Choose::Veritcal(options, 0, { (short)main_window.GetWidth() / 2, 25 }, 3, Text::TextAlign::center, true, main_window))
		{
		case 0:
		{
			try
			{
				network_role = std::make_shared<Host>(main_window);
			}
			catch (int err)
			{
				return;
			}
			break;
		}
		case 1:
		{
			try
			{
				network_role = std::make_shared<Client>(main_window);
			}
			catch (int err)
			{
				return;
			}

			break;
		}
		case 2:
			return;
		}
	}
	else
		network_role = std::make_shared<SinglePlayer>(main_window);
	
	GameLobby(main_window, network_role);
	
	//pre-game_loop interface loading
	network_role->Interface();
	network_role->Ranking(false);

	//main game loop
	for (int turn = 0; network_role->VisionBox(turn); turn++)
	{
		if (turn > 0)	//attacking start with second turn 
			network_role->Attack();

		network_role->TakeAction();
		network_role->GetOthersAction(turn);

		network_role->GetCurrentAtribs();
		network_role->Interface();

		network_role->Ranking(true);
		network_role->Ranking(false);
	}


	_getch();
	_getch();
	network_role->participants[0]->network_role->CloseConnection();
	system("cls");
	main_window.Title({ (short)main_window.GetWidth() / 2, 0 }, Text::TextAlign::center);
}
void GameLobby(ToT_Window &main_window, std::shared_ptr<SinglePlayer> network_role)
{
	//Lobby parameters
	std::vector<std::string> options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	COORD starting_point = { (short)main_window.GetWidth() / 2, 25 };
	const short spacing = 3;
	short main_menu_position = 0;
	const int max_name_size = 14;
	HANDLE handle = main_window.GetHandle();

	//Player related variables
	std::string name = "Racer";
	std::vector<std::string> tours;
	std::vector<std::string> cars;
	std::vector<std::string> tires;
	std::string tour_path;
	std::string tire_path;
	std::string car_path;
	int ais = 0;
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;

	network_role->GetTourNames(tours);
	if (tours.size() == 0)
	{
		MessageBox(0, "Cannot load any map files", "error", 0);
		return;
	}
	network_role->GetTireNames(tires);
	if (tires.size() == 0)
	{
		MessageBox(0, "Cannot load any tire files", "error", 0);
		return;
	}
	network_role->GetCarNames(cars, tours[0]);
	if (cars.size() == 0)
		MessageBox(0, "Cannot load any car from selected tour", "error", 0);
	else
		car_path = cars[0];

	tour_path = tours[0];
	tire_path = tires[0];

	//TODO validating files
	std::vector<std::string> car_parameters;
	std::vector<std::string> tire_parameters;
	std::vector<Text::OrdinaryText_atributes> text_atributes;

	//preparing vectors for OrdinaryText function
	for (int i = 0; i < Modifiers::car_modifiers->size(); i++)
	{
		text_atributes.push_back(Text::OrdinaryText_atributes::color2);
		text_atributes.push_back(Text::OrdinaryText_atributes::endl);
	}

	//showing car parameters
	std::vector<int> params1 = network_role->GetCarParameters(car_path);
	for (int i = 0; i < params1.size(); i++)
	{
		car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");
		car_parameters.push_back(std::to_string(params1[i]));
	}
	Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 22, main_window);

	//showing tire parameters
	auto params2 = network_role->GetTireParameters(tire_path);
	for (int i = 0; i < params2.size(); i++)
	{
		tire_parameters.push_back(Modifiers::tire_modifiers[i] + ": ");//atrib names:							
		tire_parameters.push_back(params2[i]);
	}
	Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 40, main_window);

	//menu segment
	while (main_menu_position != 5 || cars.size() == 0)
	{
		switch (main_menu_position = Text::Choose::Veritcal(options, main_menu_position, starting_point, spacing, Text::TextAlign::center, false, main_window))
		{
		case 0://choosing name
		{
			char button;
			SetConsoleTextAttribute(handle, main_window.color1);
			std::cout << " < ";
			SetConsoleTextAttribute(handle, main_window.color2);
			std::cout << name;
			SetConsoleTextAttribute(handle, main_window.color1);
			std::cout << " >";

			SetConsoleTextAttribute(handle, main_window.color2);
			int size = static_cast<int>(name.size());

			do
			{
				button = _getch();

				if (size < max_name_size && ((button >= 48 && button <= 57) || (button >= 65 && button <= 90 && GetKeyState(VK_SHIFT) != 1 && GetKeyState(VK_SHIFT) != 0) || (button >= 97 && button <= 122)))
				{
					name += button;
					SetConsoleTextAttribute(handle, main_window.color2);
					std::cout << "\b\b" << button;
					SetConsoleTextAttribute(handle, main_window.color1);
					std::cout << " >";
					size++;
				}
				else if (size < max_name_size && button == 32 && size != 0 && name[size - 1] != 32)
				{
					name += ' ';
					SetConsoleTextAttribute(handle, main_window.color2);
					std::cout << "\b\b";
					SetConsoleTextAttribute(handle, main_window.color1);
					std::cout << " >";
					size++;
				}
				else if (button == 8 && size > 0)
				{
					name.erase(size - 1, 1);
					SetConsoleTextAttribute(handle, main_window.color1);
					std::cout << "\b\b\b   \b\b>";
					size--;
				}

			} while (button != 13);
			for (int i = 0; i < size + 4; i++)
				std::cout << "\b";
			for (int i = 0; i < size + 4; i++)
				std::cout << " ";
			if (name.size() == 0)
				name = "Racer";
			break;
		}
		case 1://Number of ais
		{
			std::vector<std::string> text;

			for (int i = 0; i <= network_role->Possible_AIs(); i++)
				text.push_back(std::to_string(i));

			ais = Text::Choose::Horizontal(text, ais, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, main_window);
			break;
		}
		case 2://choosing tour
		{
			int i = tours_pos;
			tour_path = tours[tours_pos = Text::Choose::Horizontal(tours, tours_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, main_window)];
			if (i != tours_pos)
			{
				network_role->GetCarNames(cars, tour_path);
				cars_pos = 0;
				car_path = cars[cars_pos];
				car_parameters.clear();
				params1 = network_role->GetCarParameters(car_path);
				for (int i = 0; i < params1.size(); i++)
				{
					car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");
					car_parameters.push_back(std::to_string(params1[i]) + "   ");
				}
				Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 18, main_window);
				break;
			}
			break;
		}
		case 3://choosing car
		{
			network_role->GetCarNames(cars, tour_path);
			if (cars.size() == 0)
				MessageBoxA(0, "Cannot load any car from selected tour", "error", 0);
			else
			{
				car_path = cars[cars_pos = Text::Choose::Horizontal(cars, cars_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, main_window)];
				car_parameters.clear();
				params1 = network_role->GetCarParameters(car_path);
				for (int i = 0; i < params1.size(); i++)
				{
					car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");
					car_parameters.push_back(std::to_string(params1[i]) + "   ");
				}
				Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 18, main_window);
				break;
			}
		}
		case 4://choosing tires
		{
			tire_path = tires[tires_pos = Text::Choose::Horizontal(tires, tires_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, main_window)];
			tire_parameters.clear();
			params2 = network_role->GetTireParameters(tire_path);
			for (int i = 0; i < params2.size(); i++)
			{
				tire_parameters.push_back(Modifiers::tire_modifiers[i] + ": ");
				tire_parameters.push_back(params2[i] + "   ");
			}
			Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 36, main_window);
			break;
		}
		}
	}
	//clearing menu afterwards
	for (short i = 0; i < options.size(); i++)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::TextAlign::center / 2 * (float)options[i].size()), starting_point.Y + i * (short)spacing });
		for (int j = 0; j < options[i].size(); j++)
			std::cout << " ";
	}

	//clearing params segment
	Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 40, main_window, true);
	Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 22, main_window, true);

	//loading player

	//loading clients/ais
	network_role->GetParticipants(name, ais, tour_path, car_path, tire_path);

}
