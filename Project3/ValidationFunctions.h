#pragma once
#include "FileManagementFunctions.h"
#include "ValidationConstants.h"

namespace Validation
{
	bool ValidateGameFiles();
	bool ValidateTourFiles();
	bool ValidateCarFiles();
	bool ValidateTireFiles();
	bool ValidateRanking();
	bool ValidateLanguagePack();
}