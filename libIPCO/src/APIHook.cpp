#include "helper/helper.h"
#include "helper/simpleoutput.h"
#include "APIHook.h"

ApiHookData  g_hookGetTickCount;
ApiHookData  g_hookTmGetTime;
ApiHookData  g_hookQueryPerforCounter;

bool initApiHookData(ApiHookData* aData, const tstring& moduleName, const std::string& apiName)
{
    if (apiName.empty() || moduleName.empty())
    {
        LOG("ModuleName or API Name is empty!");
        return false;
    }
    aData->m_apiToHook = apiName;
    aData->m_moduleName = moduleName;
    aData->m_hProc = GetCurrentProcess();
    if (aData->m_hProc == NULL)
    {
        LOG("GetCurrentProcess failed!");
        return false;
    }
    aData->m_apiHookAddr = (DWORD)GetProcAddress(GetModuleHandle(
        aData->m_moduleName.c_str()), aData->m_apiToHook.c_str());
    if (aData->m_apiHookAddr == NULL)
    {
        LOG("GetProcAddress Failed!");
        return false;
    }
    
    InitializeCriticalSection(&aData->m_csec);

    return TRUE == ReadProcessMemory(
        aData->m_hProc, LPVOID(aData->m_apiHookAddr), LPVOID(aData->m_aBackup),
        sizeof(aData->m_aBackup) / sizeof(aData->m_aBackup[0]), NULL);
}

bool uninitApiHookData(ApiHookData* aData)
{
    recoverApiEntry(aData);
    DeleteCriticalSection(&aData->m_csec);
    if (aData->m_hProc != NULL)
    {
        CloseHandle(aData->m_hProc);
    }
    return true;
}

bool replaceApiEntry(ApiHookData* aData)
{
    return TRUE == WriteProcessMemory(
        aData->m_hProc, LPVOID(aData->m_apiHookAddr), LPVOID(aData->m_aOpcode),
        sizeof(aData->m_aOpcode) / sizeof(aData->m_aOpcode[0]), NULL);
}

bool recoverApiEntry(ApiHookData* aData)
{
    return TRUE == WriteProcessMemory(
        aData->m_hProc, LPVOID(aData->m_apiHookAddr), LPVOID(aData->m_aBackup), 
        sizeof(aData->m_aOpcode) / sizeof(aData->m_aOpcode[0]), NULL);
}

DWORD WINAPI HookProcGetTickCount()
{
    //Thread safety
    EnterCriticalSection(&g_hookGetTickCount.m_csec);

    //Recover the original API before calling it
    recoverApiEntry(&g_hookGetTickCount);
    static DWORD lastTick = GetTickCount();
    static DWORD lastReal = lastTick;
    DWORD curTick = GetTickCount();
    DWORD dw = lastTick + 10 * ((curTick - lastReal));
    lastReal = curTick;
    lastTick = dw;
    replaceApiEntry(&g_hookGetTickCount);

    //You can do anything here, and you can call the UninstallMonitor
    //when you want to leave.

    //Thread safety
    LeaveCriticalSection(&g_hookGetTickCount.m_csec);
    return dw;
}

typedef DWORD(*tmGetTime)();

DWORD HookProctimeGetTime()
{
    //Thread safety
    EnterCriticalSection(&g_hookTmGetTime.m_csec);

    //Recover the original API before calling it
    recoverApiEntry(&g_hookTmGetTime);
    tmGetTime otmget = (tmGetTime)(FARPROC)g_hookTmGetTime.m_apiHookAddr;
    static DWORD lastTime = otmget();
    static DWORD lastReal = lastTime;
    DWORD curTime = otmget();
    DWORD dw = lastTime + 10 * ((curTime - lastReal));
    lastReal = curTime;
    lastTime = dw;
    replaceApiEntry(&g_hookTmGetTime);

    //You can do anything here, and you can call the UninstallMonitor
    //when you want to leave.

    //Thread safety
    LeaveCriticalSection(&g_hookTmGetTime.m_csec);
    return dw;
}

BOOL WINAPI HookProcQueryPerformanceCounter(
    LARGE_INTEGER *lpPerformanceCount
    )
{
    //Thread safety
    EnterCriticalSection(&g_hookQueryPerforCounter.m_csec);

    //Recover the original API before calling it
    recoverApiEntry(&g_hookQueryPerforCounter);
    LARGE_INTEGER curTime = {0};
    QueryPerformanceCounter(&curTime);
    static long long lastCouter = curTime.QuadPart;
    static long long lastReal = curTime.QuadPart;
    long long curCounter = lastCouter + 10 * (curTime.QuadPart - lastReal);
    lpPerformanceCount->QuadPart = curCounter;
    lastReal = curTime.QuadPart;
    lastCouter = curCounter;
    replaceApiEntry(&g_hookQueryPerforCounter);

    //You can do anything here, and you can call the UninstallMonitor
    //when you want to leave.

    //Thread safety
    LeaveCriticalSection(&g_hookQueryPerforCounter.m_csec);
    return TRUE;
}

bool doAPIHook()
{
    bool rlt = false;
    if (!initApiHookData(&g_hookGetTickCount, TEXT("Kernel32"), "GetTickCount"))
    {
        LOG("API Hook Init GetTickCount Failed!");
        rlt = false;
    }
    else
    {
        g_hookGetTickCount.m_aOpcode[0] = 0xE9; //JMP Procudure
        *(DWORD*)(&(g_hookGetTickCount.m_aOpcode[1])) = 
            (DWORD)HookProcGetTickCount - g_hookGetTickCount.m_apiHookAddr - 5;
        rlt = replaceApiEntry(&g_hookGetTickCount);
        if (!rlt)
        {
            LOG("GetTickCount Hook Failed!");
        }
    }
 
    if (!initApiHookData(&g_hookQueryPerforCounter, TEXT("Kernel32"), "QueryPerformanceCounter"))
    {
        LOG("API Hook Init QueryPerformanceCounter Failed!");
        rlt = false;
    }
    else
    {
        g_hookQueryPerforCounter.m_aOpcode[0] = 0xE9; //JMP Procudure
        *(DWORD*)(&(g_hookQueryPerforCounter.m_aOpcode[1])) = 
            (DWORD)HookProcQueryPerformanceCounter - g_hookQueryPerforCounter.m_apiHookAddr - 5;

        rlt = replaceApiEntry(&g_hookQueryPerforCounter);
        if (!rlt)
        {
            LOG("QueryPerformanceCounter Hook Failed!");
        }
    }

    if (!initApiHookData(&g_hookTmGetTime, TEXT("WINMM"), "timeGetTime"))
    {
        LOG("API Hook Init timeGetTime Failed!");
        rlt = false;
    }
    else
    {
        g_hookTmGetTime.m_aOpcode[0] = 0xE9; //JMP Procudure
        *(DWORD*)(&(g_hookTmGetTime.m_aOpcode[1])) = 
            (DWORD)HookProctimeGetTime - g_hookTmGetTime.m_apiHookAddr - 5;

        rlt = replaceApiEntry(&g_hookTmGetTime);
        if (!rlt)
        {
            LOG("timeGetTime Hook Failed!");
        }
    }

    return rlt;
}

// WINMM!timeGetTime
