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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (cczAssistLanguageSetting)
};

#endif // CCZ_CONFIGURATION_LANG_HEADER_H