#pragma once

#include "Constants.h"
#include "DataStructures.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		namespace Internal
		{
			template<class T>
			class ChooseInterface
			{
			protected:
				const TextInfo* window_info;
				GenericChooseDesc<T>* choose_desc;
				ChooseInterface(GenericChooseDesc<T> &choose_desc, const TextInfo& window_info) : choose_desc(&choose_desc), window_info(&window_info) {};
			public:
				bool ClearAfter() { return choose_desc->clear_after; }
				bool ExitOnProcessedInput() { return choose_desc->exit_on_processed_input; }
				bool DontShowInterface() { return choose_desc->dont_show_interface; }

				virtual bool Valid() = 0;
				virtual void ShowInterface() = 0;
				virtual bool ProcessInput(int button) = 0;
				virtual void UpdateInterface() = 0;
				virtual void Clear() = 0;
				
				T GetValue()
				{
					return choose_desc->value;
				}
			};
			template<class T>
			long long GenericChoose(ChooseInterface<T>& choose)
			{
				int input;
				if (choose.Valid())
				{
					if (!choose.DontShowInterface())
					{
						choose.ShowInterface();
					}
					do
					{
						input = Button();
						if (choose.ProcessInput(input))
						{
							choose.UpdateInterface();
							if (choose.ExitOnProcessedInput())
							{
								break;
							}
						}

					} while (input != KeyCodes::enter);
					if (choose.ClearAfter())
					{
						choose.Clear();
					}
					return choose.GetValue();
				}
				return failed;
			}
			template<class T>
			long long GenericChoose(ChooseInterface<T>& choose, const MultithreadingData& multithreading_data)
			{
				int input;
				if (choose.Valid())
				{
					multithreading_data.mutex->lock();
					if (!choose.DontShowInterface())
					{
						choose.ShowInterface();
					}
					do
					{
						multithreading_data.mutex->unlock();
						input = Button(multithreading_data.skip_blocking_functions, KeyCodes::enter, multithreading_data.delay);
						bool index_change = choose.ProcessInput(input);
						multithreading_data.mutex->lock();
						if (index_change)
						{
							choose.UpdateInterface();
							if (choose.ExitOnProcessedInput())
							{
								break;
							}
						}
					} while (input != KeyCodes::enter);
					if (choose.ClearAfter())
					{
						choose.Clear();
					}
					multithreading_data.mutex->unlock();

					return choose.GetValue();
				}
				return failed;
			}
		}
	}
}