#pragma once
#include <thread>
#include "Connector.h"

namespace MeridorPipes
{
	class MsgHandler
	{
		Connector connector;
		std::thread handling_thread;
		bool handling_connection = true;

		template<class T>
		void HandlingFunction(void(T::* MsgHandling)(std::string), T* object)
		{
			while (handling_connection)
			{
				std::string msg = connector.Read();
				if (msg.size() >= 2)
				{
					std::invoke(MsgHandling, object, msg);
				}
			}
		}

	public:
		MsgHandler(const char* path);
		void Send(std::string msg);
		void CloseConnection();

		template<class T>
		void Init(void(T::* MsgHandling)(std::string), T* object)
		{
			handling_thread = std::thread(&MsgHandler::HandlingFunction<T>, this, MsgHandling, object);
		}
	};
}

