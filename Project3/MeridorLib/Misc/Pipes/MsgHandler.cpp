#include "MsgHandler.h"

namespace MeridorPipes
{
	MsgHandler::MsgHandler(const char* path)
	{
		connector.Connect(path);
	}
	void MsgHandler::Send(std::string msg)
	{
		connector.Write(msg);
	}
	void MsgHandler::CloseConnection()
	{
		connector.CloseConnection();
		handling_connection = false;
		if (handling_thread.joinable())
		{
			handling_thread.join();
		}
	}
}
