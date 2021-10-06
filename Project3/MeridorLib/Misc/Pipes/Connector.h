#pragma once
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

#include "Constants.h"

namespace MeridorPipes
{
	class Connector
	{
		HANDLE output_pipe_write;
		HANDLE output_pipe_read;
		HANDLE input_pipe_write;
		HANDLE input_pipe_read;

		PROCESS_INFORMATION process_info;

		bool SetPipes();
		bool SetProcess(const char* path);

	public:
		bool Connect(const char* path);
		std::string Read();
		void Write(std::string msg);
		void CloseConnection();
	};
}