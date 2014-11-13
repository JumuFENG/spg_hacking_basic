#include <tchar.h>
#include <Windows.h>
#include "helper/simpleoutput.h"

BOOL WINAPI DllMain(
    HINSTANCE  hModule,
    DWORD  dwReason,
    LPVOID  lpvReserved
    )
{
    if(dwReason == DLL_PROCESS_ATTACH)
    {
        LOG("dll Loaded!");
        return TRUE;
    }
}
