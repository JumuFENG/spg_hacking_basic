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
    }

    ~cczAssistAppConfig()
    {
        fappcofig.replaceWithText(JSON::toString(varAppConfig));
    }

    juce_DeclareSingleton(cczAssistAppConfig, false);
public:
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

private:
    File     fappcofig;
    var      varAppConfig;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(cczAssistAppConfig);
};

#endif // CCZ_CONFIGURATION_LANG_HEADER_H