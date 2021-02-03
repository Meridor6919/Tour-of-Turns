#pragma once
#include "FileManagementFunctions.h"
#include "ValidationConstants.h"

namespace Validation
{
	void SanitizeToTGameConfig(ToTGameConfig& game_config);
	void SanitizeToTWindowConfig(ToTWindowConfig& window_config);
	bool ValidateGameFiles();
	bool ValidateTourFiles();
	bool ValidateCarFiles();
	bool ValidateTireFiles();
	bool ValidateRanking();
	bool ValidateLanguagePack();
}