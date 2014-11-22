#include "helper/helper.h"
#include "helper/simpleoutput.h"
#include "../../libAssist/src/CczAssit.h"
#include "IPHook.h"

/**************************************************************** 
  WH_CALLWNDPROC hook procedure 
 ****************************************************************/ 
#define CCZ_MAINWINDOWNAME L"三国志曹操传"
#define CCZ_MAINWINDOWNAME_AUTOCLK L"三国志曹操传 (Auto click)"


LRESULT WINAPI HookWndProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
    if (nCode < 0 )  // do not process message 
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam); 
    }

    // Call an application-defined function that converts a message 
    // constant to a string and copies it to a buffer. 
    HANDLE hAutoClickEvent = OpenEvent(EVENT_ALL_ACCESS, NULL, CCZ_AUTOCLICK_EVENT_NAME);
    static bool nowStop = false;
    static HWND hCczMain = FindWindow(CCZ_MAINWINDOWNAME, CCZ_MAINWINDOWNAME);

    if (nCode == HC_ACTION && (((lParam >> 30) & 0x3) == 3))
    {
        // 只处理按键释放
        static bool bCtrlPressed = false;
        switch (wParam)
        {
        case VK_SPACE:
            {
                if (NULL != hAutoClickEvent)
                {
                    nowStop = !nowStop;
                    if (nowStop)
                    {
                        SetEvent(hAutoClickEvent);

                        LOG("space clicked! Stop!");
                    }
                    else
                    {
                        ResetEvent(hAutoClickEvent);
                        LOG("space clicked! Begin!");
                    }
                }
            }
            break;
        case 'S':
            if (bCtrlPressed)
            {
                bCtrlPressed = false;
                LOG("ctrl + S clicked!");
            }
            break;
        case VK_CONTROL:
            bCtrlPressed = true;
            break;
        default:
            break;
        }
    }
    CloseHandle(hAutoClickEvent);
    if (hCczMain != NULL)
    {
        SetWindowText(hCczMain, nowStop ? CCZ_MAINWINDOWNAME : CCZ_MAINWINDOWNAME_AUTOCLK);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam); 
} 

//Handle of current process
HANDLE g_hProc;

//Backup of orignal code of target api
BYTE g_aBackup[6];
BYTE g_aOpcode[6];

//Critical section, prevent concurrency of calling the monitor
CRITICAL_SECTION g_cs;

//Base address of target API in DWORD
DWORD g_dwApiFunc = (DWORD)GetTickCount;

//Hook the target API
__inline BOOL MonitorBase(void)
{
    // Modify the heading 6 bytes opcode in target API to jmp instruction,
    // the jmp instruction will lead the EIP to our fake function
    ReadProcessMemory(g_hProc, LPVOID(g_dwApiFunc), LPVOID(g_aBackup), sizeof(g_aBackup)/ sizeof(g_aBackup[0]), NULL);
    return WriteProcessMemory(g_hProc, LPVOID(g_dwApiFunc),
        LPVOID(g_aOpcode), sizeof(g_aOpcode) / sizeof(g_aOpcode[0]), NULL);
}

//Unhook the target API
__inline BOOL ReleaseBase(void)
{
    // Restore the heading 6 bytes opcode of target API.
    return WriteProcessMemory(g_hProc, LPVOID(g_dwApiFunc),
        LPVOID(g_aBackup), sizeof(g_aOpcode) / sizeof(g_aOpcode[0]), NULL);
}

//Pre-declare
BOOL UninstallMonitor(void);

//Monitor Function
DWORD WINAPI MonFunc()
{
    //Thread safety
    EnterCriticalSection(&g_cs);

    //Restore the original API before calling it
    ReleaseBase();
    static DWORD lastTick = GetTickCount();
    static DWORD lastReal = lastTick;
    DWORD curTick = GetTickCount();
    DWORD dw = lastTick + 10 * ((curTick - lastReal));
    lastReal = curTick;
    lastTick = dw;
    MonitorBase();

    //You can do anything here, and you can call the UninstallMonitor
    //when you want to leave.
    LOG("MonFunc");
    //Thread safety
    LeaveCriticalSection(&g_cs);
    return dw;
}

//Install Monitor
BOOL InstallMonitor(void)
{
    //Get handle of current process
    g_hProc = GetCurrentProcess();
    LOG("InstallMonitor");
    g_aOpcode[0] = 0xE9; //JMP Procudure
    *(DWORD*)(&g_aOpcode[1]) = (DWORD)MonFunc - g_dwApiFunc - 5;

    InitializeCriticalSection(&g_cs);

    //Start monitor
    return MonitorBase();
}

BOOL UninstallMonitor(void)
{
    //Release monitor
    if (!ReleaseBase())
        return FALSE;

    DeleteCriticalSection(&g_cs);

    CloseHandle(g_hProc);

    //Synchronize to main application, release semaphore to free injector
    HANDLE hSema = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, _T("Global\\InjHack"));
    if (hSema == NULL)
        return FALSE;
    return ReleaseSemaphore(hSema, 1, (LPLONG)g_hProc);
}

