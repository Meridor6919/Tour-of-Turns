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
	strcpy_s(buffer, buffer_size, (msg+'\n').c_str());
	const bool result = WriteFile(input_pipe_write, buffer, buffer_size, &bytes_written, NULL);
	if (result)
	{
		MessageBox(0, std::to_string(GetLastError()).c_str(), "Pipe Error", 0);
	}
	delete [] buffer;
	return result;
}
bool AIConnector::Read(std::string &msg_received)
{
	unsigned long bytes_read;
	char *buffer = new char[buffer_size];
	const bool result = ReadFile(output_pipe_read, buffer, buffer_size, &bytes_read, NULL);

	if (result)
	{
		msg_received = buffer;
		msg_received.erase(msg_received.begin() + msg_received.find('\r'), msg_received.end());
	}
	else
	{
		MessageBox(0, std::to_string(GetLastError()).c_str(), "Pipe Error", 0);
	}
	delete[] buffer;
	return result;
}
AIConnector::~AIConnector()
{
	Write(exit_command);
	handling_connection = false;
	connection_thread->join();
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
		CloseHandle(process_info.hProcess);
	}
	if (process_info.hThread != NULL)
	{
		CloseHandle(process_info.hThread);
	}
}
