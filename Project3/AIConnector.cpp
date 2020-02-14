#include "AIConnector.h"

AIConnector::AIConnector(std::string connector_path, int buffer_size)
{
	this->connector_path = connector_path;
	this->buffer_size = buffer_size;
}
bool AIConnector::Write(std::string msg)
{
	unsigned long bytes_written;
	char *buffer = new char[buffer_size];
	strcpy_s(buffer, buffer_size, (msg + '\n').c_str());
	const bool result = WriteFile(input_pipe_write, buffer, static_cast<int>(msg.size()) + 1, &bytes_written, NULL);
	if (!result)
	{
		MessageBox(0, ("Error  code: " + std::to_string(GetLastError())).c_str(), "Pipe Error", 0);
	}
	delete[] buffer;
	return result;
}
bool AIConnector::Read(std::string &msg_received)
{
	unsigned long bytes_read;
	char *buffer = new char[buffer_size];
	bool result = true;
	DWORD bytes_available;
	PeekNamedPipe(output_pipe_read, 0, 0, 0, &bytes_available, 0);
	if (bytes_available)
	{
		result = ReadFile(output_pipe_read, buffer, buffer_size, &bytes_read, NULL);
		if (result)
		{
			msg_received = buffer;
			msg_received = msg_received.substr(0, bytes_read);
		}
		else
		{
			MessageBox(0, ("Error  code: " + std::to_string(GetLastError())).c_str(), "Pipe Error", 0);
		}
	}
	delete[] buffer;
	return result;
}
AIConnector::~AIConnector()
{
	Write(exit_command);
	handling_connection = false;
	if (connection_thread != nullptr && connection_thread->joinable())
	{
		connection_thread->join();
	}
	if (input_pipe_write != NULL)
	{
		CloseHandle(input_pipe_write);
	}
	if (input_pipe_read != NULL)
	{
		CloseHandle(input_pipe_read);
	}
	if (output_pipe_write != NULL)
	{
		CloseHandle(output_pipe_write);
	}
	if (output_pipe_read != NULL)
	{
		CloseHandle(output_pipe_read);
	}
	if (process_info.hProcess != NULL)
	{
		if (WaitForSingleObject(process_info.hProcess, 200) == WAIT_TIMEOUT)
		{
			TerminateProcess(process_info.hProcess, 0);
		}
		CloseHandle(process_info.hProcess);
	}
	if (process_info.hThread != NULL)
	{
		CloseHandle(process_info.hThread);
	}
}
