#include "helper/helper.h"
#include "helper/simpleoutput.h"
#include "../../libAssist/src/CczAssit.h"

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
    if (hCczMain != NULL)
    {
        SetWindowText(hCczMain, nowStop ? CCZ_MAINWINDOWNAME : CCZ_MAINWINDOWNAME_AUTOCLK);
    }

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
    return CallNextHookEx(NULL, nCode, wParam, lParam); 
} 
