#ifndef _CCZ_ASSITANT_H
#define _CCZ_ASSITANT_H

#if defined (libAssist_EXPORTS)
#   define Ccc_Assist_API __declspec(dllexport)
#else
#   define Ccc_Assist_API __declspec(dllimport)
#endif

#include "helper/tstring.h"
#include <vector>

#define CCZ_AUTOCLICK_EVENT_NAME TEXT("CczAutoClickEvent")

class EnumWndBase{
public:
    EnumWndBase(){}
    virtual ~EnumWndBase(){}

    virtual bool wndselector(HWND h) = 0;
    HWND get_find_result(){ return allfind.empty() ? NULL : allfind.front(); }
    void add_find(HWND h){ allfind.push_back(h); }
    bool is_find_single() { return allfind.size() == 1; }
    std::vector<HWND> get_all_result() {return allfind; }

protected:
    std::vector<HWND>  allfind;
};

class CCZAssitSdk
{
public:
    CCZAssitSdk();
    ~CCZAssitSdk();
    /** 
     * @param ccz_path [in] 路径
     * @param procInfo [out] 进程info
     * @ret 成功与否
     */
    bool startprocess(const tstring& ccz_path, PROCESS_INFORMATION& procInfo);
    bool isprocessrunning(const tstring& ccz_path);

    static DWORD WINAPI AutoClickProc(LPVOID lpParam);
    static BOOL CALLBACK EnumThreadWindowProc(HWND hwnd, LPARAM lParam);
    /**
     * @param hWnd [in] 主窗口
     * @param invalidHeight [in] 无效区域，鼠标移动到上部则不发送
     * @param stillClickOnBg [in] 当窗口在后台时仍发送
     */
    void autosend_mouseclick(HWND hWnd, int invalidHeight, bool stillClickOnBg = true);

    void stop_autosend();

    /**
     * 获取线程主窗口句柄
     */
    HWND get_main_window(const DWORD& tid, EnumWndBase* ewb);

    HWND get_ccz_mainwnd(const DWORD& tid);
    HWND get_ccz_mainwnd() {return hcczMainWnd;}
    void set_ccz_mainwnd(HWND hwnd) {hcczMainWnd = hwnd; }
    /**
     * 注入以捕获键盘事件
     * @param dllFile [in] dll 文件名，完整路径或相对路径
     * @param procname [in] dll中需进行注入的接口
     * @param tid [in] 要注入的窗口线程id
     */
    void hookkeybdto_ccz(const tstring& dllFile, const tstring& procname, DWORD tid);
    void hookwndprocto_ccz(const tstring& dllFile, const tstring& procname, DWORD tid);
    bool sendsettimehook();
    void changetimespeed(unsigned long uprate);
    void initall();
    void resetall();

private:
    static bool bStop;
    tstring str_mainClassName;
    tstring str_mainWndName;
    HINSTANCE hinstDLL;
    HHOOK   hhookKeybdMsg;
    HHOOK   hhookWndProc;
    HWND    hcczMainWnd;
    HANDLE  hAutoThread;
    HANDLE  hAutoClickEvt;
    bool    stillClickOnBg;
    int     invalidHeight;
};

class CCZWrapperBase {
public:
    CCZWrapperBase()
    {
    }
    virtual ~CCZWrapperBase(){}
    virtual void startccz(const string&) = 0;
    virtual void autoclick() = 0;
    virtual void settimegear(float timeuprate) = 0;
    virtual void stopautoclick() = 0;
    virtual void writetoccz(unsigned long, const byte*, size_t) = 0;
};

class CCZAssitWrapper 
    : public CCZWrapperBase
{
public:
    CCZAssitWrapper()
        : hMemDO(NULL)
        , bgetDbgPriv(false)
        , bTimeHooked(false)
    {
        cczProcInfo.hProcess = NULL;
        cczProcInfo.hThread = NULL;
        cczProcInfo.dwProcessId = 0;
        cczProcInfo.dwThreadId = 0;
    }

    ~CCZAssitWrapper()
    {
        if (hMemDO != NULL)
        {
            FreeLibrary(hMemDO);
        }
    }
private:
    // this will delay the dll to load when needed, and load only one instance
    bool check_hMemDO();
    bool enableDebugPrivilege();
    void reset();

public:
    // the following are exported
    void startccz(const string&);
    void autoclick();
    void stopautoclick();
    void settimegear(float timeuprate);
    void writetoccz(unsigned long offset, const byte* data, size_t len);

private:
    CCZAssitSdk cczAssist;
    PROCESS_INFORMATION cczProcInfo;
    HMODULE     hMemDO;
    bool        bgetDbgPriv;
    bool        bTimeHooked;
};

extern "C" __declspec(dllexport) CCZWrapperBase* GetAssistWrapperObject();
extern "C" __declspec(dllexport) void ReleaseAW(CCZWrapperBase*);

#endif //_CCZ_ASSITANT_H
