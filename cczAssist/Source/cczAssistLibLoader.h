/*
  ==============================================================================

    The class to load libAssist and privide interface for UI

  ==============================================================================
*/

#ifndef CCZ_LIB_LOADER_HEADER_H
#define CCZ_LIB_LOADER_HEADER_H

#include "JuceHeader.h"
#include "libAssist/src/CczAssit.h"
 
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

private:
    HMODULE hAssist;
    CCZWrapperBase* caw;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cczAssistLibLoader)
};

#endif // CCZ_LIB_LOADER_HEADER_H