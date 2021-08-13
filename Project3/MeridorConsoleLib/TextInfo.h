#pragma once
#include <string>
#include <vector>
#include <list>

#include "DataStructures.h"

namespace MeridorConsoleLib
{
	namespace Text
	{
		template<class DataType>
		class RefContainer
		{
			std::vector<DataType>* text_ref;
			std::vector<DataType> text_val;

		public:
			RefContainer() {};
			RefContainer(std::vector<DataType>&& v)
			{
				*this = v;
			}
			RefContainer(std::vector<DataType>& v)
			{
				*this = v;
			}
			RefContainer(std::initializer_list<DataType> il)
			{
				text_val = il;
				text_ref = &text_val;
			};
			RefContainer(const RefContainer& tc)
			{
				this->text_val = std::move(tc.text_val);
				this->text_ref = &(this->text_val);
			}

			std::vector<DataType>& Get() {
				return *text_ref;
			}
			size_t size()
			{
				return text_val.size();
			}
			RefContainer& operator=(std::vector<DataType>&& v)
			{
				text_val = v;
				text_ref = &text_val;
				return *this;
			}
			RefContainer& operator=(std::vector<DataType>& v)
			{
				text_ref = &v;
				return *this;
			}
			RefContainer& operator=(RefContainer& tc)
			{
				text_ref = tc.text_ref;
				return *this;
			}
			DataType& operator[](size_t index)
			{
				return text_val[index];
			};
		};

		template < template< typename > class DataType>
		struct TextInfo
		{
			DataType<std::string> text = {};
			size_t starting_index = 0;
			COORD point_of_reference = { 0,0 };
			TextAlign text_align = TextAlign::left;
			short spacing = 1;
			bool clear_after = false;
		};

		template < template< typename > class DataType>
		struct TableTextInfo
		{
			DataType<std::string> text = {};
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

