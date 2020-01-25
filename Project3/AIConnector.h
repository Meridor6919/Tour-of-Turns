#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include "Constants.h"

class AIConnector
{
	const std::string start_command = "start";
	const std::string exit_command = "exit";
	PROCESS_INFORMATION process_info = { 0 };
	HANDLE input_pipe_write;
	HANDLE input_pipe_read;
	HANDLE output_pipe_write;
	HANDLE output_pipe_read;
	std::string path;
	std::unique_ptr<std::thread> recv_thread;
	bool handling_connection;

	template <class T>
	void RecvFunction(void(T::*MsgHandling)(std::string), T* object);

public:

	template <class T>
	bool HandleConnection(void(T::*MsgHandling)(std::string), T* object);
	AIConnector(std::string path);
	void Write(std::string msg);
	std::string Read();
	~AIConnector();
};

template <class T>
bool AIConnector::HandleConnection(void(T::*MsgHandling)(std::string), T* object)
{
	SECURITY_ATTRIBUTES security_atributes = { 0 };
	security_atributes.bInheritHandle = 1;

	CreatePipe(&output_pipe_read, &output_pipe_write, &security_atributes, 0);
	CreatePipe(&input_pipe_read, &input_pipe_write, &security_atributes, 0);

	STARTUPINFO startup_info = { 0 };
	startup_info.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startup_info.hStdInput = input_pipe_read;
	startup_info.hStdOutput = output_pipe_write;

	CreateProcessA(NULL, (LPSTR)path.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info);
	Write(start_command);
	handling_connection = true;
	recv_thread = std::make_unique<std::thread>(&AIConnector::RecvFunction<T>, this, MsgHandling, object);
	return true;
}
template <class T>
void  AIConnector::RecvFunction(void(T::*MsgHandling)(std::string), T* object)
{
	unsigned long bytes_read;


	while (handling_connection)
	{
		char buffer[2048] = "";
		if (!ReadFile(output_pipe_read, buffer, sizeof(buffer) - 1, &bytes_read, NULL))
		{
			MessageBox(0, ErrorMsg::pipe_error.c_str(), ErrorTitle::pipe_error.c_str(), 0);
			break;
		}
		else
		{
			std::invoke(MsgHandling, object, buffer);
		}

	}
}