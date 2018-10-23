#include "Race.h"

Race::Race(Window &window)
{
	main_window = &window;
}
void  Race::Lobby(SinglePlayer *network_role)
{
	std::vector<std::string> options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	COORD starting_point = { (short)main_window->GetWidth() / 2, 15 };
	const short spacing = 3;
	short main_menu_position = 0;
	const int max_name_size = 14;
	
	std::string name = "Racer";
	std::vector<std::string> tours;
	std::vector<std::string> cars;
	std::vector<std::string> tires;
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;
	int ais_pos = 0;
	
	network_role->GetTourNames(tours);
	network_role->GetTireNames(tires);

	if (tours.size() == 0)
	{
		MessageBoxA(0, "Cannot load any map files", "error", 0);
		return;
	}
	else if (tires.size() == 0)
	{
		MessageBoxA(0, "Cannot load any tire files", "error", 0);
		return;
	}

	tour_path = tours[0];
	std::string tire_path = tires[0];
	std::string car_path;

	network_role->GetCarNames(cars, tour_path);

	if (cars.size() == 0)
		MessageBoxA(0, "Cannot load any car from selected tour", "error", 0);
	else
		car_path = cars[0];
	
	while (main_menu_position != 5 || cars.size() == 0)
	{
		switch (main_menu_position = Text::Choose::Veritcal(options, main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window))
		{
			case 0:
			{
				char button;
				std::cout << " < "<< name<<" >";
				int size = name.size();

				do
				{
					button = _getch();

					if (size < max_name_size && ((button >= 48 && button <= 57) || (button >= 65 && button <= 90 && GetKeyState(VK_SHIFT) != 1 && GetKeyState(VK_SHIFT) != 0) || (button >= 97 && button <= 122)))
					{
						name += button;
						std::cout <<"\b\b"<< button << " >";
						size++;
					}
					else if (size < max_name_size && button == 32 && size != 0 && name[size - 1] != 32)
					{
						name += ' ';
						std::cout << "\b\b" << "  >";
						size++;
					}
					else if (button == 8 && size > 0)
					{
						name.erase(size - 1, 1);
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
			case 1:
			{
				std::vector<std::string> ais;

				for (int i = 0; i <= network_role->Possible_AIs(); i++)
					ais.push_back(std::to_string(i));

				ais_pos = Text::Choose::Horizontal(ais, ais_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
				break;
			}
			case 2:
			{
				int i = tours_pos;
				tour_path = tours[tours_pos = Text::Choose::Horizontal(tours, tours_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
				if (i != tours_pos)
					cars_pos = 0;
				break;
			}
			case 3:
			{
				network_role->GetCarNames(cars, tour_path);
				if (cars.size() == 0)
					MessageBoxA(0, "Cannot load any car from selected tour", "error", 0);
				else
					car_path = cars[cars_pos = Text::Choose::Horizontal(cars, cars_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
				break;
			}
			case 4:
			{
				tire_path = tires[tires_pos = Text::Choose::Horizontal(tires, tires_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
				break;
			}
		}
	}

	participants.emplace_back(new Participant(car_path, tire_path, *network_role));
	network_role->GetOtherParticipants(participants, ais_pos);
	network_role->GetTourParameters(tour_path);
}
void Race::Game()
{
	int current_pos = 0;


	//Players.Interface(color, color2, width * 2);
	//Players.VisionBox(TourArr, CurrentPosition, color, color2);

	//while (static_cast<int>(TourArr.size()) > current_pos) //main game loop
	//{
		//Players.Ranking(RacersScores, RacersNames, AliveAIs, width * 2, color, color2);

		for (int i = 0; i < static_cast<int>(participants.size()); i++)
		{
			if (participants[i]->network_role->id < 2 || i == 0) //single player, ai or current user
				//participants[i].assign_action = participants[i]->network_role->TakeAction();
			if (participants[i]->current_durability <= 0)
			{
				//erase connection
				//erase player from vector
			}
		}
		//tests, scores
		//Players.VisionBox(TourArr, CurrentPosition, color, color2);
		//attacks		

		current_pos++;
	//}
	
}
void Race::Ending()
{
	//save scores
}

//to do
	//network device
	//ai as singleplayer child