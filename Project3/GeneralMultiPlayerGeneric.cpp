#include "GeneralMultiPlayer.h"

bool GeneralMultiPlayer::Recv(SOCKET socket, char * buffer, int length, int flags)
{
	if ((recv(socket, buffer, length, flags) < 0) || (((std::string)buffer).size() > length))
	{
		MessageBox(0, "disconnected", "", 0);
		return false;
	}
	return true;
}