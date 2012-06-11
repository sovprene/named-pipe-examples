///// CLIENT PROGRAM /////

#include "stdafx.h"
#include <iostream>
#include <windows.h>
using namespace std;

int main(int argc, const char **argv)
{
	wcout << "Connecting to pipe..." << endl;

	// Open the named pipe
	// Most of these parameters aren't very relevant for pipes.
	HANDLE pipe = CreateFile(
		L"\\\\.\\pipe\\my_pipe",
		GENERIC_READ, // only need read access
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (pipe == INVALID_HANDLE_VALUE) {
		wcout << "Failed to connect to pipe." << endl;
		// look up error code here using GetLastError()
		system("pause");
		return 1;
	}

	while(1){
	wcout << "Reading data from pipe..." << endl;

	// The read operation will block until there is data to read
	wchar_t buffer[128];	//original
	
	DWORD numBytesRead = 0;
	BOOL result = ReadFile(
		pipe,
		buffer, // the data from the pipe will be put here
		127 * sizeof(wchar_t), // number of bytes allocated
		&numBytesRead, // this will store number of bytes actually read
		NULL // not using overlapped IO
	);

	if (result) {
		buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
		wcout << "Number of bytes read: " << numBytesRead << endl;
		wcout << "Message: " << buffer << endl;

		// Convert wchar_t buffer to a char*
		size_t origsize = wcslen(buffer) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		char nstring[newsize];
		wcstombs_s(&convertedChars, nstring, origsize, buffer, _TRUNCATE);
		strcat_s(nstring, " (char *)");
		cout << nstring << endl;

	} else {
		wcout << "Failed to read data from the pipe." << endl;
	}
	Sleep(1000);
	}
	// Close our pipe handle
	CloseHandle(pipe);

	wcout << "Done." << endl;

	system("pause");
	return 0;
}