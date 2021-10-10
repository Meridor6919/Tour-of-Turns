#pragma once
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace MeridorPipes
{
	constexpr size_t buffer_size = 4096;

	class Connector
	{
		PROCESS_INFORMATION process_info;
		HANDLE output_pipe_write;
		HANDLE output_pipe_read;
		HANDLE input_pipe_write;
		HANDLE input_pipe_read;
		bool is_closed = true;

		bool SetPipes() noexcept;
		bool SetProcess(const char* path) noexcept;

	public:
		bool Connect(const char* path) noexcept;
		[[nodiscard]] std::string Read() const noexcept;
		void Write(std::string msg) const noexcept;
		void CloseConnection() noexcept;
	};
}