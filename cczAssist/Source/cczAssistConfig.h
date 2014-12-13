/*
  ==============================================================================

    The class to parse and save configuration and language files

  ==============================================================================
*/

#ifndef CCZ_CONFIGURATION_LANG_HEADER_H
#define CCZ_CONFIGURATION_LANG_HEADER_H

#include "JuceHeader.h"

class cczAssistLanguageSetting
{
public:
    cczAssistLanguageSetting(const String& lang_locale = String::empty)
    {
        File jsonFile = File::getCurrentWorkingDirectory().getChildFile(String("lang/")
            + ( lang_locale.isNotEmpty() ? lang_locale : String("zh_CN") )
            + String(".json"));
        if (jsonFile.exists())
        {
            varlangText = JSON::parse(jsonFile);
        }
    }
    ~cczAssistLanguageSetting(){}

    juce_DeclareSingleton(cczAssistLanguageSetting, false);

    String getUIText(const String& target)
    {
        return varlangText[Identifier(target)];
    }

private:
    var      varlangText;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cczAssistLanguageSetting);
};

class cczAssistUILayout{
public:
    cczAssistUILayout()
    {
        File jsonFile = File::getCurrentWorkingDirectory().getChildFile(
            "settings/layout.json");
        if (jsonFile.exists())
        {
            varUIElem = JSON::parse(jsonFile);
        }
    }
    ~cczAssistUILayout()
    {

    }
    juce_DeclareSingleton(cczAssistUILayout, false);

    juce::Rectangle<int> GetUILayout(const String& s)
    {
        return juce::Rectangle<int>::fromString(varUIElem[(Identifier)s].toString());
    }

    juce::Rectangle<int> GetUIPos();

private:
    var    varUIElem;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(cczAssistUILayout);
};

class cczAssistAppConfig{
public:
    cczAssistAppConfig()
    {
        fappcofig = File::getCurrentWorkingDirectory().getChildFile("settings/appconfig.json");
        if (!fappcofig.exists())
        {
            fappcofig.create();
        }
        varAppConfig = JSON::parse(fappcofig);
        if (NULL == varAppConfig.getDynamicObject())
        {
            varAppConfig = JSON::parse("{}");
        }
        varUserAddedItems = varAppConfig["UserAdded"];
    }

    ~cczAssistAppConfig()
    {
        saveConfig();
    }

    juce_DeclareSingleton(cczAssistAppConfig, false);
public:
    void saveConfig()
    {
        varAppConfig.getDynamicObject()->setProperty("UserAdded", varUserAddedItems);
        fappcofig.replaceWithText(JSON::toString(varAppConfig));
    }

    bool getAutoSendClick()
    {
        return getConfigBool(String("AppSetting_AutoClick"));
    }

    void setAutoSendClick(bool bAuto)
    {
        varAppConfig.getDynamicObject()->setProperty("AppSetting_AutoClick", bAuto);
    }

    String getAppCczInstallPath()
    {
        return getConfigStr("AppSetting_CczInstallPath");
    }

    void setAppCczInstallPath(const String& path )
    {
        varAppConfig.getDynamicObject()->setProperty("AppSetting_CczInstallPath", path);
    }

    float getTimeSpeedRate()
    {
        float srate = getConfigFloat("AppSetting_TimeSpeed");
        if (srate == 0.0f)
        {
            setTimeSpeedRate(0.0f);
        }
        return srate;
    }

    void setTimeSpeedRate(float aR)
    {
        varAppConfig.getDynamicObject()->setProperty("AppSetting_TimeSpeed", aR);
    }

    void setUserAddItem(const String& n, const String& offset, const String& val, bool bEnable = false)
    {
        bool bFound = false;
        if (varUserAddedItems.isArray())
        {
            for (int i = 0; i < varUserAddedItems.size(); ++i)
            {
                DynamicObject* dobj = varUserAddedItems[i].getDynamicObject();
                if (dobj->getProperty("Name") == n)
                {
                    bFound = true;
                    dobj->setProperty("Offset", offset);
                    dobj->setProperty("Bytes", val);
                    dobj->setProperty("AutoEnable", bEnable);
                    break;
                }
            }
        }
        if (!bFound)
        {
            var varItem = JSON::parse("{}");
            varItem.getDynamicObject()->setProperty("Name", n);
            varItem.getDynamicObject()->setProperty("Offset", offset);
            varItem.getDynamicObject()->setProperty("Bytes", val);
            varItem.getDynamicObject()->setProperty("AutoEnable", bEnable);
            varUserAddedItems.append(varItem);
        }
    }

    void setUserAddItemAutoEnabled(const String& n, bool bEnable = true)
    {
        if (varUserAddedItems.isArray())
        {
            for (int i = 0; i < varUserAddedItems.size(); ++i)
            {
                DynamicObject* dobj = varUserAddedItems[i].getDynamicObject();
                if (dobj->getProperty("Name") == n)
                {
                    dobj->setProperty("AutoEnable", bEnable);
                    break;
                }
            }
        }
    }

    var getUserAddItems()
    {
        return varUserAddedItems;
    }

private:
    String getConfigStr(const String& cfgId)
    {
        return varAppConfig[(Identifier)cfgId];
    }

    bool getConfigBool(const String& cfgId)
    {
        return varAppConfig[(Identifier)cfgId];
    }

    size_t getConfigSize(const String& cfgId)
    {
        return int(varAppConfig[(Identifier)cfgId]);
    }

    float getConfigFloat(const String& cfgId)
    {
        return float(varAppConfig[(Identifier)cfgId]);
    }

private:
    File     fappcofig;
    var      varAppConfig;
    var      varUserAddedItems;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(cczAssistAppConfig);
};

#endif // CCZ_CONFIGURATION_LANG_HEADER_H