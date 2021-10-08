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
				const WindowInfo* window_info;
				GenericChooseDesc<T>* choose_desc;

				ChooseInterface(GenericChooseDesc<T> &choose_desc, const WindowInfo& window_info) noexcept : choose_desc(&choose_desc), window_info(&window_info) {};

			public:
				[[nodiscard]] bool ClearAfter() const noexcept 
				{ 
					return choose_desc->clear_after; 
				}
				[[nodiscard]] bool ExitOnProcessedInput() const noexcept 
				{ 
					return choose_desc->exit_on_processed_input; 
				}
				[[nodiscard]] bool DontShowInterface() const noexcept 
				{ 
					return choose_desc->dont_show_interface; 
				}

				virtual bool Valid() noexcept = 0;
				virtual void ShowInterface() const noexcept = 0;
				virtual bool ProcessInput(int button) noexcept = 0;
				virtual void UpdateInterface() noexcept = 0;
				virtual void Clear() const noexcept = 0;
				
				[[nodiscard]] T GetErrorValue() const noexcept
				{
					return choose_desc->error_value;
				}
				[[nodiscard]] T GetValue() const noexcept
				{
					return choose_desc->value;
				}
			};
			template<class T>
			[[nodiscard]] T GenericChoose(ChooseInterface<T>& choose) noexcept
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
				return choose.GetErrorValue();
			}
			template<class T>
			[[nodiscard]] T GenericChoose(ChooseInterface<T>& choose, const MultithreadingData& multithreading_data) noexcept
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
						input = Button(multithreading_data.force_break, KeyCodes::enter, multithreading_data.delay);
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
				return choose.GetErrorValue();
			}
		}
	}
}