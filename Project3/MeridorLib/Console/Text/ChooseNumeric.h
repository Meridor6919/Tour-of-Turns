#pragma once
#include <iostream>
#include <conio.h>

#include "ChooseGeneric.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			class ChooseNumeric : public ChooseInterface<long long>
			{
			protected:
				NumericChooseDesc* choose_desc;
				int lenght;
				int prev_lenght;

				[[nodiscard]] int CalculateLenght(long long value) const noexcept;
				[[nodiscard]] COORD GetPosition(int lenght) const noexcept;
				[[nodiscard]] bool InBounds(long long value) const noexcept;
				void ClearInterface(int lenght) const noexcept;

			public:
				ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info) noexcept;
				[[nodiscard]] bool Valid() noexcept override;
				void ShowInterface() const noexcept override;
				bool ProcessInput(int button) noexcept override;
				void UpdateInterface() noexcept override;
				void Clear() const noexcept override;
			};
		}
		[[nodiscard]] long long ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info) noexcept;
		[[nodiscard]] long long ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data) noexcept;
	}
}