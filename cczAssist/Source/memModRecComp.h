/*
  ==============================================================================

    memModRecComp.h
    Created: 22 Dec 2014 1:11:30pm

  ==============================================================================
*/

#ifndef CCZ_MEM_MOD_RECORD_COMPONENT_HEADER
#define CCZ_MEM_MOD_RECORD_COMPONENT_HEADER

#include "JuceHeader.h"

//==============================================================================
/*
*/
class MemModRecComp    
    : public Component
    , public ButtonListener
{
public:
    MemModRecComp() 
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        setSize(200, 600);
    }

    ~MemModRecComp()
    {
    }

    void paint (Graphics& g)
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    }

    void resized()
    {

    }

    void buttonClicked(Button* btnThatClicked)
    {
    }

private:


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MemModRecComp)

};


#endif  // CCZ_MEM_MOD_RECORD_COMPONENT_HEADER
