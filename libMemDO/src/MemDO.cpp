#include "MemDO.h"
#include "helper/helper.h"

int mdo_modify_memory(const tstring& strProcName, unsigned long offset, 
    const byte* content, size_t nsize
    )
{
    DWORD dwPid = helper::process::get_process_by_name(strProcName);
    unsigned long procStart = helper::process::get_process_base_address(dwPid);
    unsigned long realAddress = procStart + offset;
    HANDLE hTargetProc = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_TERMINATE|
        PROCESS_VM_OPERATION|PROCESS_VM_READ|
        PROCESS_VM_WRITE, FALSE, dwPid);
    if (hTargetProc == NULL)
    {
        return 1;
    }
    SIZE_T szWritten;
    if ( !WriteProcessMemory(hTargetProc, 
        (LPVOID)realAddress, content, nsize, &szWritten))
    {
        CloseHandle(hTargetProc);
        return GetLastError();
    }
    CloseHandle(hTargetProc);
    return 0;
}
