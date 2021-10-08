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
				[[nodiscard]] COORD GetPosition() const noexcept;

			public:
				ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info) noexcept;
				[[nodiscard]] bool Valid() noexcept override;
				void ShowInterface() const noexcept override;
				bool ProcessInput(int button) noexcept override;
				void UpdateInterface() noexcept override;
				void Clear() const noexcept override;
			};
		}
		[[nodiscard]] std::string ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info) noexcept;
		[[nodiscard]] std::string ChooseText(TextChooseDesc& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data) noexcept;
	}
}