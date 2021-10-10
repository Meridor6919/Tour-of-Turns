#include "MsgHandler.h"

namespace MeridorPipes
{
	MsgHandler::MsgHandler(const char* path) noexcept
	{
		connector.Connect(path);
	}
	void MsgHandler::Send(std::string msg) const noexcept
	{
		connector.Write(msg);
	}
	void MsgHandler::CloseConnection() noexcept
	{
		connector.CloseConnection();
		handling_connection = false;
		if (handling_thread.joinable())
		{
			handling_thread.join();
		}
	}
}
