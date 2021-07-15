#pragma once
#include "FileManagementFunctions.h"
#include "ValidationConstants.h"
#include "ValidationStatus.h"


class ValidationCheck {

	ValidationCheck() {};
	static void WindowInfo(MeridorConsoleLib::WindowInfoEx& window_info, Validation::Status& status);
	static void Music(float& music_volume, Validation::Status& status);
	static void AIModule(std::string& ai_module, Validation::Status& status);
	static void ThemeName(std::string& theme_name, Validation::Status& status);
	static void Language(std::string& lang, Validation::Status& status);

	class FileIntegrity {

		FileIntegrity() {};
	public:
		static void MainDirectory(Validation::Status& status);
		static void GameFiles(Validation::Status& status);
		static void MiscFiles(Validation::Status& status);
	};
	static void InvalidGameFile(const std::string& directory, const std::string& file_name);
	static void Tours(Validation::Status& status);
	static void Cars(Validation::Status& status);
	static void Tires(Validation::Status& status);

public:
	static void ToTWindowConfig(::ToTWindowConfig& window_config, Validation::Status& status);
	static void ToTGameConfig(::ToTGameConfig& game_config, Validation::Status& status);
	static void FileIntegrity(Validation::Status& status);

	static void TitleThemes(Validation::Status& status);
	static void Rankings(Validation::Status& status);
	static void LanguagePacks(Validation::Status& status);

	static void GameFiles(Validation::Status& status);

};