#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <sstream>
#include <map>

int main(void)
{
    HANDLE hPipe;
    DWORD dwWritten;
    DWORD dwRead;
    char buffer[1024];


    hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
                       GENERIC_READ | GENERIC_WRITE,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       0,
                       NULL);
    while(hPipe != INVALID_HANDLE_VALUE)
    {
        printf("Enter command: ");

        std::string command;

        getline(std::cin, command);

        WriteFile(hPipe,
                  command.c_str(),
                  command.size(),   // = length of string + terminating '\0' !!!
                  &dwWritten,
                  NULL);

    ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);

    printf(buffer);
     std::cout<<"\n";
    }


    CloseHandle(hPipe);
    return (0);
}
