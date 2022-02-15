#include "BaseChoose.h"

namespace ConsoleUI {
	BaseChoose::BaseChoose(std::vector<std::string>& data) :
		_data(data),
		_desc(Desc{})
	{}

	BaseChoose::BaseChoose(std::vector<std::string>& data, const Desc& desc) :
		_data(data),
		_desc(desc)
	{}

	BaseChoose::BaseChoose(std::vector<std::string>&data, Desc && desc) :
		_data(data),
		_desc(desc)
	{}

	BaseChoose& BaseChoose::setWrapping(bool isWrapping) {
		_desc.isWrapping = isWrapping;
		return *this;
	}

	BaseChoose& BaseChoose::setClearAfter(bool clearAfter) {
		_desc.clearAfter = clearAfter;
		return *this;
	}

	BaseChoose& BaseChoose::setCloseOnChange(bool closeOnChange) {
		_desc.closeOnChange = closeOnChange;
		return *this;
	}

	BaseChoose& BaseChoose::setAlignment(Align alignment) {
		_desc.alignment = alignment;
		return *this;
	}

	BaseChoose& BaseChoose::setIndexPos(int indexPos) {
		_desc.indexPos = indexPos;
		return *this;
	}

	BaseChoose& BaseChoose::setControls(char prev, char next) {
		_desc.prevKey = prev;
		_desc.nextKey = next;
		return *this;
	}
}