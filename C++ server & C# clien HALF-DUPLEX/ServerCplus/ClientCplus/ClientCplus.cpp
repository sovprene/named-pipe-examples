// ClientCplus.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <tchar.h>
#include <windows.h>
#include <iostream>

using namespace std;

void writeStream(){
	HANDLE pipe = ::CreateNamedPipe(_T("\\\\.\\pipe\\HyperPipe"),
    PIPE_ACCESS_DUPLEX,
    PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
    PIPE_UNLIMITED_INSTANCES,
    4096,
    4096,
    0,
    NULL);

   ConnectNamedPipe(pipe, NULL);
  
   LPTSTR data = _T("Hello\n");
   DWORD bytesWritten = 0;
   WriteFile(pipe, data, _tcslen(data) * sizeof(TCHAR), &bytesWritten, NULL);
   CloseHandle(pipe);
}

void readStream(){
	HANDLE pipe = ::CreateNamedPipe(_T("\\\\.\\pipe\\HyperPipe"),
    PIPE_ACCESS_DUPLEX,
    PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
    PIPE_UNLIMITED_INSTANCES,
    4096,
    4096,
    0,
    NULL);

   ConnectNamedPipe(pipe, NULL);
  
   LPTSTR data[128];
   DWORD bytesWritten = 0;
   ReadFile(pipe, data, sizeof(TCHAR), &bytesWritten, NULL);
   CloseHandle(pipe);

   /*
   wchar_t buffer[128];	
   DWORD numBytesRead = 0;
   ReadFile(pipe, buffer, 127 * sizeof(wchar_t), &numBytesRead, NULL);
   buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
   wcout << "Number of bytes read: " << numBytesRead << endl;
   wcout << "Message: " << buffer << endl;
   */
}

int _tmain(int argc, _TCHAR* argv[])
{
	//writeStream();
	readStream();
    
	return 0;
}