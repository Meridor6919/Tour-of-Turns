#include "WindowImmobilizer.h"
#include "Window.h"

void MeridorConsoleLib::WindowImmobilizer::ImmobilizingWindow()
{
    RECT base_winpos;
	GetWindowRect(main_window->window_info.hwnd, &base_winpos);

    while (thread_active)
    {
        RECT current_winpos;
        GetWindowRect(main_window->window_info.hwnd, &current_winpos);
        
        const bool not_starting_pos = current_winpos.top != base_winpos.top || current_winpos.left != base_winpos.left;
        const bool not_minimized = !IsIconic(main_window->window_info.hwnd);
		
        if (not_starting_pos && not_minimized)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            if (!IsIconic(main_window->window_info.hwnd))
            {
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                main_window->SetWindowSize();
                main_window->SetCursor(main_window->window_info.visible_cursor);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
