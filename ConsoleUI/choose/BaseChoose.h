#pragma once
#include <vector>
#include <string>

enum class Align {
	left,
	center,
	right
};

namespace ConsoleUI {
	class BaseChoose {
	protected:
		struct Desc {
			int indexPos{ 0 };
			Align alignment{ Align::left };
			char nextKey{ 'a' };
			char prevKey{ 'd' };
			bool closeOnChange{ false };
			bool isWrapping{ false };
			bool clearAfter{ false };
		};

		std::vector<std::string>& _data;
		Desc _desc;

		BaseChoose(std::vector<std::string>& data);
		BaseChoose(std::vector<std::string>& data, const Desc& desc);
		BaseChoose(std::vector<std::string>& data, Desc&& desc);

	public:
		virtual int Choose() = 0;

		BaseChoose& setWrapping(bool isWrapping);
		BaseChoose& setClearAfter(bool clearAfter);
		BaseChoose& setCloseOnChange(bool closeOnChange);
		BaseChoose& setAlignment(Align alignment);
		BaseChoose& setIndexPos(int indexPos);
		BaseChoose& setControls(char prev, char next);
	};
}