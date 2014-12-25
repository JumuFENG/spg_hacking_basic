/*
  ==============================================================================

    WCA: Weapon Clothes Assistants

  ==============================================================================
*/
#ifndef WCA_TAB_PAGE_COMPONENT_H_INCLUDED
#define WCA_TAB_PAGE_COMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "cczAssistConfig.h"
#include "cczAssistLibLoader.h"
#include "InputStringConverter.h"
#include "memModRecComp.h"

namespace UILC = UILayoutConverter;

using InputStringConverter::convertinputtoulong;
using InputStringConverter::convertinputbytes;

class WCATabPageComp   : public Component
    , public ButtonListener
    , public TextEditorListener
{
public:
    //==============================================================================
    WCATabPageComp()
    {
        lbl_WCAName.setText(UILC::Get_UI_Text("cczWCA_Label_WCAName"), dontSendNotification);
        lbl_PicNum.setText(UILC::Get_UI_Text("cczWCA_Label_PicNum"), dontSendNotification);
        lbl_Price.setText(UILC::Get_UI_Text("cczWCA_Label_Price"), dontSendNotification);
        lbl_EffectName.setText(UILC::Get_UI_Text("cczWCA_Label_EffectName"), dontSendNotification);
        lbl_EffectVal.setText(UILC::Get_UI_Text("cczWCA_Label_EffectVal"), dontSendNotification);
        lbl_TypeName.setText(UILC::Get_UI_Text("cczWCA_Label_TypeName"), dontSendNotification);
        lbl_OriginVal.setText(UILC::Get_UI_Text("cczWCA_Label_OriginVal"), dontSendNotification);
        lbl_LvDelta.setText(UILC::Get_UI_Text("cczWCA_Label_LvDelta"), dontSendNotification);

        UILC::Set_Comp_Size(&lbl_WCAName, "UL_WCA_lbl_WCAName");
        UILC::Set_Comp_Size(&lbl_PicNum, "UL_WCA_lbl_PicNum");
        UILC::Set_Comp_Size(&lbl_Price, "UL_WCA_lbl_Price");
        UILC::Set_Comp_Size(&lbl_EffectName, "UL_WCA_lbl_EffectName");
        UILC::Set_Comp_Size(&lbl_EffectVal, "UL_WCA_lbl_EffectVal");
        UILC::Set_Comp_Size(&lbl_TypeName, "UL_WCA_lbl_TypeName");
        UILC::Set_Comp_Size(&lbl_OriginVal, "UL_WCA_lbl_OriginVal");
        UILC::Set_Comp_Size(&lbl_LvDelta, "UL_WCA_lbl_LvDelta");

        addAndMakeVisible(lbl_WCAName);
        addAndMakeVisible(lbl_PicNum);
        addAndMakeVisible(lbl_Price);
        addAndMakeVisible(lbl_EffectName);
        addAndMakeVisible(lbl_EffectVal);
        addAndMakeVisible(lbl_TypeName);
        addAndMakeVisible(lbl_OriginVal);
        addAndMakeVisible(lbl_LvDelta);
    }

    ~WCATabPageComp()
    {
    }

    void paint (Graphics& g)
    {
        g.fillAll (Colour (0xffeeddff));

        g.setFont (Font (16.0f));
        g.setColour (Colours::black);
    }

    void resized()
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        UILC::Set_Comp_Pos(&lbl_WCAName, "UL_WCA_lbl_WCAName");
        UILC::Set_Comp_Pos(&lbl_PicNum,"UL_WCA_lbl_PicNum");
        UILC::Set_Comp_Pos(&lbl_Price, "UL_WCA_lbl_Price");
        UILC::Set_Comp_Pos(&lbl_EffectName, "UL_WCA_lbl_EffectName");
        UILC::Set_Comp_Pos(&lbl_EffectVal, "UL_WCA_lbl_EffectVal");
        UILC::Set_Comp_Pos(&lbl_TypeName, "UL_WCA_lbl_TypeName");
        UILC::Set_Comp_Pos(&lbl_OriginVal, "UL_WCA_lbl_OriginVal");
        UILC::Set_Comp_Pos(&lbl_LvDelta, "UL_WCA_lbl_LvDelta");
   }

    void buttonClicked(Button* btnThatClicked)
    {
    }

    void textEditorTextChanged (TextEditor& edt)
    {

    }

private:
    Label       lbl_WCAName;
    Label       lbl_PicNum;
    Label       lbl_Price;
    Label       lbl_EffectName;
    Label       lbl_EffectVal;
    Label       lbl_TypeName;
    Label       lbl_OriginVal;
    Label       lbl_LvDelta;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WCATabPageComp)
};

#endif WCA_TAB_PAGE_COMPONENT_H_INCLUDED
