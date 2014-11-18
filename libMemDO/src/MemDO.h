#ifndef MEM_DYNAMIC_OPERATE_HEADER_
#define MEM_DYNAMIC_OPERATE_HEADER_

#include "helper/tstring.h"
#include <Windows.h>

/** 
 * @param strProcName [in] 进程名
 * @param offset [in] 要替换的内存地址相对于进程起始地址的偏移量
 * @param content [in] 要替换的内容
 * @param nsize [in] 要替换的字节数
 * @ret 成功返回0
 * 本接口修改进程的内存值，若替换的是进程中所执行的代码必须由调用者确保
 * 替换前后相应位置的内容不会影响前后代码段的正常执行。Be Carefully!
 */
int mdo_modify_memory(const tstring& strProcName, unsigned long offset,
    const byte* content, size_t nsize);

#endif //MEM_DYNAMIC_OPERATE_HEADER_
