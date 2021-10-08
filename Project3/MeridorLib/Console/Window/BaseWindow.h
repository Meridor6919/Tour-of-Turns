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

				void ImmobilizingWindow() noexcept;

			public:

				void Init(BaseWindow* window) noexcept;
				void Start() noexcept;
				void Stop() noexcept;

				Immobilizer(const Immobilizer&) = delete;
				Immobilizer& operator=(const Immobilizer&) = delete;

				Immobilizer() noexcept = default;
				Immobilizer(Immobilizer&&) noexcept = default;
				Immobilizer& operator=(Immobilizer&&) noexcept = default;
				~Immobilizer();
			};

			Immobilizer window_immobilizer;
			WindowInfo window_info{};
			COORD font_size = minimum_font_size;
			COORD base_character_capacity = default_character_capacity;

			virtual void SetFontSize() noexcept final;
			virtual void SetWindowSize() noexcept final;
			virtual void UpdateBufferSize() noexcept final;
			virtual void UpdateFontInformation() noexcept final;
			virtual void UpdateCharacterCapacity() noexcept final;
			virtual void UpdateWindowInformation() noexcept final;

		protected:
			void Init(const WindowInfo& window_info_ex) noexcept;

		public:

			BaseWindow(const BaseWindow&) = delete;
			BaseWindow& operator=(const BaseWindow&) = delete;

			BaseWindow() noexcept = default;
			BaseWindow(BaseWindow&&) noexcept = default;
			BaseWindow& operator=(BaseWindow&&) noexcept = default;
			~BaseWindow() = default;

			[[nodiscard]] virtual const WindowInfo* GetWindowInfo() const noexcept final;
			[[nodiscard]] virtual int GetCharactersPerRow() const noexcept final;
			[[nodiscard]] virtual int GetCharactersPerColumn() const noexcept final;
			[[nodiscard]] virtual COORD GetFontSize()const noexcept final;
			[[nodiscard]] virtual HANDLE GetOutputHandle()const noexcept final;
			[[nodiscard]] virtual HANDLE GetInputHandle()const noexcept final;
			[[nodiscard]] virtual HWND GetHWND()const noexcept final;
			[[nodiscard]] virtual WindowMode GetWindowMode()const noexcept final;

			virtual void SetCursor(BOOL visible) noexcept final;
			virtual void SetWindowMode(WindowMode window_mode, COORD window_size = minimum_window_size) noexcept final;
		};
	}
}