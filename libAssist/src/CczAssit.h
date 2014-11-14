#ifndef _CCZ_ASSITANT_H
#define _CCZ_ASSITANT_H

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

class CCZAssitWrapper{
public:
    CCZAssitWrapper();
    ~CCZAssitWrapper();
    /** 
     * @param ccz_path [in] 路径
     * @param procInfo [out] 进程info
     * @ret 成功与否
     */
    bool startprocess(const tstring& ccz_path, PROCESS_INFORMATION& procInfo);

    static DWORD WINAPI AutoClickProc(LPVOID lpParam);
    static BOOL CALLBACK EnumThreadWindowProc(HWND hwnd, LPARAM lParam);
    /**
     * @param hWnd [in] 主窗口
     * @param invalidHeight [in] 无效区域，鼠标移动到上部则不发送
     * @param stillClickOnBg [in] 当窗口在后台时仍发送
     */
    void autosend_mouseclick(HWND hWnd, int invalidHeight, bool stillClickOnBg = true);

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
    void hookto_ccz(const tstring& dllFile, const tstring& procname, DWORD tid);

private:
    static bool bStop;
    tstring str_mainClassName;
    tstring str_mainWndName;
    HHOOK   hhookKeybdMsg;
    HWND    hcczMainWnd;
    HANDLE  hAutoThread;
    HANDLE  hAutoClickEvt;
    bool    stillClickOnBg;
    int     invalidHeight;
};

#endif //_CCZ_ASSITANT_H
