#pragma once
#include "BaseChoose.h"

namespace ConsoleUI {
	class HorizontalChoose : public BaseChoose {
	protected:
		struct HorizontalDescPart {
			std::string decorationLeft{ "< " };
			std::string decorationRight{ " >" };
		};

	public:
		struct Desc : 
			public HorizontalDescPart, 
			public BaseChoose::Desc 
		{};

	protected:
		HorizontalDescPart _descEx;

	public:
		HorizontalChoose(std::vector<std::string>& data);
		HorizontalChoose(std::vector<std::string>& data, const Desc& desc);
		//No constructor with rvalue desc, because of Desc extension

		int Choose() override;

		HorizontalChoose& Decorate(std::string decorationLeft, std::string decorationRight);
	};
}