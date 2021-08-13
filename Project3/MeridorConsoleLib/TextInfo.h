#pragma once
#include <string>
#include <vector>
#include <list>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		template<class T>
		class RefTextContainer
		{
			std::vector<T>* text_ref;
			std::vector<T> text_val;

		public:
			RefTextContainer() {};
			RefTextContainer(std::vector<T>&& v)
			{
				*this = v;
			}
			RefTextContainer(std::vector<T>& v)
			{
				*this = v;
			}
			RefTextContainer(std::initializer_list<T> il) 
			{
				text_val = il;
				text_ref = &text_val;
			};
			RefTextContainer(const RefTextContainer& tc)
			{
				this->text_val = std::move(tc.text_val);
				this->text_ref = &(this->text_val);
			}

			std::vector<T>& Get() {
				return *text_ref;
			}
			size_t size()
			{
				return text_val.size();
			}
			RefTextContainer& operator=(std::vector<T>&& v)
			{
				text_val = v;
				text_ref = &text_val;
				return *this;
			}
			RefTextContainer& operator=(std::vector<T>& v)
			{
				text_ref = &v;
				return *this;
			}
			RefTextContainer& operator=(RefTextContainer& tc)
			{
				text_ref = tc.text_ref;
				return *this;
			}
			T& operator[](size_t index)
			{
				return text_val[index];
			};
		};

		template < template< typename > class T>
		struct TextInfo
		{
			T<std::string> text = {};
			size_t starting_index = 0;
			COORD point_of_reference = { 0,0 };
			TextAlign text_align = TextAlign::left;
			short spacing = 1;
			bool clear_after = false;
		};

		template < template< typename > class T>
		struct TableTextInfo
		{
			T<std::string> text = {};
			COORD point_of_reference = { 0,0 };
			TextAlign table_alignment = TextAlign::left;
			TextAlign column_alignment = TextAlign::left;
			int number_of_columns = 1;
			int number_of_painted_rows = 0;
			short vertical_spacing = 0;
			short horizontal_spacing = 1;
			bool clear_after = false;
		};
	}
}

