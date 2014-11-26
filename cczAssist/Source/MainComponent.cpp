/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "cczAssistConfig.h"
#include "cczAssistLibLoader.h"
#include "InputStringConverter.h"

using InputStringConverter::convetinputtoulong;
using InputStringConverter::convertinputbytes;

//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (500, 400);
    lbl_Path_ccz.setText(cczAssistLanguageSetting::getInstance()->getUIText(
        String("cczAssistMain_Label_Path")), dontSendNotification);
    edt_Path_ccz.setText(cczAssistAppConfig::getInstance()->getAppCczInstallPath());
    edt_Path_ccz.addListener(this);
    btn_Exec.setButtonText(cczAssistLanguageSetting::getInstance()->getUIText(
        String(edt_Path_ccz.getText().isEmpty() ? 
        "cczAssistMain_btn_Browse" :
        "cczAssistMain_btn_Run") ));
    btn_Exec.addListener(this);

    lbl_Path_ccz.setSize(55, 25);
    edt_Path_ccz.setSize(300, 25);
    btn_Exec.setSize(90, 25);

    chkbx_AutoClk.setButtonText(cczAssistLanguageSetting::getInstance()->
        getUIText(String("cczAssistMain_chkbx_Autoclk")));
    chkbx_AutoClk.setToggleState(cczAssistAppConfig::getInstance()->
        getAutoSendClick(), dontSendNotification);
    chkbx_AutoClk.addListener(this);
    chkbx_AutoClk.setSize(100, 22);
    btn_AutoClk.setButtonText(cczAssistLanguageSetting::getInstance()->
        getUIText(String(chkbx_AutoClk.getToggleState() 
        ? "cczAssistMain_btn_Autoclk"
        :"cczAssistMain_btn_noAutoclk")));
    btn_AutoClk.setSize(80, 22);
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
    btn_SetMem.setSize(70, 25);
    edt_Offset.setSize(70, 25);
    lbl_Offset.setSize(70, 25);
    chkbx_SaveRecd.setSize(100, 25);
    edt_SaveRecName.setSize(70, 25);
    btn_SaveRecd.setSize(55, 25);
    edt_NewBytes.setSize(230, 60);
    edt_NewBytes.setMultiLine(true);

    timespeed_label.setText(cczAssistLanguageSetting::getInstance()->
        getUIText("cczAssistMain_Label_TimeSpeed"), dontSendNotification);
    timespeed_label.setSize(35, 20);
    timespeed_Slider.setSliderStyle(Slider::LinearHorizontal);
    timespeed_Slider.setRange(-20, 20, 1.0);
    timespeed_Slider.setValue(cczAssistAppConfig::getInstance()
        ->getTimeSpeedRate(), dontSendNotification);
    timespeed_Slider.setChangeNotificationOnlyOnRelease(true);
    timespeed_Slider.setTextBoxStyle(Slider::TextBoxRight, false, 30, 20);
    timespeed_Slider.addListener(this);
    timespeed_Slider.setSize(180, 20);

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
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    int topx = 10, topy = 5;
    lbl_Path_ccz.setTopLeftPosition(topx, topy);
    topx += 70;
    edt_Path_ccz.setTopLeftPosition(topx, topy);
    topx += 310;
    btn_Exec.setTopLeftPosition(topx, topy);

    topx = 50; topy += 30;
    chkbx_AutoClk.setTopLeftPosition(topx, topy);
    topx += 115;
    btn_AutoClk.setTopLeftPosition(topx, topy);
    topx += 100;
    timespeed_label.setTopLeftPosition(topx, topy + 2);
    topx += 30; 
    timespeed_Slider.setTopLeftPosition(topx, topy);

    topx = 10; topy += 30;
    lbl_Offset.setTopLeftPosition(topx, topy);
    topx += 70;
    edt_Offset.setTopLeftPosition(topx, topy);
    topx += 95;
    btn_SetMem.setTopLeftPosition(topx, topy);
    topx = 10; topy += 30;
    edt_NewBytes.setTopLeftPosition(topx + 5, topy);
    topy += 70;
    chkbx_SaveRecd.setTopLeftPosition(topx, topy);
    topx += 100;
    edt_SaveRecName.setTopLeftPosition(topx, topy);
    topx += 80;
    btn_SaveRecd.setTopLeftPosition(topx, topy);

}

void MainContentComponent::buttonClicked(Button* btnThatClicked)
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
        }
    }
    else if (btnThatClicked == &btn_SetMem)
    {
        String strOffset = edt_Offset.getText().trim();
        unsigned long offset = convetinputtoulong(strOffset);
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
        edt_SaveRecName.setText("place holder");
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
}

void MainContentComponent::textEditorTextChanged (TextEditor& edt)
{
    if (&edt == &edt_Path_ccz)
    {
        btn_Exec.setButtonText( cczAssistLanguageSetting::getInstance()->getUIText(
            String( (edt_Path_ccz.isEmpty() || !File(edt_Path_ccz.getText()).exists() ||
            File(edt_Path_ccz.getText()).isDirectory())
            ? "cczAssistMain_btn_Browse" : "cczAssistMain_btn_Run")));
    }
}


void MainContentComponent::sliderValueChanged (Slider* slider)
{
    if (slider == &timespeed_Slider)
    {
        float sldVal = timespeed_Slider.getValue();
        cczAssistLibLoader::getInstance()->SetTimeSpeed(sldVal);
        cczAssistAppConfig::getInstance()->setTimeSpeedRate(sldVal);
    }
}
