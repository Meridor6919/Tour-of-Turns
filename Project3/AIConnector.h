#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>

class AIConnector
{
	int buffer_size;
	const std::string exit_command = "exit";
	PROCESS_INFORMATION process_info = { 0 };
	HANDLE input_pipe_write;
	HANDLE input_pipe_read;
	HANDLE output_pipe_write;
	HANDLE output_pipe_read;
	std::string connector_path;
	std::unique_ptr<std::thread> connection_thread;
	bool handling_connection;

	template <class T>
	void RecvFunction(void(T::*MsgHandling)(std::string), T* object);

public:

	template <class T>
	bool HandleConnection(void(T::*MsgHandling)(std::string), T* object);
	AIConnector(std::string connector_path, int buffer_size);
	bool Write(std::string msg_send);
	bool Read(std::string &msg_received);
	~AIConnector();
};

template <class T>
bool AIConnector::HandleConnection(void(T::*MsgHandling)(std::string), T* object)
{
	SECURITY_ATTRIBUTES security_atributes = { 0 };
	security_atributes.bInheritHandle = 1;
	
	if (!CreatePipe(&output_pipe_read, &output_pipe_write, &security_atributes, 0))
	{
		MessageBox(0, ("Error  code: " + std::to_string(GetLastError())).c_str(), "Pipe Error", 0);
		return false;
	}
	if(!CreatePipe(&input_pipe_read, &input_pipe_write, &security_atributes, 0))
	{
		MessageBox(0, ("Error  code: " + std::to_string(GetLastError())).c_str(), "Pipe Error", 0);
		return false;
	}

	STARTUPINFO startup_info = { 0 };
	startup_info.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startup_info.hStdInput = input_pipe_read;
	startup_info.hStdOutput = output_pipe_write;

	if (!CreateProcessA(NULL, (LPSTR)connector_path.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info))
	{
		MessageBox(0, ("Error  code: " + std::to_string(GetLastError())).c_str(), "Pipe Error", 0);
		return false;
	}
	handling_connection = true;
	connection_thread = std::make_unique<std::thread>(&AIConnector::RecvFunction<T>, this, MsgHandling, object);
	return true;
}
template <class T>
void  AIConnector::RecvFunction(void(T::*MsgHandling)(std::string), T* object)
{
	while (handling_connection)
	{
		std::string msg_received = "";
		if (!Read(msg_received))
		{
			break;
		}
		else
		{
			std::invoke(MsgHandling, object, msg_received);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}