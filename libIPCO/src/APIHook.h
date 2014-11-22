#ifndef IPC_HOOK_APIHOOK_HEADER_H_
#define IPC_HOOK_APIHOOK_HEADER_H_
#include "helper/tstring.h"

typedef struct tagApiHookData{
    //Critical section, prevent concurrency of calling the monitor
    CRITICAL_SECTION m_csec;

    //Handle of process to Hook
    HANDLE m_hProc;
    //Backup of orignal code of target api
    BYTE m_aBackup[6];
    BYTE m_aOpcode[6];
    tstring m_moduleName;
    std::string  m_apiToHook;
    DWORD m_apiHookAddr;
}ApiHookData;

bool initApiHookData(ApiHookData* aData, const tstring& moduleName, const std::string& apiName);
bool uninitApiHookData(ApiHookData* aData);
bool replaceApiEntry(ApiHookData* aData);
bool recoverApiEntry(ApiHookData* aData);
bool doAPIHook();

DWORD WINAPI HookProcGetTickCount();
DWORD HookProctimeGetTime();
BOOL WINAPI HookProcQueryPerformanceCounter(
    _Out_  LARGE_INTEGER *lpPerformanceCount
    );

#endif // IPC_HOOK_APIHOOK_HEADER_H_
