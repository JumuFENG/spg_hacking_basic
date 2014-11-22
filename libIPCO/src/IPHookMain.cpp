#include <tchar.h>
#include <Windows.h>
#include "helper/simpleoutput.h"
#include "APIHook.h"

BOOL WINAPI DllMain(
    HINSTANCE  hModule,
    DWORD  dwReason,
    LPVOID  lpvReserved
    )
{
    if(dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
//         LOG("dll Loaded!");
        doAPIHook();
        //SpeedUp();
        LOG("APIHook done!");
        return TRUE;
    }
}
