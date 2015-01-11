/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "JuceHeader.h"
#include "MainTabComponent.h"
#include "WCATabPageComp.h"

namespace UILC = UILayoutConverter;
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
        tabMain = new MainTabComponent();
        tabWCA = new WCATabPageComp();
        tabMain->addChangeListener(tabWCA);
        spTc = new TabbedComponent(TabbedButtonBar::TabsAtTop);
        spTc->addTab(UILC::Get_UI_Text("cczAssistMain_TabName_Main"), Colour(0xffeeddff), tabMain, false);
        spTc->addTab(UILC::Get_UI_Text("cczAssistMain_TabName_WCA"), Colour(0xffeeddff), tabWCA, false);
        spTc->setCurrentTabIndex(1);
        addAndMakeVisible(spTc);
        UILC::Set_Comp_Size(this, "UL_MainWindow_Rect");
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
    ScopedPointer<TabbedComponent>         spTc;
    ScopedPointer<MainTabComponent>        tabMain;
    ScopedPointer<WCATabPageComp>          tabWCA;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
