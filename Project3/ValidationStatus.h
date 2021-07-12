#pragma once

namespace Validation
{
	class Status
	{
		char status = 0x0;

	public:
		enum class Flags : char {
			valid = 0x0,
			repaired = 0x1,
			unplayable = 0x2,
			no_music = 0x4,
			no_ranking = 0x8,
			corrupted = 0x10,
			last
		};
		void SetFlag(Flags flag);
		void UnsetFlag(Flags flag);
		bool IsFlagActive(Flags flag);
	};
}


