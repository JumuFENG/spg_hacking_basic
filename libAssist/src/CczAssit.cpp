#include "CczAssit.h"
#include "helper/helper.h"
#include "helper/simpleoutput.h"
#include <WindowsX.h>

bool CCZAssitWrapper::bStop = false;

bool CCZAssitWrapper::startprocess(const tstring& ccz_path, PROCESS_INFORMATION& procInfo)
{
//     ShellExecute(NULL, _T("Open"), ccz_path.c_str(), NULL, NULL, 0);
//     WinExec( util_win::to_string(ccz_path).c_str(), 0);
    
    STARTUPINFO startInfo = {0};
    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.dwFlags = STARTF_USESHOWWINDOW;
    startInfo.wShowWindow = SW_SHOW;
    TCHAR* exePath = new TCHAR[ccz_path.length() + 1];
    _tcscpy_s(exePath, ccz_path.length() + 1, ccz_path.c_str());
    bool bRt;
    if (!CreateProcess(NULL, exePath, NULL, NULL, TRUE, 
        NORMAL_PRIORITY_CLASS, 0, ccz_path.substr(0, ccz_path.rfind(_T("\\"))).c_str(), &startInfo, &procInfo))
    {
        LOG("CreateProcess failed!");
        bRt = false;
    }
    else if (WaitForInputIdle(procInfo.hProcess, 2000) != WAIT_OBJECT_0)
    {
        LOG("WaitForInputIdle error!");
        bRt = false;
    }
    else
    {
        bRt = true;
    }
    if (exePath != NULL)
    {
        delete[] exePath;
        exePath = NULL;
    }
    return bRt;
}

DWORD WINAPI CCZAssitWrapper::AutoClickProc(LPVOID lpParam)
{
    CCZAssitWrapper* pAssit = (CCZAssitWrapper*)lpParam;
    LOG(pAssit->hcczMainWnd);
    while (!bStop)
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(pAssit->hAutoClickEvt, 20))
        {
            //printf("now dont send click\n");
            continue;
        }

        //printf("now send click when needed\n");
        bool bSendClick = true;
        if (pAssit->hcczMainWnd == ::GetForegroundWindow())
        {
            RECT windowRect;
            ::GetWindowRect(pAssit->hcczMainWnd, &windowRect);
            POINT point;
            ::GetCursorPos(&point);
            if (windowRect.top < point.y && point.y < windowRect.top + pAssit->invalidHeight
                && windowRect.left < point.x && point.x < windowRect.right)
            {
                bSendClick = false;
            }
        }
        else
        {
            bSendClick = pAssit->stillClickOnBg;
        }

        if (bSendClick)
        {
            //             ::SendMessage(hWnd, WM_LBUTTONDOWN, 0, MAKELPARAM(300, 300));
            //             ::Sleep(20);
            //             ::SendMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(300, 300));
            //             printf("Send: %d\n", k++);
            ::PostMessage(pAssit->hcczMainWnd, WM_LBUTTONDOWN, 0, MAKELPARAM(300, 300));
            ::Sleep(20);
            ::PostMessage(pAssit->hcczMainWnd, WM_LBUTTONUP, 0, MAKELPARAM(300,300));
            //::PostMessage(hWnd, BM_CLICK, 0, 0);
        }
    }
    return 0;
}

CCZAssitWrapper::CCZAssitWrapper()
    : str_mainClassName(_T("三国志曹操传"))
    , str_mainWndName(_T("三国志曹操传"))
    , hhookKeybdMsg(NULL)
    , hcczMainWnd(NULL)
    , hAutoThread(NULL)
    , hAutoClickEvt(NULL)
    , stillClickOnBg(true)
    , invalidHeight(0)
{
}

CCZAssitWrapper::~CCZAssitWrapper()
{
    bStop = true;
    if (hhookKeybdMsg)
    {
        UnhookWindowsHookEx(hhookKeybdMsg);
        hhookKeybdMsg = NULL;
    }
    if (hAutoThread != NULL)
    {
        WaitForSingleObject(hAutoThread, INFINITE);
        CloseHandle(hAutoThread);
        hAutoThread = NULL;
    }
    if (hAutoClickEvt != NULL)
    {
        CloseHandle(hAutoClickEvt);
        hAutoClickEvt = NULL;
    }

}

BOOL CALLBACK  CCZAssitWrapper::EnumThreadWindowProc(HWND hwnd, LPARAM lParam)
{
    EnumWndBase* ew = (EnumWndBase*)lParam;
    if (ew == NULL || hwnd == NULL)
    {
        return FALSE;
    }
    if (ew->wndselector(hwnd))
    {
        ew->add_find(hwnd);
    }
    return TRUE;
}

class cczEnumWndSelector : public EnumWndBase
{
public:
    bool wndselector(HWND hwnd)
    {
         DWORD wstyle = GetWindowStyle(hwnd);
         if (( wstyle & WS_CHILDWINDOW) || (wstyle & WS_CHILD)
             ||(wstyle & WS_POPUP))
         {
             return false;
         }
         if (!IsWindowVisible(hwnd))
         {
             return false;
         }
         return true;
    }
};

HWND CCZAssitWrapper::get_ccz_mainwnd(const DWORD& tid)
{
    if (hcczMainWnd == NULL)
    {
        cczEnumWndSelector cczsel;
        hcczMainWnd = get_main_window(tid, &cczsel);
    }
    return hcczMainWnd;
}

HWND CCZAssitWrapper::get_main_window(const DWORD& tid, EnumWndBase* ewb)
{
    HWND hMainWnd = NULL;
    int k = 0;
    while (hMainWnd == NULL && k < 100)
    {
        EnumThreadWindows(tid, CCZAssitWrapper::EnumThreadWindowProc, (LPARAM)ewb);
        if (ewb->is_find_single())
        {
            hMainWnd = ewb->get_find_result();
        }
        else if (ewb->get_all_result().size() > 1)
        {
            LOG("find more than one HWND!");
            break;
        }
        Sleep(20);
    }
    LOG(hMainWnd);
    return hMainWnd;
}

void CCZAssitWrapper::autosend_mouseclick(HWND hWnd, int invalidH, bool clickOnBg)
{
    hAutoClickEvt = CreateEvent(NULL, TRUE, FALSE, CCZ_AUTOCLICK_EVENT_NAME);
    if ( NULL == hAutoClickEvt)
    {
        return;
    }
    stillClickOnBg = clickOnBg;
    invalidHeight = invalidH;
    DWORD dwTh;
    hAutoThread = CreateThread(NULL, 0, AutoClickProc, this, 0, &dwTh);
}

void CCZAssitWrapper::hookto_ccz(const tstring& dllFile, const tstring& procname, DWORD tid)
{
    HINSTANCE hinstDLL = LoadLibrary(dllFile.c_str()); //"dlibIPCO.dll"
    HOOKPROC hkprcKeyBd = (HOOKPROC)GetProcAddress(hinstDLL, util_win::to_string(procname).c_str());//"HookWndProc" 

    hhookKeybdMsg = SetWindowsHookEx( 
        WH_KEYBOARD,
        //WH_CALLWNDPROC,
        hkprcKeyBd,
        hinstDLL,
        tid);
}
