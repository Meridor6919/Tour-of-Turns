#pragma once
#include "FileManagementFunctions.h"
#include "ValidationConstants.h"

namespace Validation
{
	void ValidateToTGameConfig(ToTGameConfig& game_config);
	void ValidateToTWindowConfig(ToTWindowConfig& window_config);
	bool ValidateGameFiles();
	bool ValidateTourFiles();
	bool ValidateCarFiles();
	bool ValidateTireFiles();
	bool ValidateRanking();
	bool ValidateLanguagePack();
}