#pragma once
#include "DataStructures.h"
#include "WindowImmobilizer.h"

namespace MeridorConsoleLib
{
	class Window
	{
	protected:

		WindowInfoEx window_info;
		WindowImmobilizer window_immobilizer;
		COORD font_size;
		COORD base_character_capacity;

		void AdjustFontSize();
		void SetWindowSize();
		COORD GetMetricsWithSelectedFontSize();
		void SetBufferSize();
		void UpdateWindowInformation();

		Window() {};
		void Init(const WindowInfoEx& window_info_ex);
	public:

		Color* main_color;
		Color* secondary_color;

		void BlockingSleep(const int miliseconds);

		const WindowInfo* GetWindowInfo();
		const WindowInfoEx* GetWindowInfoEx();
		int GetCharactersPerRow();
		int GetCharactersPerColumn();
		COORD GetFontSize();
		HANDLE GetOutputHandle();
		HANDLE GetInputHandle();
		HWND GetHWND();
		WindowMode GetWindowMode();

		void SetConsoleEditMode(bool enable);
		void SetCursor(const bool visible);
		void SetWindowMode(WindowMode window_mode, COORD window_size = MeridorConsoleLib::minimum_window_size);

		friend class WindowImmobilizer;
	};
}