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

				int CalculateLenght(long long value);
				COORD GetPosition(int lenght);
				long long InBounds(long long value);
				void ClearInterface(int lenght);

			public:
				ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info);
				bool Valid() override;
				void ShowInterface() override;
				bool ProcessInput(int button) override;
				void UpdateInterface() override;
				void Clear() override;
			};
		}
		long long ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info);
		long long ChooseNumeric(NumericChooseDesc& choose_desc, const WindowInfo& window_info, const MultithreadingData& multithreading_data);
	}
}