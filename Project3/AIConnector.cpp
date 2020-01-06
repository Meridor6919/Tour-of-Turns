#include "AIConnector.h"

AIConnector::AIConnector(std::string path)
{
	this->path = path;
}
void AIConnector::Write(std::string msg)
{
	unsigned long bytes_written;
	msg += "\n";
	WriteFile(input_pipe_write, msg.c_str(), static_cast<DWORD>(msg.size()), &bytes_written, NULL);
}
std::string AIConnector::Read()
{
	std::string ret = "";
	unsigned long bytes_read;
	char buffer[2048] = "";

	ReadFile(output_pipe_read, buffer, sizeof(buffer) - 1, &bytes_read, NULL);
	std::cout << buffer;
	return ret;
}
AIConnector::~AIConnector()
{
	Write(exit_command);
	handling_connection = false;
	recv_thread->join();
	if (input_pipe_write != NULL) CloseHandle(input_pipe_write);
	if (input_pipe_read != NULL) CloseHandle(input_pipe_read);
	if (output_pipe_write != NULL) CloseHandle(output_pipe_write);
	if (output_pipe_read != NULL) CloseHandle(output_pipe_read);
	if (process_info.hProcess != NULL) CloseHandle(process_info.hProcess);
	if (process_info.hThread != NULL) CloseHandle(process_info.hThread);
}
