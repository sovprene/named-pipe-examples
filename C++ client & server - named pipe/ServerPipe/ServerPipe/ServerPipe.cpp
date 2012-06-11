//// SERVER PROGRAM /////

#include "stdafx.h"
#include <iostream>
#include <windows.h>
using namespace std;

void sendData(HANDLE pipe, BOOL result, const wchar_t *data){
		
	wcout << "Sending data to pipe..." << endl;

	// This call blocks until a client process reads all the data
	//const wchar_t *data = L"*** Hello Pipe World ***";	//original
	
	DWORD numBytesWritten = 0;
	result = WriteFile(
		pipe, // handle to our outbound pipe
		data, // data to send
		wcslen(data) * sizeof(wchar_t), // length of data to send (bytes)
		&numBytesWritten, // will store actual amount of data sent
		NULL // not using overlapped IO
	);
	

	if (result) {
		wcout << "Number of bytes sent: " << numBytesWritten << endl;
	} else {
		wcout << "Failed to send data." << endl;
		// look up error code here using GetLastError()
	}
	Sleep(2000);	
}

int main(int argc, const char **argv)
{
	wcout << "Creating an instance of a named pipe..." << endl;

	// Create a pipe to send data
	HANDLE pipe = CreateNamedPipe(
		L"\\\\.\\pipe\\my_pipe", // name of the pipe
		PIPE_ACCESS_OUTBOUND, // 1-way pipe -- send only
		PIPE_TYPE_BYTE, // send data as a byte stream
		1, // only allow 1 instance of this pipe
		0, // no outbound buffer
		0, // no inbound buffer
		0, // use default wait time
		NULL // use default security attributes
	);

	if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
		wcout << "Failed to create outbound pipe instance.";
		// look up error code here using GetLastError()
		system("pause");
		return 1;
	}

	wcout << "Waiting for a client to connect to the pipe..." << endl;

	// This call blocks until a client process connects to the pipe
	BOOL result = ConnectNamedPipe(pipe, NULL);
	if (!result) {
		wcout << "Failed to make connection on named pipe." << endl;
		// look up error code here using GetLastError()
		CloseHandle(pipe); // close the pipe
		system("pause");
		//return 1;
	}

	sendData(pipe,result, L"trap");
	Sleep(5000);
	sendData(pipe, result, L"3.0");
	Sleep(5000);
	sendData(pipe, result, L"1");
	
	// Close the pipe (automatically disconnects client too)
	CloseHandle(pipe);

	wcout << "Done." << endl;
	
	system("pause");
	return 0;
}
