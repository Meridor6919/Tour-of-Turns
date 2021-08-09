#include "WindowImmobilizer.h"
#include "Window.h"

void MeridorConsoleLib::WindowImmobilizer::ImmobilizingWindow()
{
    RECT base_window_pos;
	GetWindowRect(main_window->window_info.hwnd, &base_window_pos);
	bool last_minimized = false;

    while (thread_active)
    {
        RECT current_window_pos;
        GetWindowRect(main_window->window_info.hwnd, &current_window_pos);

        if ((current_window_pos.top != base_window_pos.top || current_window_pos.left != base_window_pos.left) && (!IsIconic(main_window->window_info.hwnd)))
        {
			if (!last_minimized)
			{
				WINDOWPLACEMENT window_placement = {};
				DWORD number_of_events;
				GetWindowPlacement(main_window->GetHWND(), &window_placement);

				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				main_window->SetWindowSize();
				main_window->SetCursor(main_window->window_info.visible_cursor);

				GetNumberOfConsoleInputEvents(main_window->GetInputHandle(), &number_of_events);

				if (number_of_events > 0)
				{
					INPUT_RECORD buffer[buffer_size];
					ReadConsoleInputA(main_window->GetInputHandle(), buffer, buffer_size, &number_of_events);

					for (int i = 0; i < number_of_events; ++i)
					{
						if (buffer[i].EventType == FOCUS_EVENT && buffer[i].Event.MenuEvent.dwCommandId == 0)
						{
							ShowWindow(main_window->GetHWND(), SW_MINIMIZE);
							last_minimized = true;
						}
					}

				}
            }
			else
			{
				last_minimized = false;
			}
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(refresh_rate));
    }
}
void MeridorConsoleLib::WindowImmobilizer::Init(Window* window)
{
    this->main_window = window;
}
void MeridorConsoleLib::WindowImmobilizer::Start()
{
    if (!thread_active)
    {
        thread_active = true;
        main_thread = std::thread(&WindowImmobilizer::ImmobilizingWindow, this);
    }
}
void MeridorConsoleLib::WindowImmobilizer::Stop()
{
    thread_active = false;
    if (main_thread.joinable())
    {
        main_thread.join();
    }
}
MeridorConsoleLib::WindowImmobilizer::~WindowImmobilizer()
{
    Stop();
}
