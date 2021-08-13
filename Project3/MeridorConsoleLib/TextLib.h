#pragma once
#include <iostream>
#include <conio.h>

#include "TextInfo.h"
#include "GeneralFunctions.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		inline char Button(const bool* loop, std::chrono::milliseconds delay);

		namespace Choose
		{
			template<template<typename> class ContainerType>
			extern int Horizontal(TextInfo<ContainerType>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});

			template<template<typename> class ContainerType>
			extern int Vertical(TextInfo<ContainerType>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});

			template<template<typename> class ContainerType>
			extern void VerticalShowGUI(TextInfo<ContainerType>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});

			template<template<typename> class ContainerType>
			extern void VerticalClearGUI(TextInfo<ContainerType>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});

			inline void Numeric(int* number_return_value, const int max, COORD starting_point, const WindowInfo& window_info, const MultithreadingData& multithreading_data);
		}
		template<template<typename> class ContainerType>
		extern void OrdinaryText(TextInfo<ContainerType>& text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});

		template<template<typename> class ContainerType>
		extern void TableText(TableTextInfo<ContainerType>& table_text_info, const WindowInfo& window_info, const MultithreadingData& multithreading_data = {});
	}
}