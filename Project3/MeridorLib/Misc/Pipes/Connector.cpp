#include "Connector.h"

namespace MeridorPipes
{
	bool Connector::SetPipes() noexcept
	{
		SECURITY_ATTRIBUTES sats{};
		sats.nLength = sizeof(SECURITY_ATTRIBUTES);
		sats.bInheritHandle = 1;
		sats.lpSecurityDescriptor = 0;

		return 
			CreatePipe(&output_pipe_read, &output_pipe_write, &sats, 0) &&
			CreatePipe(&input_pipe_read, &input_pipe_write, &sats, 0);
	}
	bool Connector::SetProcess(const char* path) noexcept
	{
		STARTUPINFOA sti{};
		sti.dwFlags = STARTF_USESTDHANDLES;
		sti.hStdInput = input_pipe_read;
		sti.hStdOutput = output_pipe_write;
		sti.hStdError = output_pipe_write;

		return CreateProcessA(0, (CHAR*)path, 0, 0, 1, 0, 0, 0, &sti, &process_info);
	}
	bool Connector::Connect(const char* path) noexcept
	{
		return SetPipes() * SetProcess(path);
	}
	std::string Connector::Read() const noexcept
	{
		char buffer[buffer_size]{};
		DWORD read;
		std::string ret;

		PeekNamedPipe(output_pipe_read, buffer, buffer_size, &read, 0, 0);
		do
		{
			if (!ReadFile(output_pipe_read, buffer, buffer_size, &read, 0) || !read)
			{
				break;
			}
			ret += buffer;
		} while (read >= buffer_size);

		return ret;
	}
	void Connector::Write(std::string msg) const noexcept
	{
		WriteFile(input_pipe_write, (msg + '\n').c_str(), static_cast<DWORD>(msg.size() + 1), 0, 0);
	}
	void Connector::CloseConnection() noexcept
	{
		if (!is_closed)
		{
			output_pipe_read&& CloseHandle(output_pipe_read);
			output_pipe_write&& CloseHandle(output_pipe_write);
			input_pipe_write&& CloseHandle(input_pipe_write);
			input_pipe_read&& CloseHandle(input_pipe_read);
			TerminateProcess(process_info.hProcess, 0);
			process_info.hProcess&& CloseHandle(process_info.hProcess);
			process_info.hThread&& CloseHandle(process_info.hThread);
			is_closed = false;
		}
	}
}