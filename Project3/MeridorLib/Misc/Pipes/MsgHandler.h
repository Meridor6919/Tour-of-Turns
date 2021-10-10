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
		void HandlingFunction(void(T::* MsgHandling)(std::string), T* object) const noexcept
		{
			while (handling_connection)
			{
				std::string msg = connector.Read();
				std::invoke(MsgHandling, object, msg);
			}
		}

	public:
		MsgHandler(const char* path) noexcept;
		void Send(std::string msg) const noexcept;
		void CloseConnection() noexcept;

		template<class T>
		void Init(void(T::* MsgHandling)(std::string), T* object) noexcept
		{
			handling_thread = std::thread(&MsgHandler::HandlingFunction<T>, this, MsgHandling, object);
		}
	};
}

