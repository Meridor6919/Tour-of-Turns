#include "../choose/Choose.h"

int main() {
	std::vector<std::string> text{ "option1", "option2", "option3" };
	int result = ConsoleUI::HorizontalChoose(text).Decorate("-- ", " --").Choose();
}