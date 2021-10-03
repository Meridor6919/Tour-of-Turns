#include "GeneralFunctions.h"
namespace MeridorConsoleLib
{
	namespace Text
	{
		int Button()
		{
			return _getch();
		}
		int Button(const bool* loop, int out_of_loop_return_value, std::chrono::nanoseconds delay)
		{
			while (*loop)
			{
				if (_kbhit())
				{
					return _getch();
				}
				std::this_thread::sleep_for(delay);
			}
			return out_of_loop_return_value;
		}
		float GetTextAlignScalar(MeridorConsoleLib::Text::Align text_align)
		{
			return static_cast<float>(text_align) / 2.0f;;
		}
		void SetColor(HANDLE output_handle, Color color)
		{
			SetConsoleTextAttribute(output_handle, static_cast<WORD>(color));
		}
	}
}