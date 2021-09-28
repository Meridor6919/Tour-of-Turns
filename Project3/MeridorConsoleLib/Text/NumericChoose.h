#pragma once
#include <iostream>
#include <conio.h>

#include "GenericChoose.h"
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
				COORD CalculatePosition(int lenght);
				long long InBounds(long long value);
				void ClearInterface(int lenght);

			public:
				ChooseNumeric(NumericChooseDesc& choose_desc, const TextInfo& window_info);
				bool Valid() override;
				void ShowInterface() override;
				bool ProcessInput(int button) override;
				void UpdateInterface() override;
				void Clear() override;
			};
		}
		long long ChooseNumeric(NumericChooseDesc& choose_desc, const TextInfo& window_info);
		long long ChooseNumeric(NumericChooseDesc& choose_desc, const TextInfo& window_info, const MultithreadingData& multithreading_data);
	}
}