#include "CczAssit.h"
#include "helper/helper.h"
#include "helper/simpleoutput.h"
#include <WindowsX.h>

bool CCZAssitSdk::bStop = false;

bool CCZAssitSdk::isprocessrunning(const tstring& ccz_path)
{
    tstring procname = ccz_path.substr(ccz_path.rfind(TEXT("\\")) + 1);
    return helper::process::get_process_by_name(procname) != 0;
}

bool CCZAssitSdk::startprocess(const tstring& ccz_path, PROCESS_INFORMATION& procInfo)
{
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

DWORD WINAPI CCZAssitSdk::AutoClickProc(LPVOID lpParam)
{
    CCZAssitSdk* pAssit = (CCZAssitSdk*)lpParam;
    LOG(pAssit->hcczMainWnd);
    while (!bStop)
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(pAssit->hAutoClickEvt, 20))
        {
            continue;
        }

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
            ::PostMessage(pAssit->hcczMainWnd, WM_LBUTTONDOWN, 0, MAKELPARAM(300, 300));
            ::Sleep(20);
            ::PostMessage(pAssit->hcczMainWnd, WM_LBUTTONUP, 0, MAKELPARAM(300,300));
        }
    }
    return 0;
}

CCZAssitSdk::CCZAssitSdk()
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

CCZAssitSdk::~CCZAssitSdk()
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

BOOL CALLBACK  CCZAssitSdk::EnumThreadWindowProc(HWND hwnd, LPARAM lParam)
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

HWND CCZAssitSdk::get_ccz_mainwnd(const DWORD& tid)
{
    if (hcczMainWnd == NULL)
    {
        cczEnumWndSelector cczsel;
        hcczMainWnd = get_main_window(tid, &cczsel);
    }
    return hcczMainWnd;
}

HWND CCZAssitSdk::get_main_window(const DWORD& tid, EnumWndBase* ewb)
{
    HWND hMainWnd = NULL;
    int k = 0;
    while (hMainWnd == NULL && k < 100)
    {
        EnumThreadWindows(tid, CCZAssitSdk::EnumThreadWindowProc, (LPARAM)ewb);
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

void CCZAssitSdk::autosend_mouseclick(HWND hWnd, int invalidH, bool clickOnBg)
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

void CCZAssitSdk::hookto_ccz(const tstring& dllFile, const tstring& procname, DWORD tid)
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

bool CCZAssitWrapper::check_hMemDO()
{
    if (hMemDO == NULL)
    {
        hMemDO = LoadLibrary(TEXT("libMemDO.dll"));
    }
    return hMemDO != NULL;
}

void CCZAssitWrapper::startccz(const string& path)
{
    if (!cczAssist.isprocessrunning(util_win::to_tstring(path)))
    {
        cczAssist.startprocess(util_win::to_tstring(path), cczProcInfo);
    }
}

void CCZAssitWrapper::autoclick()
{
    cczAssist.hookto_ccz(TEXT("libIPCO.dll"),TEXT("HookWndProc"), cczProcInfo.dwThreadId);
    cczAssist.autosend_mouseclick(cczAssist.get_ccz_mainwnd(cczProcInfo.dwThreadId), 90);
}

void CCZAssitWrapper::writetoccz(unsigned long offset, byte* data, size_t len)
{
    if (!check_hMemDO())
    {
        LOG("libMemDO doesn't loaded!");
        return;
    }
    typedef int(*mdo_Mod_MemProc)(const tstring&, unsigned long, byte*, size_t);
    mdo_Mod_MemProc mdo_mod_mem = (mdo_Mod_MemProc)GetProcAddress(hMemDO, "mdo_modify_memory");
    if (mdo_mod_mem != NULL)
    {
        mdo_mod_mem(tstring(_T("Ekd5.exe")), offset, data, len);
        LOG("Write memory done!");
    }
}

CCZWrapperBase* GetAssistWrapperObject()
{
    return new CCZAssitWrapper();
}

void ReleaseAW(CCZWrapperBase* wbObj)
{
    delete wbObj;
}
