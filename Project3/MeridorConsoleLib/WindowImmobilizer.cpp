#include "WindowImmobilizer.h"
#include "Window.h"


void MeridorConsoleLib::WindowImmobilizer::ImmobilizingWindow()
{
    RECT prev = { 0, 0, 0, 0 };

    while (thread_active)
    {
        RECT temp;
        GetWindowRect(main_window->window_info.hwnd, &temp);

        const bool stopped_moving = (temp.top == prev.top && temp.left == prev.left);
        const bool not_starting_pos = temp.top != 0 || temp.left != 0;
        const bool not_minimized = !IsIconic(main_window->window_info.hwnd);

        if (not_starting_pos && not_minimized)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            if (!IsIconic(main_window->window_info.hwnd))
            {
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                main_window->SetWindowSize();
                main_window->SetCursor(false);
                prev = { 0, 0, 0, 0 };
            }
        }
        prev = temp;
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
