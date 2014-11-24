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
    btn_SetMem.setButtonText(cczAssistLanguageSetting::getInstance()->getUIText(
        String("cczAssistMain_btn_SetMem")));
    btn_SetMem.addListener(this);
    lbl_Offset.setText(cczAssistLanguageSetting::getInstance()->getUIText(
        String("cczAssistMain_Label_Offset")), dontSendNotification);
    lbl_Path_ccz.setSize(55, 25);
    edt_Path_ccz.setSize(220, 25);
    btn_Exec.setSize(90, 25);

    chkbx_AutoClk.setButtonText(cczAssistLanguageSetting::getInstance()->
        getUIText(String("cczAssistMain_chkbx_Autoclk")));
    chkbx_AutoClk.setToggleState(cczAssistAppConfig::getInstance()->
        getAutoSendClick(), dontSendNotification);
    chkbx_AutoClk.addListener(this);
    chkbx_AutoClk.setSize(150, 22);
    btn_AutoClk.setButtonText(cczAssistLanguageSetting::getInstance()->
        getUIText(String(chkbx_AutoClk.getToggleState() 
        ? "cczAssistMain_btn_Autoclk"
        :"cczAssistMain_btn_noAutoclk")));
    btn_AutoClk.setSize(80, 22);
    btn_AutoClk.setToggleState(chkbx_AutoClk.getToggleState(),dontSendNotification);
    btn_AutoClk.addListener(this);

    btn_SetMem.setSize(70, 25);
    edt_Offset.setSize(70, 25);
    lbl_Offset.setSize(70, 25);
    edt_NewBytes.setSize(200, 100);
    edt_NewBytes.setMultiLine(true);

    timespeed_Slider.setSliderStyle(Slider::LinearVertical);
    timespeed_Slider.setRange(-20, 20, 1.0);
    float sldVal = cczAssistAppConfig::getInstance()->getTimeSpeedRate();
    if (sldVal != 0.0f)
    {
        cczAssistLibLoader::getInstance()->SetTimeSpeed(sldVal);
    }
    timespeed_Slider.setValue(sldVal, dontSendNotification);
    timespeed_Slider.setChangeNotificationOnlyOnRelease(true);
    timespeed_Slider.setTextBoxStyle(Slider::TextBoxAbove, false, 50, 20);
    timespeed_Slider.addListener(this);
    timespeed_Slider.setSize(60, 135);

    addAndMakeVisible(lbl_Path_ccz);
    addAndMakeVisible(edt_Path_ccz);
    addAndMakeVisible(btn_Exec);
    addAndMakeVisible(chkbx_AutoClk);
    addAndMakeVisible(btn_AutoClk);
    addAndMakeVisible(btn_SetMem);
    addAndMakeVisible(edt_Offset);
    addAndMakeVisible(lbl_Offset);
    addAndMakeVisible(edt_NewBytes);
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
    topx += 60;
    edt_Path_ccz.setTopLeftPosition(topx, topy);
    topx += 230;
    btn_Exec.setTopLeftPosition(topx, topy);

    topx = 50; topy += 30;
    chkbx_AutoClk.setTopLeftPosition(topx, topy);
    topx += 160;
    btn_AutoClk.setTopLeftPosition(topx, topy);
    topx = 10; topy += 27;
    edt_NewBytes.setTopLeftPosition(topx, topy);
    topx += 220;
    lbl_Offset.setTopLeftPosition(topx, topy);
    topy += 30;
    edt_Offset.setTopLeftPosition(topx, topy);
    topy += 40;
    btn_SetMem.setTopLeftPosition(topx, topy);

    topx += 80; topy -= 95;
    timespeed_Slider.setTopLeftPosition(topx, topy);
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
    else if (btnThatClicked == &chkbx_AutoClk)
    {
        cczAssistAppConfig::getInstance()->setAutoSendClick(chkbx_AutoClk.getToggleState());
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
