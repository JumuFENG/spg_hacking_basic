// 测试libMemDO中相关接口
#include <tchar.h>
#include "../../libMemDO/src/MemDO.h"

int main()
{
    HMODULE hMemDO = LoadLibrary(TEXT("libMemDO.dll"));
    if (hMemDO == NULL)
    {
        return 1;
    }
    typedef int(*mdo_Mod_MemProc)(const tstring&, unsigned long, byte*, size_t);
    mdo_Mod_MemProc mdo_mod_mem = (mdo_Mod_MemProc)GetProcAddress(hMemDO, "mdo_modify_memory");
    if (mdo_mod_mem != NULL)
    {
        byte bnop[] = {0x90, 0x90}; // 植物大战僵尸， 扣取阳光值的代码位置替换成nop
        // 0x01baa4 是植物大战僵尸中扣减阳光值的位置。
        mdo_mod_mem(tstring(_T("PlantsVsZombies.exe")), 0x01baa4, bnop, 2);
    }
    FreeLibrary(hMemDO);
    return 0;
}
