#pragma once
#include "BaseChoose.h"

namespace ConsoleUI {
	class VerticalChoose : public BaseChoose {
	public:
		using Desc = BaseChoose::Desc;

		VerticalChoose(std::vector<std::string>& data);
		VerticalChoose(std::vector<std::string>& data, const Desc& desc);
		VerticalChoose(std::vector<std::string>& data, Desc&& desc);

		int Choose() override;
	};
}