#pragma once
#include <thread>

#include "Functions.h"
#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Window
	{
		class BaseWindow
		{
		private:

			class Immobilizer
			{
				bool thread_active = false;
				BaseWindow* main_window = nullptr;
				std::thread main_thread;

				void ImmobilizingWindow();

			public:

				void Init(BaseWindow* window);
				void Start();
				void Stop();

				Immobilizer(const Immobilizer&) = delete;
				Immobilizer& operator=(const Immobilizer&) = delete;

				Immobilizer() = default;
				Immobilizer(Immobilizer&&) = default;
				Immobilizer& operator=(Immobilizer&&) = default;
				~Immobilizer();
			};

			Immobilizer window_immobilizer;
			WindowInfo window_info{};
			COORD font_size = minimum_font_size;
			COORD base_character_capacity = default_character_capacity;

			virtual void SetFontSize() final;
			virtual void SetWindowSize() final;
			virtual void UpdateBufferSize() final;
			virtual void UpdateFontInformation() final;
			virtual void UpdateCharacterCapacity() final;
			virtual void UpdateWindowInformation() final;

		protected:
			void Init(const WindowInfo& window_info_ex);

		public:

			BaseWindow(const BaseWindow&) = delete;
			BaseWindow& operator=(const BaseWindow&) = delete;

			BaseWindow() = default;
			BaseWindow(BaseWindow&&) = default;
			BaseWindow& operator=(BaseWindow&&) = default;
			~BaseWindow() = default;

			[[nodiscard]] virtual const WindowInfo* GetWindowInfo() const noexcept final;
			[[nodiscard]] virtual int GetCharactersPerRow() const noexcept final;
			[[nodiscard]] virtual int GetCharactersPerColumn() const noexcept final;
			[[nodiscard]] virtual COORD GetFontSize()const noexcept final;
			[[nodiscard]] virtual HANDLE GetOutputHandle()const noexcept final;
			[[nodiscard]] virtual HANDLE GetInputHandle()const noexcept final;
			[[nodiscard]] virtual HWND GetHWND()const noexcept final;
			[[nodiscard]] virtual WindowMode GetWindowMode()const noexcept final;

			virtual void SetCursor(BOOL visible) final;
			virtual void SetWindowMode(WindowMode window_mode, COORD window_size = minimum_window_size) final;
		};
	}
}