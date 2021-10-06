#include "Connector.h"

namespace MeridorPipes
{
	bool Connector::SetPipes()
	{
		bool good = true;
		SECURITY_ATTRIBUTES sats{};
		sats.nLength = sizeof(SECURITY_ATTRIBUTES);
		sats.bInheritHandle = 1;
		sats.lpSecurityDescriptor = 0;

		good = CreatePipe(&output_pipe_read, &output_pipe_write, &sats, 0);
		good = good && CreatePipe(&input_pipe_read, &input_pipe_write, &sats, 0);
		return good;
	}
	bool Connector::SetProcess(const char* path)
	{
		STARTUPINFOA sti{};
		sti.dwFlags = STARTF_USESTDHANDLES;
		sti.hStdInput = input_pipe_read;
		sti.hStdOutput = output_pipe_write;
		sti.hStdError = output_pipe_write;

		return CreateProcessA(0, (CHAR*)path, 0, 0, 1, 0, 0, 0, &sti, &process_info);
	}
	bool Connector::Connect(const char* path)
	{
		return SetPipes() * SetProcess(path);
	}
	std::string Connector::Read()
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

	void Connector::Write(std::string msg)
	{
		BYTE buffer[buffer_size];
		msg = msg + "\n";
		WriteFile(input_pipe_write, msg.c_str(), msg.size(), 0, 0);
	}
	void Connector::CloseConnection()
	{
		output_pipe_read && CloseHandle(output_pipe_read);
		output_pipe_write && CloseHandle(output_pipe_write);
		input_pipe_write && CloseHandle(input_pipe_write);
		input_pipe_read && CloseHandle(input_pipe_read);
		TerminateProcess(process_info.hProcess, 0);
		process_info.hProcess && CloseHandle(process_info.hProcess);
		process_info.hThread && CloseHandle(process_info.hThread);
	}
}