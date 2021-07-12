#pragma once
#include "FileManagementFunctions.h"
#include "ValidationConstants.h"


class ValidationCheck {

	ValidationCheck() {};
	static void WindowInfo(MeridorConsoleLib::WindowInfoEx& window_info, char& status);
	static void Music(float& music_volume, char& status);
	static void AIModule(std::string& ai_module, char& status);
	static void ThemeName(std::string & theme_name, char& status);

	class FileIntegrity {

		FileIntegrity() {};
	public:
		static void MainDirectory(char& status);
		static void GameFiles(char& status);
		static void MiscFiles(char& status);
	};
	static void InvalidGameFile(const std::string& directory, const std::string& file_name);
	static void Tours(char& status);
	static void Cars(char& status);
	static void Tires(char& status);

public:
	static void ToTWindowConfig(::ToTWindowConfig& window_config, char& status);
	static void ToTGameConfig(::ToTGameConfig& game_config, char& status);
	static void FileIntegrity(char& status);
	
	static void TitleThemes(char& status);
	static void Rankings(char& status);
	static void LanguagePacks(char& status);

	static void GameFiles(char& status);
	
};