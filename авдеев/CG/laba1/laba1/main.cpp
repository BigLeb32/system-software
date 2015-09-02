#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

TCHAR* envVarStrings[] =
{
    TEXT("OS         = %OS%"),
    TEXT("PATH       = %PATH%"),
    TEXT("HOMEPATH   = %HOMEPATH%"),
    TEXT("TEMP       = %TEMP%")
};
#define  ENV_VAR_STRING_COUNT  (sizeof(envVarStrings)/sizeof(TCHAR*))
#define INFO_BUFFER_SIZE 32767

#define MAX_VALUE_NAME 16383


void printError( TCHAR* msg );

int main(void)
{

    DWORD i;
    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;
    OSVERSIONINFO lpVerInfo;
    lpVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&lpVerInfo);

    int n = lpVerInfo.dwMajorVersion;
    int m = lpVerInfo.dwMinorVersion;

    if(n == 5 && m == 1)
    {
        printf("Name of OS: WINDOWS XP\n");
    }       // Windows XP

    // Get and display the system directory.
    /* if( !GetSystemDirectory( infoBuf, INFO_BUFFER_SIZE ) )
         printError( TEXT("GetSystemDirectory") ); */
    GetSystemDirectory( infoBuf, INFO_BUFFER_SIZE );
    _tprintf( TEXT("System Directory:   %s"), infoBuf );

    /*if( !GetComputerName( infoBuf, &bufCharCount ) )
    printError( TEXT("GetComputerName") ); */

    GetComputerName( infoBuf, &bufCharCount );
    _tprintf( TEXT("\nComputer name:      %s"), infoBuf );

    /*if( !GetUserName( infoBuf, &bufCharCount ) )
    printError( TEXT("GetUserName") ); */

    GetUserName( infoBuf, &bufCharCount );
    _tprintf( TEXT("\nUser name:          %s"), infoBuf );

    char name[MAX_PATH];
    char Names[MAX_PATH];
    DWORD CharCount = MAX_PATH+1;
    HANDLE h = FindFirstVolume(name, sizeof(name));

    ULARGE_INTEGER lpFreeBytesAvailable;
    ULARGE_INTEGER lpTotalNumberOfBytes;
    ULARGE_INTEGER lpTotalNumberOfFreeBytes;

    do
    {
        printf("\nNAME = %d\n",name);
        GetVolumePathNamesForVolumeName(name,Names,CharCount,&CharCount);
        std::cout << "PATH: "<<Names << std::endl;
        GetDiskFreeSpaceEx(name,&lpFreeBytesAvailable, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes);
        std::cout << "Space:"<<lpTotalNumberOfBytes.QuadPart << std::endl;
        std::cout << "Free space:"<<lpTotalNumberOfFreeBytes.QuadPart << std::endl;
        BOOL b = FindNextVolume(h, name, sizeof(name));
        if (!b) break;
    }
    while(true);



    LONG lResult;
    HKEY hKey;

    lResult = RegOpenKeyEx (HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);


    DWORD  retCode, cValues;

    TCHAR  achValue[MAX_VALUE_NAME];
    DWORD cchValue = MAX_VALUE_NAME;

    for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++)
    {
        cchValue = MAX_VALUE_NAME;
        achValue[0] = '\0';
        retCode = RegEnumValue(hKey, i,
                               achValue,
                               &cchValue,
                               NULL,
                               NULL,
                               NULL,
                               NULL);

        if (retCode == ERROR_SUCCESS )
        {
            _tprintf(TEXT("(%d) %s\n"), i+1, achValue);
        }
    }


    LARGE_INTEGER Start;
    LARGE_INTEGER QPT;
    LARGE_INTEGER End;

    QueryPerformanceCounter(&Start);

    QueryPerformanceFrequency(&QPT);



    QueryPerformanceCounter(&End);

    double TimeTraf = double(End.QuadPart - Start.QuadPart)/QPT.QuadPart;

    std::cout<<TimeTraf<<std::endl;

    exit(0);
}
