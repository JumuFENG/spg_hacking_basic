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
    btn_SetMem.setSize(70, 25);
    edt_Offset.setSize(70, 25);
    lbl_Offset.setSize(70, 25);
    edt_NewBytes.setSize(200, 100);
    edt_NewBytes.setMultiLine(true);
    addAndMakeVisible(lbl_Path_ccz);
    addAndMakeVisible(edt_Path_ccz);
    addAndMakeVisible(btn_Exec);
    addAndMakeVisible(btn_SetMem);
    addAndMakeVisible(edt_Offset);
    addAndMakeVisible(lbl_Offset);
    addAndMakeVisible(edt_NewBytes);
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

    topx = 10; topy += 35;
    edt_NewBytes.setTopLeftPosition(topx, topy);
    topx += 220;
    lbl_Offset.setTopLeftPosition(topx, topy);
    topy += 30;
    edt_Offset.setTopLeftPosition(topx, topy);
    topy += 40;
    btn_SetMem.setTopLeftPosition(topx, topy);
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
            cczAssistLibLoader::getInstance()->AutoClickCczMain();
        }
    }
    else if (btnThatClicked == &btn_SetMem)
    {
        String strOffset = edt_Offset.getText().trim();
        unsigned long offset = convetinputtoulong(strOffset);
        String strNewBytes = edt_NewBytes.getText().trim();
        std::vector<byte> newBytes = convertinputbytes(strNewBytes);
        if (!newBytes.empty())
        {
            cczAssistLibLoader::getInstance()->SetCczMemory(offset, (byte*)&newBytes[0], newBytes.size());
        }
    }
}
