#pragma once
#include <iostream>
#include <conio.h>
#include <algorithm>

#include "ChooseGeneric.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			class ChooseText : public ChooseInterface<std::string>
			{
			protected:
				TextChooseDesc* choose_desc;
				COORD GetPosition();

			public:
				ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info);
				bool Valid() override;
				void ShowInterface() override;
				bool ProcessInput(int button) override;
				void UpdateInterface() override;
				void Clear() override;
			};
		}
		std::string ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info);
		std::string ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data);
	}
}