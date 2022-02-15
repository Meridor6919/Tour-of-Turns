#include "HorizontalChoose.h"

namespace ConsoleUI {
	HorizontalChoose::HorizontalChoose(std::vector<std::string>& data) :
		BaseChoose(data),
		_descEx(HorizontalDescPart{}) 
	{}

	HorizontalChoose::HorizontalChoose(std::vector<std::string>& data, const Desc& desc) :
		BaseChoose(data, desc),
		_descEx(desc) 
	{}

	int HorizontalChoose::Choose() {
		return 0;
	}

	HorizontalChoose& HorizontalChoose::Decorate(std::string decorationLeft, std::string decorationRight) {
		_descEx.decorationLeft = decorationLeft;
		_descEx.decorationRight = decorationRight;
		return *this;
	}
}
