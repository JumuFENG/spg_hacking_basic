/*
  ==============================================================================

    The class to load libAssist and privide interface for UI

  ==============================================================================
*/

#ifndef CCZ_LIB_LOADER_HEADER_H
#define CCZ_LIB_LOADER_HEADER_H

#include "JuceHeader.h"
#include "libAssist/src/CczAssit.h"
#include "libAssist/src/CczDataStructs.h"

#ifndef LOG
#define LOG(x) Logger::writeToLog(String(__FILE__) + " [" + \
    String(__LINE__) + "] " + String((x)));
#endif

class cczAssistLibLoader
{
public:
    cczAssistLibLoader()
    {
        hAssist = LoadLibrary("libAssist.dll");
        typedef CCZWrapperBase* (* GetObj)();
        GetObj assistHelper = (GetObj)GetProcAddress(hAssist, "GetAssistWrapperObject");
        caw = assistHelper == NULL ? NULL : assistHelper();
    }

    ~cczAssistLibLoader()
    {
        if (caw != NULL)
        {
            typedef void(*ObjRelease)(CCZWrapperBase*);
            ObjRelease assistRelease = (ObjRelease)GetProcAddress(hAssist, "ReleaseAW");
            if (assistRelease != NULL)
            {
                assistRelease(caw);
            }
        }
        FreeLibrary(hAssist);
    }

    juce_DeclareSingleton(cczAssistLibLoader, false);

    void RunCczProgram(const String& fullPath)
    {
        if (caw != NULL)
        {
            caw->startccz(fullPath.toStdString());
        }
    }

    void AutoClickCczMain()
    {
        if (caw != NULL)
        {
            caw->autoclick();
        }
    }

    void StopAutoClick()
    {
        if (caw != NULL)
        {
            caw->stopautoclick();
        }
    }

    void SetCczMemory(unsigned long offset, const byte* data, size_t len)
    {
        if (caw != NULL)
        {
            caw->writetoccz(offset, data, len);
        }
    }

    std::vector<byte> GetCczMemory(unsigned long offset, size_t len)
    {
        std::vector<byte> mem;
        if (caw != NULL)
        {
            byte* pbytes = new byte[len];
            size_t rsize = len;
            caw->readfromccz(offset, pbytes, rsize);
            if (rsize == len)
            {
                for (size_t i = 0; i < rsize; ++i)
                {
                    mem.push_back(pbytes[i]);
                }
            }
            if (pbytes != NULL)
            {
                delete[] pbytes;
                pbytes = NULL;
            }
        }
        return mem;
    }

    void SetTimeSpeed(float ar)
    {
        if (caw != NULL)
        {
            float r = ar < 0.0f ? 1.0f / (1.0f - ar) : 1.0f + ar;
            caw->settimegear(r);
        }
    }

    std::vector<ItemDetail> GetCczItems(bool bRepeatForGetData = false)
    {
        std::vector<ItemDetail> allItems;
        int itemNum = kItemsNum;
        unsigned long itemOffset = kItemsOffset;
        size_t rsize = itemNum * sizeof(ItemDetail);
        std::vector<byte> itemMem;
        int maxRepeat = 1000;
        bool getFaultData = true;
        do 
        {
            itemMem = GetCczMemory(itemOffset, rsize);
            Sleep(1000);
            getFaultData = (itemMem.empty() || itemMem[0] == 0);
        } while (getFaultData && bRepeatForGetData && maxRepeat--);

        if (!getFaultData)
        {
            PItemDetail pItems = (PItemDetail)&itemMem[0];
            for (int i = 0; i < itemNum; ++i, pItems ++)
            {
                allItems.push_back(*pItems);
            }
        }
        else
        {
            LOG("Get Mem failed!");
        }
        return allItems;
    }

    bool GetCczSingleItem(int idx, ItemDetail& rltDetail)
    {
        if (idx >= kItemsNum)
        {
            LOG("Index Out of range!");
            return false;
        }

        unsigned long itemOffset = kItemsOffset + idx * sizeof(ItemDetail);
        std::vector<byte> itemMem = GetCczMemory(itemOffset, sizeof(ItemDetail));
        if (itemMem.empty() || itemMem[0] == 0)
        {
            return false;
        }
        rltDetail = *((PItemDetail)&itemMem[0]);
        return true;
    }

    void WriteItemToCcz(int idx, const ItemDetail& itDtl)
    {
        if (idx < 0 || idx >= kItemsNum)
        {
            LOG("Bad Item Idx to Write");
            return;
        }
        unsigned long aOffset = kItemsOffset + idx * sizeof(ItemDetail);
        const int noMod = 17;
        aOffset += noMod;
        SetCczMemory(aOffset, (const byte *)&itDtl + noMod, sizeof(ItemDetail) - noMod);
    }

private:
    HMODULE hAssist;
    CCZWrapperBase* caw;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cczAssistLibLoader)
};

#endif // CCZ_LIB_LOADER_HEADER_H