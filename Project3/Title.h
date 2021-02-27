#pragma once
#include "DataStructures.h"
#include "FileManagementFunctions.h"
#include "ValidationFunctions.h"

class ToT_Window;

class Title
{
	ToT_Window* main_window;
	TitleTheme theme;

public:

	void Init(ToT_Window* main_window);
	void SetTheme(std::string name);
	const TitleTheme& GetTheme();

	void ShowMainPart();
	void ShowDecoration(bool clear);

};