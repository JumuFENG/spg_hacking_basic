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
class UserRecordApplyComp 
    : public Component
    , public ButtonListener
{
public:
    UserRecordApplyComp()
    {
        chkbx_Name.setSize(120, 22);
        btn_Apply.setSize(40, 22);
        btn_Restore.setSize(40, 22);

        addAndMakeVisible(chkbx_Name);
        addAndMakeVisible(btn_Apply);
        addAndMakeVisible(btn_Restore);
    }
    ~UserRecordApplyComp(){}
    
    void InitUARecd(const String& aName, unsigned long off_, std::vector<byte> bs)
    {
        offset = off_;
        udName = aName;
        usBytes = bs;
        //获取该偏移处 usBytes.size() 个原始值
        oldBytes = ;
    }

    void SetToCczMem()
    {

    }

    void RestoreCczMem()
    {

    }

    void paint(Graphics& g)
    {
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);
    }

    void resized()
    {
        chkbx_Name.setTopLeftPosition(3, 1);
        btn_Apply.setTopLeftPosition(125, 1);
        btn_Restore.setTopLeftPosition(168, 1);
    }

    void buttonClicked(Button* btnThatClicked)
    {
    }

private:
    ToggleButton      chkbx_Name;
    TextButton        btn_Apply;
    TextButton        btn_Restore;
    String            udName;     //user defined name
    unsigned long     offset;
    std::vector<byte> usBytes;    // usser set bytes
    std::vector<byte> oldBytes;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserRecordApplyComp)

};

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

public:
    void AddUserModifyRecord(const String& name, 
        const String& offset, const String& val)
    {

    }

private:
    std::vector<UserRecordApplyComp>     recds;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MemModRecComp)

};


#endif  // CCZ_MEM_MOD_RECORD_COMPONENT_HEADER
