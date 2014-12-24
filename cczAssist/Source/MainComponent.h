/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "MainTabComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent()
    {
        spTc = new TabbedComponent(TabbedButtonBar::TabsAtTop);
        spTc->addTab("Main", Colour(0xffeeddff), new TextButton("hello"), false);
        //spTc->addTab("Second", Colour(0xffeeddff), tbtn, false);
        addAndMakeVisible(spTc);
        UILayoutConverter::Set_Comp_Size(this, "UL_MainWindow_Rect");
    }
    ~MainContentComponent()
    {

    }

    void paint(Graphics& g)
    {
        g.fillAll (Colour (0xffeeddff));
    }

    void resized()
    {
        spTc->setBounds(getBounds());
    }

private:
    SafePointer<TabbedComponent> spTc;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
