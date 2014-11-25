#include "CczAssit.h"
#include "helper/helper.h"
#include "helper/simpleoutput.h"
#include <WindowsX.h>
#include "../../libIPCO/src/IPHook.h"
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
    , hinstDLL(NULL)
    , hhookKeybdMsg(NULL)
    , hhookWndProc(NULL)
    , hcczMainWnd(NULL)
    , hAutoThread(NULL)
    , hAutoClickEvt(NULL)
    , stillClickOnBg(true)
    , invalidHeight(0)
{
}

void CCZAssitSdk::initall()
{
    bStop = false;
}

void CCZAssitSdk::resetall()
{
    bStop = true;
    if (hhookKeybdMsg)
    {
        UnhookWindowsHookEx(hhookKeybdMsg);
        hhookKeybdMsg = NULL;
    }
    if (hhookWndProc)
    {
        UnhookWindowsHookEx(hhookWndProc);
        hhookWndProc = NULL;
    }
    if (hAutoThread != NULL)
    {
        WaitForSingleObject(hAutoThread, INFINITE);
        CloseHandle(hAutoThread);
        hAutoThread = NULL;
    }

    if (hinstDLL != NULL)
    {
        FreeLibrary(hinstDLL);
        hinstDLL = NULL;
    }
    if (hAutoClickEvt != NULL)
    {
        SetEvent(hAutoClickEvt);
        CloseHandle(hAutoClickEvt);
        hAutoClickEvt = NULL;
    }

    hcczMainWnd = NULL;
    stillClickOnBg = true;
    invalidHeight = 0;
}

CCZAssitSdk::~CCZAssitSdk()
{
    resetall();
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
    hAutoClickEvt = OpenEvent(EVENT_ALL_ACCESS, NULL, CCZ_AUTOCLICK_EVENT_NAME);
    if (hAutoClickEvt == NULL && GetLastError() == ERROR_FILE_NOT_FOUND)
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
    else
    {
        ResetEvent(hAutoClickEvt);
    }
}

void CCZAssitSdk::stop_autosend()
{
    if (hAutoClickEvt != NULL)
    {
        SetEvent(hAutoClickEvt);
    }
}

void CCZAssitSdk::hookkeybdto_ccz(const tstring& dllFile, const tstring& procname, DWORD tid)
{
    if (NULL == hinstDLL)
    {
        hinstDLL = LoadLibrary(dllFile.c_str()); //"dlibIPCO.dll"
    }
    HOOKPROC hkprcKeyBd = (HOOKPROC)GetProcAddress(hinstDLL, 
        util_win::to_string(procname).c_str());//"HookKeybdProc" 

    hhookKeybdMsg = SetWindowsHookEx( 
        WH_KEYBOARD,
        //WH_CALLWNDPROC,
        hkprcKeyBd,
        hinstDLL,
        tid);
}

void CCZAssitSdk::hookwndprocto_ccz(const tstring& dllFile, const tstring& procname, DWORD tid)
{
    if (NULL == hinstDLL)
    {
        hinstDLL = LoadLibrary(dllFile.c_str()); //"dlibIPCO.dll"
    }
    HOOKPROC hkprcWndProc = (HOOKPROC)GetProcAddress(hinstDLL, 
        util_win::to_string(procname).c_str());

    hhookWndProc = SetWindowsHookEx( 
        WH_CALLWNDPROC,
        hkprcWndProc,
        hinstDLL,
        tid);
}

bool CCZAssitSdk::sendsettimehook()
{
    if (hcczMainWnd == NULL)
    {
        LOG("MainWnd is NULL");
        return false;
    }
    COPYDATASTRUCT cdstruc = {0};
    cdstruc.dwData = SetHookTime;
    SendMessage(hcczMainWnd, WM_COPYDATA, 0, (LPARAM)(LPVOID)(&cdstruc));
    return true;
}

void CCZAssitSdk::changetimespeed(unsigned long uprate)
{
    if (hcczMainWnd == NULL)
    {
        LOG("MainWnd is NULL");
        return;
    }
    COPYDATASTRUCT cdstruc = {0};
    unsigned long cdUprate = uprate;
    cdstruc.cbData = sizeof(cdUprate);
    cdstruc.dwData = ChangeTimeSpeed;
    cdstruc.lpData = (LPVOID)(&cdUprate);
    SendMessage(hcczMainWnd, WM_COPYDATA, 0, (LPARAM)(LPVOID)(&cdstruc));
    LOG("changetimespeed Done!");
}

void CCZAssitWrapper::reset()
{
    bTimeHooked = false;
    cczProcInfo.hProcess = NULL;
    cczProcInfo.hThread = NULL;
    cczProcInfo.dwProcessId = 0;
    cczProcInfo.dwThreadId = 0;
}

bool CCZAssitWrapper::check_hMemDO()
{
    if (hMemDO == NULL)
    {
        hMemDO = LoadLibrary(TEXT("libMemDO.dll"));
    }
    return hMemDO != NULL;
}

bool CCZAssitWrapper::enableDebugPrivilege()
{
    HANDLE hToken;
    BOOL fOk = FALSE;
    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount=1;
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tp,sizeof(tp), NULL, NULL);
        fOk = (GetLastError() == ERROR_SUCCESS);
        CloseHandle(hToken);
    }
    return fOk == TRUE;
}

void CCZAssitWrapper::startccz(const string& path)
{
    if (!cczAssist.isprocessrunning(util_win::to_tstring(path)))
    {
        cczAssist.resetall();
        cczAssist.initall();
        reset();
        cczAssist.startprocess(util_win::to_tstring(path), cczProcInfo);
    }
}

#define CHECK_NULL_HANDLE(a, b)     if (((a) == NULL)) \
{\
    LOG((b));\
    return;\
}

void CCZAssitWrapper::autoclick()
{
    CHECK_NULL_HANDLE(cczProcInfo.hProcess, "CCZ is not running!");
    cczAssist.hookkeybdto_ccz(TEXT("libIPCO.dll"),TEXT("HookKeybdProc"), cczProcInfo.dwThreadId);
    cczAssist.autosend_mouseclick(cczAssist.get_ccz_mainwnd(cczProcInfo.dwThreadId), 90);
}

void CCZAssitWrapper::settimegear(float timeuprate)
{
    CHECK_NULL_HANDLE(cczProcInfo.hProcess, "CCZ is not running!");
    if (!bTimeHooked)
    {
        cczAssist.hookwndprocto_ccz(TEXT("libIPCO.dll"), TEXT("HookCallWndProc"), cczProcInfo.dwThreadId);
        bTimeHooked = cczAssist.sendsettimehook();
        Sleep(1000);
    }
    if (!bTimeHooked)
    {
        LOG("Time hook not correct!");
        return;
    }
    cczAssist.changetimespeed((unsigned long)(1000*timeuprate));
}

void CCZAssitWrapper::stopautoclick()
{
    CHECK_NULL_HANDLE(cczProcInfo.hProcess, "CCZ is not running!");
    cczAssist.stop_autosend();
}

void CCZAssitWrapper::writetoccz(unsigned long offset, const byte* data, size_t len)
{
    CHECK_NULL_HANDLE(cczProcInfo.hProcess, "CCZ is not running!");
    if (!check_hMemDO())
    {
        LOG("libMemDO doesn't loaded!");
        return;
    }
    if (!bgetDbgPriv)
    {
        if (!enableDebugPrivilege())
        {
            LOG("EnableDebugPrivilege Failed!");
            LOG(GetLastError());
            return;
        }
    }
    typedef int(*mdo_Mod_MemProc)(const tstring&, unsigned long, const byte*, size_t);
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

#undef CHECK_NULL_HANDLE
