#include "MemDO.h"
#include "helper/helper.h"
#include "helper/simpleoutput.h"

int mdo_modify_memory(const tstring& strProcName, unsigned long offset, 
    const byte* content, size_t nsize
    )
{
    LOG("begin to write Mem");
    DWORD dwPid = helper::process::get_process_by_name(strProcName);
    unsigned long procStart = helper::process::get_process_base_address(dwPid);
    unsigned long realAddress = procStart + offset;
    HANDLE hTargetProc = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_TERMINATE|
        PROCESS_VM_OPERATION|PROCESS_VM_READ|
        PROCESS_VM_WRITE, FALSE, dwPid);
    if (hTargetProc == NULL)
    {
        LOG("OpenProcess failed!");
        LOG(GetLastError());
        return 1;
    }
    SIZE_T szWritten;
    if ( !WriteProcessMemory(hTargetProc, 
        (LPVOID)realAddress, content, nsize, &szWritten))
    {
        CloseHandle(hTargetProc);
        LOG("WriteProcessMemory failed!");
        LOG(GetLastError());
        return GetLastError();
    }
    LOG("WriteProcessMemory OK!");
    CloseHandle(hTargetProc);
    return 0;
}

int mdo_get_memory(const tstring& strProcName, unsigned long offset,
    const char* content, size_t& nsize)
{
    LOG("begin to Get Mem");
    DWORD dwPid = helper::process::get_process_by_name(strProcName);
    unsigned long procStart = helper::process::get_process_base_address(dwPid);
    unsigned long realAddress = procStart + offset;
    HANDLE hTargetProc = OpenProcess(PROCESS_ALL_ACCESS|PROCESS_TERMINATE|
        PROCESS_VM_OPERATION|PROCESS_VM_READ|
        PROCESS_VM_WRITE, FALSE, dwPid);
    if (hTargetProc == NULL)
    {
        LOG("OpenProcess failed!", GetLastError());
        LOG("Pid: ", dwPid);
        return 1;
    }

    SIZE_T szRead;
    if ( !ReadProcessMemory(hTargetProc, 
        (LPVOID)realAddress, (LPVOID)content, nsize, &szRead))
    {
        CloseHandle(hTargetProc);
        LOG("ReadProcessMemory failed!");
        LOG(GetLastError());
        return GetLastError();
    }
    nsize = szRead;
    LOG("ReadProcessMemory OK!");
    CloseHandle(hTargetProc);
    return 0;
}
