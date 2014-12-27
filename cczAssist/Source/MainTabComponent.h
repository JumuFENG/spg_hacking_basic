/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/
#ifndef MAIN_TAB_COMPONENT_H_INCLUDED
#define MAIN_TAB_COMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "cczAssistConfig.h"
#include "cczAssistLibLoader.h"
#include "InputStringConverter.h"
#include "memModRecComp.h"

using InputStringConverter::convertinputtoulong;
using InputStringConverter::convertinputbytes;

namespace UILayoutConverter{
    void Set_Comp_Pos(Component* c, const String& p)
    {
        juce::Rectangle<int> sz = cczAssistUILayout::getInstance()->GetUILayout(p);
        c->setTopLeftPosition(sz.getX(), sz.getY());
    }

    void Set_Comp_Size(Component* c, const String& s)
    {
        juce::Rectangle<int> sz = cczAssistUILayout::getInstance()->GetUILayout(s);
        c->setSize(sz.getWidth(), sz.getHeight());//(55, 25);
    }

    String Get_UI_Text(const String& t)
    {
        return cczAssistLanguageSetting::getInstance()->getUIText(t);
    }
}

class MainTabComponent   : public Component
    , public ButtonListener
    , public TextEditorListener
    , public SliderListener
    , public ChangeBroadcaster
{
public:
    //==============================================================================
    MainTabComponent() : bCczRunning(false)
    {
        //UILayoutConverter::Set_Comp_Size(this, "UL_MainWindow_Rect");
        //setSize(480, 200);
        lbl_Path_ccz.setText(cczAssistLanguageSetting::getInstance()->getUIText(
            String("cczAssistMain_Label_Path")), dontSendNotification);
        edt_Path_ccz.setText(cczAssistAppConfig::getInstance()->getAppCczInstallPath());
        edt_Path_ccz.addListener(this);
        btn_Exec.setButtonText(cczAssistLanguageSetting::getInstance()->getUIText(
            String(edt_Path_ccz.getText().isEmpty() ? 
            "cczAssistMain_btn_Browse" :
            "cczAssistMain_btn_Run") ));
        btn_Exec.addListener(this);

        UILayoutConverter::Set_Comp_Size(&lbl_Path_ccz, "UL_Label_Path_Rect");
        UILayoutConverter::Set_Comp_Size(&edt_Path_ccz, "UL_Edt_Path_Rect");
        UILayoutConverter::Set_Comp_Size(&btn_Exec, "UL_Btn_Exec_Rect");

        UILayoutConverter::Set_Comp_Size(&chkbx_AutoClk, "UL_Chkbx_AutoClick_Rect");
        chkbx_AutoClk.setButtonText(cczAssistLanguageSetting::getInstance()->
            getUIText(String("cczAssistMain_chkbx_Autoclk")));
        chkbx_AutoClk.setToggleState(cczAssistAppConfig::getInstance()->
            getAutoSendClick(), dontSendNotification);
        chkbx_AutoClk.addListener(this);
        UILayoutConverter::Set_Comp_Size(&btn_AutoClk, "UL_Btn_AutoClick_Rect");
        btn_AutoClk.setButtonText(cczAssistLanguageSetting::getInstance()->
            getUIText(String(chkbx_AutoClk.getToggleState() 
            ? "cczAssistMain_btn_Autoclk"
            :"cczAssistMain_btn_noAutoclk")));
        btn_AutoClk.setToggleState(chkbx_AutoClk.getToggleState(),dontSendNotification);
        btn_AutoClk.addListener(this);

        btn_SetMem.setButtonText(cczAssistLanguageSetting::getInstance()->getUIText(
            String("cczAssistMain_btn_SetMem")));
        btn_SetMem.addListener(this);
        lbl_Offset.setText(cczAssistLanguageSetting::getInstance()->getUIText(
            String("cczAssistMain_Label_Offset")), dontSendNotification);
        chkbx_SaveRecd.setButtonText(cczAssistLanguageSetting::getInstance()->
            getUIText("cczAssistMain_chkbx_SaveRec"));
        chkbx_SaveRecd.addListener(this);
        btn_SaveRecd.setButtonText(cczAssistLanguageSetting::getInstance()->
            getUIText("cczAssistMain_Text_Save"));
        btn_SaveRecd.setEnabled(false);
        btn_SaveRecd.addListener(this);
        edt_SaveRecName.setEnabled(false);
        UILayoutConverter::Set_Comp_Size(&btn_SetMem, "UL_Btn_SetMem_Rect");
        UILayoutConverter::Set_Comp_Size(&edt_Offset, "UL_Edt_Offset_Rect");
        UILayoutConverter::Set_Comp_Size(&lbl_Offset, "UL_Label_Offset_Rect");
        UILayoutConverter::Set_Comp_Size(&chkbx_SaveRecd, "UL_Chkbx_SaveRec_Rect");
        UILayoutConverter::Set_Comp_Size(&edt_SaveRecName, "UL_Edt_SaveRec_Rect");
        UILayoutConverter::Set_Comp_Size(&btn_SaveRecd, "UL_Btn_SaveRec_Rect");
        UILayoutConverter::Set_Comp_Size(&edt_NewBytes, "UL_Edt_NewBytes_Rect");

        edt_NewBytes.setMultiLine(true);

        timespeed_label.setText(cczAssistLanguageSetting::getInstance()->
            getUIText("cczAssistMain_Label_TimeSpeed"), dontSendNotification);
        UILayoutConverter::Set_Comp_Size(&timespeed_label, "UL_Label_TimeSpd_Rect");
        timespeed_Slider.setSliderStyle(Slider::LinearHorizontal);
        timespeed_Slider.setRange(-20, 20, 1.0);
        timespeed_Slider.setValue(cczAssistAppConfig::getInstance()
            ->getTimeSpeedRate(), dontSendNotification);
        timespeed_Slider.setChangeNotificationOnlyOnRelease(true);
        timespeed_Slider.setTextBoxStyle(Slider::TextBoxRight, false, 30, 20);
        timespeed_Slider.addListener(this);
        UILayoutConverter::Set_Comp_Size(&timespeed_Slider, "UL_Slider_TimeSpd_Rect");

        lbl_RecdList.setText(cczAssistLanguageSetting::getInstance()->
            getUIText("cczAssistMain_Label_UserModRecd"), dontSendNotification);
        UILayoutConverter::Set_Comp_Size(&lbl_RecdList, "UL_Label_RecdLst_Rect");
        usrRecdViewport.setScrollBarsShown(true, false, true);
        UILayoutConverter::Set_Comp_Size(&usrRecdViewport, "UL_ViewPort_Recd_Rect");
        usrRecdViewport.setViewedComponent(&usrRecdComp, false);
        chkbx_RecdSelAll.setButtonText(cczAssistLanguageSetting::getInstance()->
            getUIText("cczAssistMain_Text_SelectAll"));
        chkbx_RecdSelAll.addListener(this);
        UILayoutConverter::Set_Comp_Size(&chkbx_RecdSelAll, "UL_Chkbx_RecdSelAll_Rect");
        btn_SaveAutoApply.setButtonText(cczAssistLanguageSetting::getInstance()->
            getUIText("cczAssistMain_Text_Save"));
        UILayoutConverter::Set_Comp_Size(&btn_SaveAutoApply, "UL_Btn_SavAutoApply_Rect");
        btn_SaveAutoApply.addListener(this);
        btn_ApplyAllRecd.setButtonText(cczAssistLanguageSetting::getInstance()->
            getUIText("cczAssistMain_Text_Apply"));
        UILayoutConverter::Set_Comp_Size(&btn_ApplyAllRecd, "UL_Btn_ApplyAllRecd_Rect");
        btn_ApplyAllRecd.addListener(this);

        addAndMakeVisible(lbl_Path_ccz);
        addAndMakeVisible(edt_Path_ccz);
        addAndMakeVisible(btn_Exec);
        addAndMakeVisible(chkbx_AutoClk);
        addAndMakeVisible(btn_AutoClk);
        addAndMakeVisible(btn_SetMem);
        addAndMakeVisible(edt_Offset);
        addAndMakeVisible(lbl_Offset);
        addAndMakeVisible(chkbx_SaveRecd);
        addAndMakeVisible(edt_SaveRecName);
        addAndMakeVisible(btn_SaveRecd);
        addAndMakeVisible(edt_NewBytes);
        addAndMakeVisible(timespeed_label);
        addAndMakeVisible(timespeed_Slider);
        addAndMakeVisible(lbl_RecdList);
        addAndMakeVisible(usrRecdViewport);
        addAndMakeVisible(chkbx_RecdSelAll);
        addAndMakeVisible(btn_SaveAutoApply);
        addAndMakeVisible(btn_ApplyAllRecd);
    }

    ~MainTabComponent()
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
        UILayoutConverter::Set_Comp_Pos(&lbl_Path_ccz, "UL_Label_Path_Rect");
        UILayoutConverter::Set_Comp_Pos(&edt_Path_ccz, "UL_Edt_Path_Rect");
        UILayoutConverter::Set_Comp_Pos(&btn_Exec, "UL_Btn_Exec_Rect");

        UILayoutConverter::Set_Comp_Pos(&chkbx_AutoClk, "UL_Chkbx_AutoClick_Rect");
        UILayoutConverter::Set_Comp_Pos(&btn_AutoClk, "UL_Btn_AutoClick_Rect");    
        UILayoutConverter::Set_Comp_Pos(&timespeed_label, "UL_Label_TimeSpd_Rect");
        UILayoutConverter::Set_Comp_Pos(&timespeed_Slider, "UL_Slider_TimeSpd_Rect");

        UILayoutConverter::Set_Comp_Pos(&lbl_Offset, "UL_Label_Offset_Rect");
        UILayoutConverter::Set_Comp_Pos(&edt_Offset, "UL_Edt_Offset_Rect");
        UILayoutConverter::Set_Comp_Pos(&btn_SetMem, "UL_Btn_SetMem_Rect");
        UILayoutConverter::Set_Comp_Pos(&edt_NewBytes, "UL_Edt_NewBytes_Rect");
        UILayoutConverter::Set_Comp_Pos(&chkbx_SaveRecd, "UL_Chkbx_SaveRec_Rect");
        UILayoutConverter::Set_Comp_Pos(&edt_SaveRecName, "UL_Edt_SaveRec_Rect");
        UILayoutConverter::Set_Comp_Pos(&btn_SaveRecd, "UL_Btn_SaveRec_Rect");

        UILayoutConverter::Set_Comp_Pos(&lbl_RecdList, "UL_Label_RecdLst_Rect");
        UILayoutConverter::Set_Comp_Pos(&usrRecdViewport, "UL_ViewPort_Recd_Rect");
        UILayoutConverter::Set_Comp_Pos(&chkbx_RecdSelAll, "UL_Chkbx_RecdSelAll_Rect");
        UILayoutConverter::Set_Comp_Pos(&btn_SaveAutoApply, "UL_Btn_SavAutoApply_Rect");
        UILayoutConverter::Set_Comp_Pos(&btn_ApplyAllRecd, "UL_Btn_ApplyAllRecd_Rect");
    }

    void buttonClicked(Button* btnThatClicked)
    {
        if (btnThatClicked == &btn_Exec)
        {
            String toRun = edt_Path_ccz.getText();
            if (edt_Path_ccz.getText().isEmpty() || !File(toRun).exists())
            {
                // Open the file choose dialog
                FileChooser fc(cczAssistLanguageSetting::getInstance()->
                    getUIText("cczAssistMain_Filechooser_Tittle"), 
                    File::getSpecialLocation(File::globalApplicationsDirectory),
                    "*.exe");
                fc.browseForFileToOpen();
                toRun = fc.getResult().getFullPathName();
                if (toRun.isNotEmpty())
                {
                    edt_Path_ccz.setText(toRun);
                    btn_Exec.setButtonText(cczAssistLanguageSetting::getInstance()->
                        getUIText("cczAssistMain_btn_Run"));
                }
            }
            // Run the Program
            if (File(toRun).exists())
            {
                cczAssistLibLoader::getInstance()->RunCczProgram(toRun);
                cczAssistAppConfig::getInstance()->setAppCczInstallPath(toRun);
                if (chkbx_AutoClk.getToggleState() && btn_AutoClk.getToggleState())
                {
                    cczAssistLibLoader::getInstance()->AutoClickCczMain();
                }
                if (timespeed_Slider.getValue() != 0.0f)
                {
                    cczAssistLibLoader::getInstance()->SetTimeSpeed(timespeed_Slider.getValue());
                }
                usrRecdComp.ApplyAllSelected();
                bCczRunning = true;
                sendChangeMessage();        // The Application started up
            }
        }
        else if (btnThatClicked == &btn_SetMem)
        {
            String strOffset = edt_Offset.getText().trim();
            unsigned long offset = convertinputtoulong(strOffset);
            String strNewBytes = edt_NewBytes.getText().trim();
            std::vector<byte> newBytes = convertinputbytes(strNewBytes);
            if (!newBytes.empty() && offset != 0)
            {
                cczAssistLibLoader::getInstance()->SetCczMemory(offset,
                    (byte*)&newBytes[0], newBytes.size());
            }
        }
        else if (btnThatClicked == &btn_AutoClk)
        {
            btn_AutoClk.setToggleState(!btn_AutoClk.getToggleState(), dontSendNotification);
            btn_AutoClk.setButtonText(cczAssistLanguageSetting::getInstance()->getUIText(
                btn_AutoClk.getToggleState() ? "cczAssistMain_btn_Autoclk"
                :"cczAssistMain_btn_noAutoclk"));
            btn_AutoClk.getToggleState() ? 
                cczAssistLibLoader::getInstance()->AutoClickCczMain()
                : cczAssistLibLoader::getInstance()->StopAutoClick();
        }
        else if (btnThatClicked == &btn_SaveRecd)
        {
            usrRecdComp.AddUserModifyRecord(
                edt_SaveRecName.getText(),
                edt_Offset.getText(),
                edt_NewBytes.getText(), false);
            usrRecdComp.resized();
        }
        else if (btnThatClicked == &chkbx_AutoClk)
        {
            cczAssistAppConfig::getInstance()->setAutoSendClick(chkbx_AutoClk.getToggleState());
        }
        else if (btnThatClicked == &chkbx_SaveRecd)
        {
            bool checked = chkbx_SaveRecd.getToggleState();
            edt_SaveRecName.setEnabled(checked);
            btn_SaveRecd.setEnabled(checked);
        }
        else if (btnThatClicked == &chkbx_RecdSelAll)
        {
            usrRecdComp.UnSelectAll(chkbx_RecdSelAll.getToggleState());
        }
        else if (btnThatClicked == &btn_ApplyAllRecd)
        {
            usrRecdComp.ApplyAllSelected();
        }
        else if (btnThatClicked == &btn_SaveAutoApply)
        {
            cczAssistAppConfig::getInstance()->saveConfig();
        }
    }

    void textEditorTextChanged (TextEditor& edt)
    {
        if (&edt == &edt_Path_ccz)
        {
            btn_Exec.setButtonText( cczAssistLanguageSetting::getInstance()->getUIText(
                String( (edt_Path_ccz.isEmpty() || !File(edt_Path_ccz.getText()).exists() ||
                File(edt_Path_ccz.getText()).isDirectory())
                ? "cczAssistMain_btn_Browse" : "cczAssistMain_btn_Run")));
        }
    }


    void sliderValueChanged (Slider* slider)
    {
        if (slider == &timespeed_Slider)
        {
            float sldVal = timespeed_Slider.getValue();
            cczAssistLibLoader::getInstance()->SetTimeSpeed(sldVal);
            cczAssistAppConfig::getInstance()->setTimeSpeedRate(sldVal);
        }
    }

public:
    bool isCczRunning() {return bCczRunning;}

private:
    bool        bCczRunning;

private:
    TextButton  btn_Exec;
    Label       lbl_Path_ccz;
    TextEditor  edt_Path_ccz;

    ToggleButton chkbx_AutoClk;
    TextButton   btn_AutoClk;

    TextButton  btn_SetMem;
    Label       lbl_Offset;
    TextEditor  edt_Offset;
    TextEditor  edt_NewBytes;
    ToggleButton chkbx_SaveRecd;
    TextEditor  edt_SaveRecName;
    TextButton  btn_SaveRecd;

    Slider      timespeed_Slider;
    Label       timespeed_label;

    Label       lbl_RecdList;
    Viewport    usrRecdViewport;
    MemModRecComp usrRecdComp;
    ToggleButton  chkbx_RecdSelAll;
    TextButton    btn_SaveAutoApply;
    TextButton    btn_ApplyAllRecd;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainTabComponent)
};

#endif MAIN_TAB_COMPONENT_H_INCLUDED
