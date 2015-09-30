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
    char buffer[1024];
    DWORD dwRead;
    char pipename[10];
    std::map<std::string, std::string> storage;

    char pipeadr[100];
    strcpy( pipeadr, "\\\\.\\pipe\\" );

    printf("Enter pipe name (without '\\\\.\\pipe\\)': ");
    std::cin>>pipename;
    strcat( pipeadr, pipename);

    std::cout<<std::endl;


    hPipe = CreateNamedPipe(TEXT(pipeadr),
                            PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
                            PIPE_WAIT,
                            1,
                            1024 * 16,
                            1024 * 16,
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);
    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
        {
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                /* add terminating zero */
                buffer[dwRead] = '\0';

                /* do something with data in buffer */


                std::istringstream line(buffer);
                std::string command;
                if (line >> command)
                    if (command == "set")
                    {
                        std::string key, value;
                        line >> key >> value;
                        //std::cout<<key<<" "<<value;
                        storage[key] = value;
                        WriteFile(hPipe,
                                  "acknowledged",
                                  13,   // = length of string + terminating '\0' !!!
                                  &dwWritten,
                                  NULL);
                    }
                /*for (auto const& item : storage) {
                    std::string const key = item.first;
                    std::cout<<key;
                    std::string const value = item.second;
                    std::cout<<value;
                    }*/
                    else if(command == "get")
                    {
                        std::string key, value;
                        line >> key >> value;
                        if (storage.find(key) != storage.end())
                            std::string value = storage[key];
                        std::string reply = "found " + value;
                        WriteFile(hPipe,
                                  reply.c_str(),
                                  reply.size()+1,   // = length of string + terminating '\0' !!!
                                  &dwWritten,
                                  NULL);

                    }
                    else if(command == "list")
                    {
                        std::ostringstream reply;
                        for (auto const& item : storage)
                        {
                            std::string const key = item.first;
                            std::cout<<key;

                            reply << key << ' ';

                        }
                        auto buffer = reply.str();
                        WriteFile(hPipe,
                                  buffer.c_str(),
                                  buffer.size()+1,   // = length of string + terminating '\0' !!!
                                  &dwWritten,
                                  NULL);
                    }


                    else if(command == "delete")
                    {/*
                        std::string key, value;
                        line >> key >> value;
                        if(storage.find(key)!=storage.end())
                        {

                        }
                        WriteFile(hPipe,
                                  reply.c_str(),
                                  reply.size()+1,   // = length of string + terminating '\0' !!!
                                  &dwWritten,
                                  NULL);*/

                    }
            }

        }

    }

DisconnectNamedPipe(hPipe);


return 0;

}
