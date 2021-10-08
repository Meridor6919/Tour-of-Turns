#include "BaseWindow.h"

namespace MeridorConsoleLib
{
	namespace Window
	{
		void BaseWindow::Immobilizer::ImmobilizingWindow() noexcept
		{
			RECT base_window_pos;
			GetWindowRect(main_window->window_info.hwnd, &base_window_pos);
			bool last_minimized = false;

			while (thread_active)
			{
				RECT current_window_pos;
				GetWindowRect(main_window->window_info.hwnd, &current_window_pos);

				const bool window_moved = (current_window_pos.top != base_window_pos.top || current_window_pos.left != base_window_pos.left);
				const bool window_minimized = IsIconic(main_window->window_info.hwnd) != 0;

				if (window_moved && !window_minimized)
				{
					if (!last_minimized)
					{
						mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						main_window->SetWindowSize();
						main_window->SetCursor(static_cast<BOOL>(main_window->window_info.visible_cursor));

						if (IsMinimizeButtonPressed(main_window->GetInputHandle()))
						{
							ShowWindow(main_window->GetHWND(), SW_MINIMIZE);
							last_minimized = true;
						}
					}
					else
					{
						last_minimized = false;
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(window_immobilizer_refresh_rate));
			}
		}
		void BaseWindow::Immobilizer::Init(BaseWindow* window) noexcept
		{
			this->main_window = window;
		}
		void BaseWindow::Immobilizer::Start() noexcept
		{
			if (!thread_active)
			{
				thread_active = true;
				main_thread = std::thread(&BaseWindow::Immobilizer::ImmobilizingWindow, this);
			}
		}
		void BaseWindow::Immobilizer::Stop() noexcept
		{
			thread_active = false;
			if (main_thread.joinable())
			{
				main_thread.join();
			}
		}
		BaseWindow::Immobilizer::~Immobilizer()
		{
			Stop();
		}
	}
}