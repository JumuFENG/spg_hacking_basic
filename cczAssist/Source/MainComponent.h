/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
    , public ButtonListener
    , public TextEditorListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();

    void buttonClicked(Button* btnThatClicked);
    void textEditorTextChanged (TextEditor&);

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

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
