#include "TextLib.cpp"

namespace MeridorConsoleLib
{
	namespace Text
	{
		template int Choose::Horizontal<RefContainer>(TextInfo<RefContainer>&, const WindowInfo&, const MultithreadingData&);
		template int Choose::Vertical<RefContainer>(TextInfo<RefContainer>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalShowGUI<RefContainer>(TextInfo<RefContainer>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalClearGUI<RefContainer>(TextInfo<RefContainer>&, const WindowInfo&, const MultithreadingData&);
		template void OrdinaryText<RefContainer>(TextInfo<RefContainer>&, const WindowInfo& window_info, const MultithreadingData&);
		template void TableText<RefContainer>(TableTextInfo<RefContainer>&, const WindowInfo& window_info, const MultithreadingData&);

		template int Choose::Horizontal<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template int Choose::Vertical<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalShowGUI<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalClearGUI<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template void OrdinaryText<std::vector>(TextInfo<std::vector>&, const WindowInfo& window_info, const MultithreadingData&);
		template void TableText<std::vector>(TableTextInfo<std::vector>&, const WindowInfo& window_info, const MultithreadingData&);
	}
}

