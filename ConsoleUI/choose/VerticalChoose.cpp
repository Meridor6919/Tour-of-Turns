#include "VerticalChoose.h"

namespace ConsoleUI {
	VerticalChoose::VerticalChoose(std::vector<std::string>& data) : 
		BaseChoose(data) 
	{}

	VerticalChoose::VerticalChoose(std::vector<std::string>& data, const Desc& desc) : 
		BaseChoose(data, desc) 
	{}

	VerticalChoose::VerticalChoose(std::vector<std::string>& data, Desc&& desc) : 
		BaseChoose(data, desc) 
	{}
	
	int VerticalChoose::Choose() {
		return 0;
	}
}