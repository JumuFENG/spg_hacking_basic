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

    void SetCczMemory(unsigned long offset, const byte* data, size_t len)
    {
        if (caw != NULL)
        {
            caw->writetoccz(offset, data, len);
        }
    }

private:
    HMODULE hAssist;
    CCZWrapperBase* caw;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cczAssistLibLoader)
};

#endif // CCZ_LIB_LOADER_HEADER_H