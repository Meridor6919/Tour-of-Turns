#include "BaseWindow.h"
namespace MeridorConsoleLib
{
	namespace Window
	{
		void BaseWindow::SetFontSize() noexcept
		{
			const double minimal_font_size1 = static_cast<double>(window_info.window_size.X) / (round(static_cast<double>(window_info.characters_capacity.X) + 1.0) * font_aspect_ratio);
			const double minimal_font_size2 = static_cast<double>(window_info.window_size.Y) / (static_cast<double>(window_info.characters_capacity.Y) + 1.0);
			const double required_font_size = (minimal_font_size1 < minimal_font_size2) ? minimal_font_size1 : minimal_font_size2;

			CONSOLE_FONT_INFOEX console_font_info{};
			console_font_info.cbSize = sizeof(console_font_info);
			wcscpy_s(console_font_info.FaceName, font_name);
			console_font_info.dwFontSize.Y = static_cast<short>(required_font_size);;

			SetCurrentConsoleFontEx(window_info.output_handle, FALSE, &console_font_info);
		}
		void BaseWindow::SetWindowSize() noexcept
		{
			SetWindowLong(window_info.hwnd, GWL_STYLE, GetWindowFlags(window_info.window_mode));
			if (window_info.window_mode != WindowMode::windowed)
			{
				ShowWindow(window_info.hwnd, SW_SHOWMAXIMIZED);
			}
			else
			{
				RECT desktop_rect{};
				GetWindowRect(GetDesktopWindow(), &desktop_rect);
				desktop_rect.left = (desktop_rect.right - window_info.window_size.X) / 2;
				desktop_rect.top = (desktop_rect.bottom - window_info.window_size.Y) / 2;

				RECT window_rect{
					desktop_rect.left,
					desktop_rect.top,
					desktop_rect.left + window_info.window_size.X,
					desktop_rect.top + window_info.window_size.Y
				};

				AdjustWindowRect(&window_rect, GetWindowLong(window_info.hwnd, GWL_STYLE), FALSE);
				SetWindowPos(window_info.hwnd, nullptr, window_rect.left, window_rect.top, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 0);
				ShowWindow(window_info.hwnd, SW_NORMAL);
			}
		}
		void BaseWindow::UpdateBufferSize() noexcept
		{
			CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
			GetConsoleScreenBufferInfo(window_info.output_handle, &screen_buffer_info);
			COORD buffer_size = {
				static_cast<short>(screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left + 1),
				static_cast<short>(screen_buffer_info.srWindow.Bottom - screen_buffer_info.srWindow.Top + 1)
			};
			SetConsoleScreenBufferSize(window_info.output_handle, buffer_size);
		}
		void BaseWindow::UpdateFontInformation() noexcept
		{
			CONSOLE_FONT_INFOEX font_info;
			font_info.cbSize = sizeof(font_info);
			GetCurrentConsoleFontEx(window_info.output_handle, FALSE, &font_info);
			font_size = font_info.dwFontSize;
		}
		void BaseWindow::UpdateCharacterCapacity() noexcept
		{
			CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
			GetConsoleScreenBufferInfo(window_info.output_handle, &screen_buffer_info);
			window_info.characters_capacity = screen_buffer_info.dwSize;
		}
		void BaseWindow::UpdateWindowInformation() noexcept
		{
			SetFontSize();
			SetWindowSize();
			UpdateBufferSize();
			UpdateCharacterCapacity();
			UpdateFontInformation();
			SetCursor(static_cast<BOOL>(window_info.visible_cursor));
		}
		void BaseWindow::Init(const WindowInfo& window_info) noexcept
		{
			this->window_info = window_info;
			this->base_character_capacity = window_info.characters_capacity;

			window_immobilizer.Init(this);
			SetWindowMode(window_info.window_mode, window_info.window_size);
			SetConsoleTitleA(window_info.title);
			SetConsoleEditMode(FALSE);
		}
		int BaseWindow::GetCharactersPerRow() const noexcept
		{
			CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
			GetConsoleScreenBufferInfo(window_info.output_handle, &screen_buffer_info);

			return window_info.characters_capacity.X;
		}
		int BaseWindow::GetCharactersPerColumn() const noexcept
		{
			return window_info.characters_capacity.Y;
		}
		COORD BaseWindow::GetFontSize() const noexcept
		{
			return font_size;
		}
		HANDLE BaseWindow::GetOutputHandle() const noexcept
		{
			return window_info.output_handle;
		}
		HANDLE BaseWindow::GetInputHandle() const noexcept
		{
			return window_info.input_handle;
		}
		HWND BaseWindow::GetHWND() const noexcept
		{
			return window_info.hwnd;
		}
		WindowMode MeridorConsoleLib::Window::BaseWindow::GetWindowMode() const noexcept
		{
			return window_info.window_mode;
		}
		const WindowInfo* MeridorConsoleLib::Window::BaseWindow::GetWindowInfo() const noexcept
		{
			return &window_info;
		}
		void BaseWindow::SetCursor(BOOL visible) noexcept
		{
			MeridorConsoleLib::Window::SetCursor(window_info.output_handle, visible);
		}
		void BaseWindow::SetWindowMode(WindowMode window_mode, COORD window_size) noexcept
		{
			if (window_mode != WindowMode::windowed)
			{
				window_info.window_size = { static_cast<short>(GetSystemMetrics(SM_CXSCREEN)), static_cast<short>(GetSystemMetrics(SM_CYSCREEN)) };
			}
			else
			{
				window_info.window_size = window_size;
			}

			window_info.window_mode = window_mode;
			window_info.characters_capacity = base_character_capacity;
			UpdateWindowInformation();

			if (window_mode != WindowMode::windowed_fullscreen)
			{
				window_immobilizer.Stop();
			}
			else
			{
				window_immobilizer.Start();
			}
		}
	}
}