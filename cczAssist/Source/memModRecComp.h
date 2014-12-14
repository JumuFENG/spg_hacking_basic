/*
  ==============================================================================

    memModRecComp.h
    Created: 22 Dec 2014 1:11:30pm

  ==============================================================================
*/

#ifndef CCZ_MEM_MOD_RECORD_COMPONENT_HEADER
#define CCZ_MEM_MOD_RECORD_COMPONENT_HEADER

#include "JuceHeader.h"
#include "cczAssistLibLoader.h"
#include "InputStringConverter.h"
#include "cczAssistConfig.h"

using InputStringConverter::convertinputtoulong;
using InputStringConverter::convertinputbytes;

using UILayoutConverter::Set_Comp_Pos;
using UILayoutConverter::Set_Comp_Size;

//==============================================================================
/*
*/
class UserRecordApplyComp 
    : public Component
    , public MouseListener
{
public:
    UserRecordApplyComp()
    {
        Set_Comp_Size(&chkbx_Name, "UL_Rcd_NameChkbx_Rect");
        Set_Comp_Size(&btn_Apply, "UL_Rcd_Applybtn_Rect");
        Set_Comp_Size(&btn_Restore, "UL_Rcd_Restorebtn_Rect");

        chkbx_Name.setClickingTogglesState(false);
        addAndMakeVisible(chkbx_Name);
        addAndMakeVisible(btn_Apply);
        addAndMakeVisible(btn_Restore);

        addMouseListener(this, true);
    }
    ~UserRecordApplyComp(){}
    
    void InitUARecd(const String& aName, unsigned long off_, 
        std::vector<byte> bs, bool bEnable)
    {
        offset = off_;
        udName = aName;
        usBytes = bs;
        bEnableWhenStart = bEnable;
        //获取该偏移处 usBytes.size() 个原始值
        oldBytes = cczAssistLibLoader::getInstance()->GetCczMemory(
            offset, usBytes.size());

        chkbx_Name.setButtonText(udName);
        chkbx_Name.setToggleState(bEnableWhenStart, dontSendNotification);
    }

    void SetToCczMem()
    {
        if (!usBytes.empty())
        {
            if (oldBytes.empty())
            {
                oldBytes = cczAssistLibLoader::getInstance()->GetCczMemory(
                    offset, usBytes.size());
            }
            cczAssistLibLoader::getInstance()->SetCczMemory(offset, 
                (byte*)&usBytes[0], usBytes.size());
        }
    }

    void RestoreCczMem()
    {
        if (!oldBytes.empty())
        {
            cczAssistLibLoader::getInstance()->SetCczMemory(offset, 
                (byte*)&oldBytes[0], oldBytes.size());
        }
    }

    void paint(Graphics& g)
    {
//         g.setColour (Colours::grey);
//         g.drawRect (getLocalBounds(), 1);
    }

    void resized()
    {
        Set_Comp_Pos(&chkbx_Name, "UL_Rcd_NameChkbx_Rect");
        Set_Comp_Pos(&btn_Apply, "UL_Rcd_Applybtn_Rect");
        Set_Comp_Pos(&btn_Restore, "UL_Rcd_Restorebtn_Rect");
        btn_Apply.setButtonText(cczAssistLanguageSetting::getInstance()->getUIText(
            String("cczAssistMain_Text_Apply")));
        btn_Restore.setButtonText(cczAssistLanguageSetting::getInstance()->getUIText(
            String("cczAssistMain_Text_Restore")));
    }

    void mouseUp (const MouseEvent& mevt)
    {
        if (mevt.mods.isRightButtonDown() && mevt.eventComponent == &chkbx_Name)
        {
            
        }
        else if (mevt.eventComponent == &btn_Apply)
        {
            buttonClicked(&btn_Apply);
        }
        else if (mevt.eventComponent == &btn_Restore)
        {
            buttonClicked(&btn_Restore);
        }
        else if (mevt.eventComponent == &chkbx_Name)
        {
            buttonClicked(&chkbx_Name);
        }
    }

    void buttonClicked(Button* btnThatClicked)
    {
        if (btnThatClicked == &btn_Apply)
        {
            SetToCczMem();
        }
        else if (btnThatClicked == &btn_Restore)
        {
            RestoreCczMem();
        }
        if (btnThatClicked == &chkbx_Name)
        {
            chkbx_Name.setToggleState(!chkbx_Name.getToggleState(), dontSendNotification);
            cczAssistAppConfig::getInstance()->
                setUserAddItemAutoEnabled(udName, chkbx_Name.getToggleState());
        }
    }

    void UnSelectAll(bool bToggle)
    {
        chkbx_Name.setToggleState(bToggle, sendNotification);
    }

    bool isChecked()
    {
        return chkbx_Name.getToggleState();
    }

private:
    ToggleButton      chkbx_Name;
    TextButton        btn_Apply;
    TextButton        btn_Restore;
    bool              bEnableWhenStart;
    String            udName;     //user defined name
    unsigned long     offset;
    std::vector<byte> usBytes;    // usser set bytes
    std::vector<byte> oldBytes;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UserRecordApplyComp)

};

class MemModRecComp    
    : public Component
{
public:
    MemModRecComp() 
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        var itms = cczAssistAppConfig::getInstance()->getUserAddItems();
        if (itms.isArray())
        {
            for (int i = 0; i < itms.size(); ++i)
            {
                AddUserModifyRecord(itms[i].getDynamicObject()->getProperty("Name"),
                    itms[i].getDynamicObject()->getProperty("Offset"),
                    itms[i].getDynamicObject()->getProperty("Bytes"),
                    itms[i].getDynamicObject()->getProperty("AutoEnable"));
            }
        }
        Set_Comp_Size(this, "UL_Rcd_ViewPort_Main_Rect");
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
        juce::Rectangle<int> singleRcdCmp = cczAssistUILayout::getInstance()
            ->GetUILayout("UL_Rcd_Cmp_Rect");
        int ht = singleRcdCmp.getHeight();
        int x = singleRcdCmp.getX();
        int y = singleRcdCmp.getY();
        int wd = singleRcdCmp.getWidth();
        int deltaY = 0;
        for (std::vector<std::shared_ptr<UserRecordApplyComp> >::iterator 
            itRecd = recds.begin(); itRecd != recds.end(); ++itRecd)
        {
            (*itRecd)->setBounds(x, y + deltaY, wd, ht);
            deltaY += ht;
        }
        if (deltaY > getHeight())
        {
            setSize(getWidth(), y + deltaY + 1);
        }
    }

public:
    void AddUserModifyRecord(const String& name, 
        const String& offset, const String& val, bool bEnable)
    {
        std::shared_ptr<UserRecordApplyComp> ura = 
            std::shared_ptr<UserRecordApplyComp>(new UserRecordApplyComp());
        ura->InitUARecd(name, convertinputtoulong(offset), convertinputbytes(val.trim()), bEnable);
        addAndMakeVisible(ura.get());
        recds.push_back(ura);
        cczAssistAppConfig::getInstance()->
            setUserAddItem(name, offset, val.trim(), bEnable);
    }

    void UnSelectAll(bool bToggle)
    {
        for (std::vector<std::shared_ptr<UserRecordApplyComp> >::iterator
            itRcd = recds.begin(); itRcd != recds.end(); ++ itRcd)
        {
            (*itRcd)->UnSelectAll(bToggle);
        }
    }

    void ApplyAllSelected()
    {
        for (std::vector<std::shared_ptr<UserRecordApplyComp> >::iterator
            itRcd = recds.begin(); itRcd != recds.end(); ++ itRcd)
        {
            if ((*itRcd)->isChecked())
            {
                (*itRcd)->SetToCczMem();
            }
        }
    }

private:
    std::vector<std::shared_ptr<UserRecordApplyComp> >     recds;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MemModRecComp)

};


#endif  // CCZ_MEM_MOD_RECORD_COMPONENT_HEADER
