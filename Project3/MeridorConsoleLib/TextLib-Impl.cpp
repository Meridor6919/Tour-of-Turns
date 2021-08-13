#include "TextLib.cpp"

namespace MeridorConsoleLib
{
	namespace Text
	{
		template int Choose::Horizontal<RefTextContainer>(TextInfo<RefTextContainer>&, const WindowInfo&, const MultithreadingData&);
		template int Choose::Veritcal<RefTextContainer>(TextInfo<RefTextContainer>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalShowGUI<RefTextContainer>(TextInfo<RefTextContainer>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalClearGUI<RefTextContainer>(TextInfo<RefTextContainer>&, const WindowInfo&, const MultithreadingData&);
		template void OrdinaryText<RefTextContainer>(TextInfo<RefTextContainer>&, const WindowInfo& window_info, const MultithreadingData&);
		template void TableText<RefTextContainer>(TableTextInfo<RefTextContainer>&, const WindowInfo& window_info, const MultithreadingData&);

		template int Choose::Horizontal<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template int Choose::Veritcal<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalShowGUI<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template void Choose::VerticalClearGUI<std::vector>(TextInfo<std::vector>&, const WindowInfo&, const MultithreadingData&);
		template void OrdinaryText<std::vector>(TextInfo<std::vector>&, const WindowInfo& window_info, const MultithreadingData&);
		template void TableText<std::vector>(TableTextInfo<std::vector>&, const WindowInfo& window_info, const MultithreadingData&);
	}
}

