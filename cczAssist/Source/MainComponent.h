/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "memModRecComp.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
    , public ButtonListener
    , public TextEditorListener
    , public SliderListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();

    void buttonClicked(Button* );
    void textEditorTextChanged (TextEditor&);
    void sliderValueChanged (Slider* );

public:
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
