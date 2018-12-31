#include "Race.h"

Race::Race(ToT_Window &window, std::vector<Participant*> *participants)
{
	main_window = &window;
	this->participants = participants;
}
void  Race::Lobby(SinglePlayer *network_role)
{

	std::vector<std::string> options = { "Name", "Number of AIs", "Tours", "Cars", "Tires", "Next" };
	COORD starting_point = { (short)main_window->GetWidth() / 2, 20 };
	const short spacing = 3;
	short main_menu_position = 0;
	const int max_name_size = 14;
	HANDLE handle = main_window->GetHandle();
	
	std::string name = "Racer";
	std::vector<std::string> tours;
	std::vector<std::string> cars;
	std::vector<std::string> tires;
	int tires_pos = 0;
	int cars_pos = 0;
	int tours_pos = 0;
	ais = 0;
	
	network_role->GetTourNames(tours);
	network_role->GetTireNames(tires);

	if (tours.size() == 0)
	{
		MessageBox(0, "Cannot load any map files", "error", 0);
		return;
	}
	else if (tires.size() == 0)
	{
		MessageBox(0, "Cannot load any tire files", "error", 0);
		return;
	}

	tour_path = tours[0];
	std::string tire_path = tires[0];
	std::string car_path;

	network_role->GetCarNames(cars, tour_path);

	if (cars.size() == 0)
		MessageBox(0, "Cannot load any car from selected tour", "error", 0);
	else
		car_path = cars[0];

	//showing params
	//-----------------------------------------------------------------------------------------------------------
	std::vector<std::string> car_parameters;																	//
	std::vector<std::string> tire_parameters;																	//
	std::vector<Text::Atributes> text_atributes;																//
																												//
	for (int i = 0; i < Modifiers::car_modifiers->size(); i++)													//
	{																											//
		text_atributes.push_back(Text::Atributes::color2);														//
		text_atributes.push_back(Text::Atributes::endl);														//
	}																											//
																												//
	auto params1 = network_role->GetCarParameters(car_path);													//
	for (int i = 0; i < params1.size(); i++)																	//
	{																											//
		car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");//atrib names:								//
		car_parameters.push_back(std::to_string(params1[i]));													//
	}																											//
	Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 18, *main_window);				//
																												//
	auto params2 = network_role->GetTireParameters(tire_path);													//
	for (int i = 0; i < params2.size(); i++)																	//
	{																											//
		tire_parameters.push_back(Modifiers::tire_modifiers[i]+ ": ");//atrib names:							//
		tire_parameters.push_back(params2[i]);																	//
	}																											//
	Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 36, *main_window);			//
	//-----------------------------------------------------------------------------------------------------------

	while (main_menu_position != 5 || cars.size() == 0)
	{
		switch (main_menu_position = Text::Choose::Veritcal(options, main_menu_position, starting_point, spacing, Text::TextAlign::center, false, *main_window))
		{
			case 0://choosing name
			{
				char button;
				SetConsoleTextAttribute(handle, main_window->color1);
				std::cout << " < ";
				SetConsoleTextAttribute(handle, main_window->color2);
				std::cout << name;
				SetConsoleTextAttribute(handle, main_window->color1);
				std::cout<< " >";

				SetConsoleTextAttribute(handle, main_window->color2);
				int size = static_cast<int>(name.size());

				do
				{
					button = _getch();

					if (size < max_name_size && ((button >= 48 && button <= 57) || (button >= 65 && button <= 90 && GetKeyState(VK_SHIFT) != 1 && GetKeyState(VK_SHIFT) != 0) || (button >= 97 && button <= 122)))
					{
						name += button;
						SetConsoleTextAttribute(handle, main_window->color2);
						std::cout << "\b\b" << button;
						SetConsoleTextAttribute(handle, main_window->color1);
						std::cout<< " >";
						size++;
					}
					else if (size < max_name_size && button == 32 && size != 0 && name[size - 1] != 32)
					{
						name += ' ';
						SetConsoleTextAttribute(handle, main_window->color2);
						std::cout << "\b\b";
						SetConsoleTextAttribute(handle, main_window->color1);
						std::cout<< " >";
						size++;
					}
					else if (button == 8 && size > 0)
					{
						name.erase(size - 1, 1);
						SetConsoleTextAttribute(handle, main_window->color1);
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

				ais = Text::Choose::Horizontal(text, ais, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window);
				break;
			}
			case 2://choosing tour
			{
				int i = tours_pos;
				tour_path = tours[tours_pos = Text::Choose::Horizontal(tours, tours_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
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
					Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 18, *main_window);
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
					car_path = cars[cars_pos = Text::Choose::Horizontal(cars, cars_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
					car_parameters.clear();
					params1 = network_role->GetCarParameters(car_path);													
					for (int i = 0; i < params1.size(); i++)																	
					{																											
						car_parameters.push_back(Modifiers::car_modifiers[i] + ": ");							
						car_parameters.push_back(std::to_string(params1[i])+"   ");													
					}																											
					Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 18, *main_window);
					break;
				}
			}
			case 4://choosing tires
			{
				tire_path = tires[tires_pos = Text::Choose::Horizontal(tires, tires_pos, { starting_point.X + (short)options[main_menu_position].size() / 2 + spacing, starting_point.Y + main_menu_position * spacing }, Text::TextAlign::left, true, *main_window)];
				tire_parameters.clear();
				params2 = network_role->GetTireParameters(tire_path);													
				for (int i = 0; i < params2.size(); i++)																
				{																										
					tire_parameters.push_back(Modifiers::tire_modifiers[i] + ": ");						
					tire_parameters.push_back(params2[i] + "   ");
				}																										
				Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 36, *main_window);
				break;
			}
		}
	}
	for (short i = 0; i < options.size(); i++)
	{
		SetConsoleCursorPosition(handle, { starting_point.X - static_cast<short>((float)Text::TextAlign::center / 2 * (float)options[i].size()), starting_point.Y + i * (short)spacing });
		for (int j = 0; j < options[i].size(); j++)
			std::cout << " ";
	}

	Text::OrdinaryText(tire_parameters, text_atributes, Text::TextAlign::left, 2, 36, *main_window, true);
	Text::OrdinaryText(car_parameters, text_atributes, Text::TextAlign::left, 2, 18, *main_window, true);

	//AIs

	participants->emplace_back(new Participant(name, car_path, tire_path, *network_role));
	
	network_role->GetOtherParticipants(*participants, ais, tour_path);
}
bool Race::Game()
{
	bool alive = true;
	SinglePlayer* network_role = (*participants)[0]->network_role;
	int visibility = (*participants)[0]->car_modifiers[CarModifiers::visibility];
	std::vector<std::string> tour = (*participants)[0]->network_role->GetTourParameters(tour_path);
	
	Interface();
	Ranking(network_role, false);

	for(int turn = 0; turn < tour.size()+1; turn++) //main game loop
	{
		std::vector<std::string>::const_iterator first = tour.begin() + turn;
		std::vector<std::string>::const_iterator last;

		if (tour.size() < turn + visibility)
			last = tour.begin() + tour.size();
		else
			last = tour.begin() + turn + visibility;

		std::vector<std::string> visible_tour(first, last);
		VisionBox(visible_tour, visibility);

		if(turn > 0)
			network_role->Attack(*participants, static_cast<int>((*participants).size())-static_cast<int>(alive), alive);

		if (alive)
		{
			network_role->TakeAction((*participants)[0]);
			Interface();
		}

		network_role->GetOthersAction(*participants, static_cast<int>((*participants).size()) - 1, tour);
		Ranking(network_role, true);

		if (turn < tour.size())
		{
			if (!network_role->GetCurrentAtribs(*participants, tour[turn]))
			{
				alive = false;
			}
		}
		if (static_cast<int>((*participants).size()) == 0)
			break;

		Ranking(network_role, false);
	}
	return alive;
}
void Race::Ending()
{

	std::fstream fvar;

	int points = (ais + 2 - Ranking((*participants)[0]->network_role, true)) * static_cast<int>(1000.0f / (*participants)[0]->score);
	
	fvar.open((tour_path.substr(0, tour_path.size() - 4) + "rank").c_str(), std::ios::in | std::ios::app);
	
	fvar << (*participants)[0]->name << std::endl;
	fvar << (*participants)[0]->car_path << std::endl;
	fvar << ais << std::endl;
	fvar << (*participants)[0]->score << std::endl;
	fvar << Ranking((*participants)[0]->network_role, true) << std::endl;
	fvar << points << std::endl;
			
	fvar.close();
}
int Race::Ranking(SinglePlayer* network_role, bool clear)
{
	std::vector<std::string> text;
	text.push_back("PLACE");
	text.push_back("RACER");
	text.push_back("SCORE");
	std::vector<std::pair<float, std::string>> ranking_info = network_role->GetRankingInfo(*participants);
	int ret;
	
	for (int i = 0; i < (*participants).size(); i++)
	{
		text.push_back(std::to_string(i + 1));
		text.push_back(ranking_info[i].second);
		text.push_back(std::to_string(static_cast<int>(ranking_info[i].first)));
		if (ranking_info[i].second == (*participants)[0]->name && ranking_info[i].first == (*participants)[0]->score)
			ret = i + 1;
	}
	Text::TableText(text, 1, 3, 3, 16, { static_cast<short>(main_window->GetWidth() - 55), 17 }, *main_window, clear);
	return ret;
}
void Race::Interface()
{
	HANDLE window = main_window->GetHandle();

	std::string string[5] = {
		"You can speed up by (0 to " + std::to_string(static_cast<int>((*participants)[0]->car_modifiers[CarModifiers::max_accelerating])) + ")   ",
		"You can slow down by (0 to " + std::to_string(static_cast<int>((*participants)[0]->car_modifiers[CarModifiers::max_braking])) + ")   ",
		"Hand brake value is - " + std::to_string(static_cast<int>((*participants)[0]->car_modifiers[CarModifiers::hand_brake_value]))+ "   ",
		"You can just wait if you're moving...",
		"Remember only weak ones surrenders" };

	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 16 });
	std::cout << " You're racing as: ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << (*participants)[0]->name;
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 18 });
	std::cout << " Your current speed is: ";
	SetConsoleTextAttribute(window, main_window->color2);

	std::cout << static_cast<int>((*participants)[0]->current_speed) << "     ";
	SetConsoleTextAttribute(window, main_window->color1);
	SetConsoleCursorPosition(window, { 0, 20 });
	std::cout << " Your vehice has ";
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << static_cast<int>((*participants)[0]->current_durability) << " durability   ";

	SetConsoleTextAttribute(window, 8);
	for (int i = 0; i < 5; i++)
	{
		SetConsoleCursorPosition(window, { 2, static_cast<short>(40 + i * 2) });
		std::cout << string[i];
	}
	SetConsoleCursorPosition(window, { 0, 55 });
	SetConsoleTextAttribute(window, main_window->color2);
	std::cout << "                          Infobox                         " << std::endl;
	std::cout << "----------------------------------------------------------";
	SetConsoleCursorPosition(window, { 0, 67 });
	std::cout << "----------------------------------------------------------";

	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 34),48 });
	std::cout << "Action Box";
	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51), 49});
	std::cout << "---------------------------------------------";
	SetConsoleCursorPosition(window, { static_cast<short>(main_window->GetWidth() - 51),67 });
	std::cout << "---------------------------------------------";
}

void Race::VisionBox(std::vector<std::string> visible_tour, int visibility)
{

	HANDLE window = main_window->GetHandle();
	std::string helper;
	std::string Distance[] = { "In front of you: ", "Close to you: ", "At some distance: ", "A little further: ",
								"At a considerable distance: ", "Far ahead: ", "Barely noticeable: " };

	for (int i = 0; i < visibility; i++)
	{
		if (visible_tour.size() == i)
		{
			SetConsoleCursorPosition(window, { 1,static_cast<short>(25 + 2 * i) });
			SetConsoleTextAttribute(window, main_window->color1);
			std::cout << Distance[i];
			SetConsoleTextAttribute(window, main_window->color2);
			std::cout << "META                                              ";
			SetConsoleCursorPosition(window, { 1,static_cast<short>(27 + 2 * i) });
			std::cout << "                                                   ";
			break;
		}

		helper = visible_tour[i];

		SetConsoleTextAttribute(window, main_window->color1);
		SetConsoleCursorPosition(window, { 1,static_cast<short>(25 + 2 * i) });
		std::cout << Distance[i];
		SetConsoleTextAttribute(window, main_window->color2);

		switch (visible_tour[i][0])
		{
		case '0':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on asphalt - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight road, terrain is asphalt" << "                    ";
			break;
		}
		case '1':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on grass - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight grassy road" << "                    ";
			break;
		}
		case '2':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on gravel - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Straight road, gravel" << "                    ";
			break;
		}
		case '3':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on sand - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Linear part of tour, but sandy" << "                    ";
			break;
		}
		case '4':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on mud - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Muddy road, without turn" << "                    ";
			break;
		}
		case '5':
		{
			if (visible_tour[i].size() > 1)
				std::cout << "Turn on ice - safe speed is around " << helper.erase(0, 1) << "          ";
			else
				std::cout << "Slippery road with ice" << "                    ";
			break;
		}
		}
	}
}

