#include "helper/helper.h"
#include "helper/simpleoutput.h"
#include "../../libAssist/src/CczAssit.h"
#include "IPHook.h"
#include "APIHook.h"

/**************************************************************** 
  WH_CALLWNDPROC hook procedure 
 ****************************************************************/ 
#define CCZ_MAINWINDOWNAME L"三国志曹操传"
#define CCZ_MAINWINDOWNAME_AUTOCLK L"三国志曹操传 (Auto click)"

// Keyboad Message hook proc
LRESULT WINAPI HookKeybdProc(int nCode, WPARAM wParam, LPARAM lParam) 
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

// Window proc hook
LRESULT WINAPI HookCallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 )  // do not process message 
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam); 
    }
    
    if (nCode == HC_ACTION)
    {
        LPCWPSTRUCT cwps = (LPCWPSTRUCT)lParam;

        switch (cwps->message)
        {
        case WM_COPYDATA:
            {
                PCOPYDATASTRUCT cpdata = (PCOPYDATASTRUCT)cwps->lParam;
                switch (cpdata->dwData)
                {
                case SetHookTime:
                    doAPIHook();
                    LOG("Time Hook Done!");
                    break;
                case ChangeTimeSpeed:
                    ChangeTimeSpeedFunc(*(unsigned long*)cpdata->lpData);
                    LOG("Change Time Speed Done!");
                    LOG((unsigned long)(*(unsigned long*)cpdata->lpData));
                    break;
                }
            }
            break;
        }
//         else if (cwps->message == WM_QUIT)
//         {
//             LOG("Get WM_Quit");
//         }
//         else
//         {
//             //LOG(cwps->message);
//         }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam); 
}
