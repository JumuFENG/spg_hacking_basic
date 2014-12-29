/*
  ==============================================================================

    WCA: Weapon Clothes Assistants

  ==============================================================================
*/
#ifndef WCA_TAB_PAGE_COMPONENT_H_INCLUDED
#define WCA_TAB_PAGE_COMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "cczAssistConfig.h"
#include "cczAssistLibLoader.h"
#include "InputStringConverter.h"
#include "memModRecComp.h"

namespace UILC = UILayoutConverter;

using InputStringConverter::convertinputtoulong;
using InputStringConverter::convertinputbytes;

class WCAListListener{
public:
    virtual ~WCAListListener()  {}
    virtual void onSelectedRowChanged(int rowSelected) = 0;

};

class WCAListModel : public ListBoxModel
    , public Component
{
public:
    WCAListModel()
        : listener(nullptr)
    {

    }

    ~WCAListModel()
    {

    }

    void initListData()
    {
        getItems(true);
    }

    int getNumRows()
    {
        if (items.empty())
        {
            getItems(false);
        }
        return items.size();
    }

    ItemDetail getItemDetail(int rowNum)
    {
        return items.at(rowNum);
    }

    void setModify(int rowNum, const ItemDetail& idtl )
    {
        if (rowNum >= items.size() || rowNum < 0)
        {
            LOG("Row Number Invalid!");
            return;
        }
        if (itemsModified.size() < items.size())
        {
            itemsModified.resize(items.size());
        }
        itemsModified[rowNum] = idtl;
    }

    void selectedRowsChanged(int lastRowSelected)
    {
        if (nullptr != listener)
        {
            listener->onSelectedRowChanged(lastRowSelected);
        }
    }

    void paintListBoxItem (int rowNumber,
        Graphics& g,
        int width, int height,
        bool rowIsSelected)
    {
        if (rowIsSelected)
            g.fillAll (findColour (TextEditor::highlightColourId));

        ItemDetail id = items[rowNumber];
        std::vector<char> szName;
        for (int i = 0; i < 17; ++i)
        {
            szName.push_back(id.szName[i]);
        }
        g.drawText(InputStringConverter::ConvertGBKToUtf8Str(szName), 
            juce::Rectangle<float>(1.0, 1.0, width, height),
            Justification::centredLeft);
        
    }

    void setListListener(WCAListListener* wcaLln)
    {
        listener = wcaLln;
    }

private:
    void getItems(bool bRepeat = false)
    {
        items = cczAssistLibLoader::getInstance()->GetCczItems(bRepeat);
    }

private:
    WCAListListener*        listener;
    std::vector<ItemDetail> items;
    std::vector<ItemDetail> itemsModified;
};

class WCATabPageComp   : public Component
    , public LabelListener
    , public ComboBoxListener
    , public ButtonListener
    , public TextEditorListener
    , public WCAListListener
    , public ChangeListener
{
public:
    //==============================================================================
    WCATabPageComp()
    {
        lstModel.setListListener(this);
        lstWCA.setModel(&lstModel);
        lstWCA.setMultipleSelectionEnabled(false);
        lstWCA.selectRow(0);
        
        lbl_WCAName.setText(UILC::Get_UI_Text("cczWCA_Label_WCAName"), dontSendNotification);
        lbl_WCAName.setJustificationType(Justification::centredRight);
        lbl_PicNum.setText(UILC::Get_UI_Text("cczWCA_Label_PicNum"), dontSendNotification);
        lbl_PicNum.setJustificationType(Justification::centredRight);
        lbl_Price.setText(UILC::Get_UI_Text("cczWCA_Label_Price"), dontSendNotification);
        lbl_Price.setJustificationType(Justification::centredRight);
        lbl_EffectName.setText(UILC::Get_UI_Text("cczWCA_Label_EffectName"), dontSendNotification);
        lbl_EffectName.setJustificationType(Justification::centredRight);
        lbl_EffectVal.setText(UILC::Get_UI_Text("cczWCA_Label_EffectVal"), dontSendNotification);
        lbl_EffectVal.setJustificationType(Justification::centredRight);
        lbl_TypeName.setText(UILC::Get_UI_Text("cczWCA_Label_TypeName"), dontSendNotification);
        lbl_TypeName.setJustificationType(Justification::centredRight);
        lbl_OriginVal.setText(UILC::Get_UI_Text("cczWCA_Label_OriginVal"), dontSendNotification);
        lbl_OriginVal.setJustificationType(Justification::centredRight);
        lbl_LvDelta.setText(UILC::Get_UI_Text("cczWCA_Label_LvDelta"), dontSendNotification);
        lbl_LvDelta.setJustificationType(Justification::centredRight);

        UILC::Set_Comp_Size(&lstWCA, "UL_WCA_lstbox_WCAMain");
        UILC::Set_Comp_Size(&lbl_WCAName, "UL_WCA_lbl_WCAName");
        UILC::Set_Comp_Size(&lbl_PicNum, "UL_WCA_lbl_PicNum");
        UILC::Set_Comp_Size(&lbl_Price, "UL_WCA_lbl_Price");
        UILC::Set_Comp_Size(&lbl_EffectName, "UL_WCA_lbl_EffectName");
        UILC::Set_Comp_Size(&lbl_EffectVal, "UL_WCA_lbl_EffectVal");
        UILC::Set_Comp_Size(&lbl_TypeName, "UL_WCA_lbl_TypeName");
        UILC::Set_Comp_Size(&lbl_OriginVal, "UL_WCA_lbl_OriginVal");
        UILC::Set_Comp_Size(&lbl_LvDelta, "UL_WCA_lbl_LvDelta");

        for (int i = 0; i < kItemNmTypeNum; ++i)
        {
            saItemTypeNm.add(String(kItemProperName[i].c_str()));
        }
        saItemTypeNm.add(String(kItemProperUnknown.c_str()));
        for (int i = 0; i < kItemSpTypeNum; ++i)
        {
            saItemTypeSp.add(String(kItemProperName[i + kItemNmTypeNum].c_str()));
        }
        saItemTypeSp.add(String(kItemProperUnknown.c_str()));
        for (int i = 0; i < kItemUseTypeNum; ++i)
        {
            saItemTypeUse.add(String(kItemProperName[
                i + kItemNmTypeNum + kItemSpTypeNum].c_str()));
        }
        saItemTypeUse.add(String(kItemProperUnknown.c_str()));

        lbl_Price__Val.setEditable(true);
        lbl_Price__Val.addListener(this);
        combo_EffectNVal.addItemList(saItemTypeSp, 1);
        combo_TypeNamVal.addItemList(saItemTypeNm, 1);
        lbl_OriginVVal.setEditable(true);
        lbl_OriginVVal.addListener(this);
        lbl_LvDelt_Val.setEditable(true);
        lbl_LvDelt_Val.addListener(this);

        combo_EffectNVal.addListener(this);
        combo_EffectVVal.setEditableText(true);
        combo_EffectVVal.addListener(this);
        combo_TypeNamVal.addListener(this);

        UILC::Set_Comp_Size(&lbl_WCANam_Val, "UL_WCA_lbl_WCANam_Val");
        UILC::Set_Comp_Size(&lbl_PicNum_Val, "UL_WCA_lbl_PicNum_Val");
        UILC::Set_Comp_Size(&lbl_Price__Val, "UL_WCA_lbl_Price__Val");
        UILC::Set_Comp_Size(&combo_EffectNVal, "UL_WCA_lbl_EffectNVal");
        UILC::Set_Comp_Size(&combo_EffectVVal, "UL_WCA_lbl_EffectVVal");
        UILC::Set_Comp_Size(&combo_TypeNamVal, "UL_WCA_lbl_TypeNamVal");
        UILC::Set_Comp_Size(&lbl_OriginVVal, "UL_WCA_lbl_OriginVVal");
        UILC::Set_Comp_Size(&lbl_LvDelt_Val, "UL_WCA_lbl_LvDelt_Val");

        btn_WriteCurSel.setButtonText(UILC::Get_UI_Text("cczAssistMain_Text_WriteIn"));
        btn_WriteCurSel.addListener(this);
        UILC::Set_Comp_Size(&btn_WriteCurSel, "UL_WCA_btn_Write_Cur");

        addAndMakeVisible(lstWCA);
        addAndMakeVisible(lbl_WCAName);
        addAndMakeVisible(lbl_PicNum);
        addAndMakeVisible(lbl_Price);
        addAndMakeVisible(lbl_EffectName);
        addAndMakeVisible(lbl_EffectVal);
        addAndMakeVisible(lbl_TypeName);
        addAndMakeVisible(lbl_OriginVal);
        addAndMakeVisible(lbl_LvDelta);

        addAndMakeVisible(lbl_WCANam_Val);
        addAndMakeVisible(lbl_PicNum_Val);
        addAndMakeVisible(lbl_Price__Val);
        addAndMakeVisible(combo_EffectNVal);
        addAndMakeVisible(combo_EffectVVal);
        addAndMakeVisible(combo_TypeNamVal);
        addAndMakeVisible(lbl_OriginVVal);
        addAndMakeVisible(lbl_LvDelt_Val);

        addAndMakeVisible(btn_WriteCurSel);
    }

    ~WCATabPageComp()
    {
    }

    void paint (Graphics& g)
    {
        g.fillAll (Colour (0xffeeddff));

        g.setFont (Font (16.0f));
        g.setColour (Colours::black);
    }

    void resized()
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        UILC::Set_Comp_Pos(&lstWCA, "UL_WCA_lstbox_WCAMain");
        UILC::Set_Comp_Pos(&lbl_WCAName, "UL_WCA_lbl_WCAName");
        UILC::Set_Comp_Pos(&lbl_PicNum,"UL_WCA_lbl_PicNum");
        UILC::Set_Comp_Pos(&lbl_Price, "UL_WCA_lbl_Price");
        UILC::Set_Comp_Pos(&lbl_EffectName, "UL_WCA_lbl_EffectName");
        UILC::Set_Comp_Pos(&lbl_EffectVal, "UL_WCA_lbl_EffectVal");
        UILC::Set_Comp_Pos(&lbl_TypeName, "UL_WCA_lbl_TypeName");
        UILC::Set_Comp_Pos(&lbl_OriginVal, "UL_WCA_lbl_OriginVal");
        UILC::Set_Comp_Pos(&lbl_LvDelta, "UL_WCA_lbl_LvDelta");

        UILC::Set_Comp_Pos(&lbl_WCANam_Val, "UL_WCA_lbl_WCANam_Val");
        UILC::Set_Comp_Pos(&lbl_PicNum_Val, "UL_WCA_lbl_PicNum_Val");
        UILC::Set_Comp_Pos(&lbl_Price__Val, "UL_WCA_lbl_Price__Val");
        UILC::Set_Comp_Pos(&combo_EffectNVal, "UL_WCA_lbl_EffectNVal");
        UILC::Set_Comp_Pos(&combo_EffectVVal, "UL_WCA_lbl_EffectVVal");
        UILC::Set_Comp_Pos(&combo_TypeNamVal, "UL_WCA_lbl_TypeNamVal");
        UILC::Set_Comp_Pos(&lbl_OriginVVal, "UL_WCA_lbl_OriginVVal");
        UILC::Set_Comp_Pos(&lbl_LvDelt_Val, "UL_WCA_lbl_LvDelt_Val");

        UILC::Set_Comp_Pos(&btn_WriteCurSel, "UL_WCA_btn_Write_Cur");
    }

    void initData()
    {
        lstModel.initListData();
        int lstSelect = lstWCA.getSelectedRow();
        lstWCA.updateContent();
        lstWCA.selectRow(lstSelect);
        onSelectedRowChanged(lstSelect);
    }

    void labelTextChanged (Label* labelThatHasChanged)
    {
        if (labelThatHasChanged == &lbl_Price__Val)
        {
            int prc = lbl_Price__Val.getText().getIntValue();
            if (prc >= 0 && prc <= 255 )
            {
                tmpItemDetail.byPrice = prc;
            }
            else
            {
                tmpItemDetail.byPrice = 255;
                LOG("Invalid price value!, Set to 255!");
            }
            if (tmpItemDetail.byPrice == 255)
            {
                lbl_Price__Val.setText(UILC::Get_UI_Text(
                    "cczWCA_Text_NoPrice"), dontSendNotification);
            }
        }
        else if (labelThatHasChanged == &lbl_OriginVVal)
        {
            int oriVal = lbl_OriginVVal.getText().getIntValue();
            if (oriVal >= 0 && oriVal <= 255)
            {
                tmpItemDetail.byLvOne = oriVal;
            }
            else
            {
                lbl_OriginVVal.setText(String(0), dontSendNotification);
                LOG("Invalid Origin value!, 0 - 255!");
                return;
            }
        }
        else if (labelThatHasChanged == &lbl_LvDelt_Val)
        {
            int lvDlt = lbl_LvDelt_Val.getText().getIntValue();
            if (lvDlt >= 0 && lvDlt <= 255)
            {
                tmpItemDetail.byLvInc = lvDlt;
            }
            else
            {
                lbl_LvDelt_Val.setText(String(0), dontSendNotification);
                LOG("Invalid Level Increase value!, 0 - 255!");
                return;
            }
        }
        else
        {
            LOG("The labelThatHasChanged isn't listened");
            return;
        }
        lstModel.setModify(lstWCA.getSelectedRow(), tmpItemDetail);
    }

    void comboBoxChanged (ComboBox* comboBoxThatHasChanged)
    {
        if (comboBoxThatHasChanged == &combo_EffectVVal)
        {
            int effVal = combo_EffectVVal.getText().getIntValue();
            if (effVal >= 0 && effVal <= 255)
            {
                tmpItemDetail.bySpValue = effVal;
            }
            else
            {
                combo_EffectVVal.setText(String("0"), dontSendNotification);
                LOG("Invalid Special Effect Value");
                return;
            }
        }
        else if (comboBoxThatHasChanged == &combo_TypeNamVal)
        {
            int selIdx = combo_TypeNamVal.getSelectedItemIndex();
            if (selIdx >= 0 && selIdx < 18)
            {
                tmpItemDetail.byType = selIdx;
            }
            else
            {
                LOG("Type should be [0, 18)");
                return;
            }
        }
        else if (comboBoxThatHasChanged == &combo_EffectNVal)
        {
            int selIdx = combo_EffectNVal.getSelectedItemIndex();
            if (selIdx >= 0 && selIdx < kItemSpTypeNum)
            {
                tmpItemDetail.bySpEffect = selIdx + kItemNmTypeNum;
            }
            else
            {
                LOG("Special Effect idx Wrong!");
                return;
            }
        }
        else
        {
            LOG("The comboBoxThatHasChanged isn't listened");
            return;
        }
        lstModel.setModify(lstWCA.getSelectedRow(), tmpItemDetail);
    }

    void editorShown (Label* , TextEditor& edt) 
    {
        edt.setColour(TextEditor::backgroundColourId, findColour(TextEditor::backgroundColourId));
        edt.setColour(TextEditor::outlineColourId, findColour(TextEditor::outlineColourId));
    }

    void buttonClicked(Button* btnThatClicked)
    {
        if (btnThatClicked == &btn_WriteCurSel)
        {
            cczAssistLibLoader::getInstance()->WriteItemToCcz(
                lstWCA.getSelectedRow(), tmpItemDetail);
        }
    }

    void textEditorTextChanged (TextEditor& edt)
    {

    }

    void onSelectedRowChanged(int rowSelected)
    {
        if (rowSelected < 0 || rowSelected >= lstModel.getNumRows())
        {
            LOG(String("Wrong selected Row id: ") + String(rowSelected));
            return;
        }
        tmpItemDetail = lstModel.getItemDetail(rowSelected);
        lbl_WCANam_Val.setText(InputStringConverter::ConvertGBKToUtf8Str(tmpItemDetail.szName, 
            17), dontSendNotification);
        lbl_PicNum_Val.setText(String(tmpItemDetail.byIcon), dontSendNotification);
        lbl_Price__Val.setText(tmpItemDetail.byPrice == 255 ? 
            UILC::Get_UI_Text("cczWCA_Text_NoPrice")
            : String(tmpItemDetail.byPrice), dontSendNotification);
        if (tmpItemDetail.byType < Ef_RenewHP)
        {
            // 武具
            if (tmpItemDetail.byType % 2 == 0)
            {
                combo_EffectNVal.setSelectedId(combo_EffectNVal.getNumItems());
                combo_EffectVVal.setSelectedId(-1);
                combo_EffectVVal.setEnabled(false);
            }
            else
            {
                combo_EffectNVal.setSelectedItemIndex(tmpItemDetail.bySpEffect - kItemNmTypeNum);
                combo_EffectVVal.setText(String(tmpItemDetail.bySpValue)); // TODO:改为描述
                combo_EffectVVal.setEnabled(true);
            }
            combo_TypeNamVal.setEnabled(true);
            combo_TypeNamVal.setSelectedItemIndex(tmpItemDetail.byType);
            lbl_OriginVVal.setText(String(tmpItemDetail.byLvOne), dontSendNotification);
            lbl_LvDelt_Val.setText(String(tmpItemDetail.byLvInc), dontSendNotification);
        }
        else 
        {
            if(tmpItemDetail.byAstSpEff < Use_4HP)
            {
                // 辅助
                combo_EffectNVal.clear(dontSendNotification);
                combo_EffectNVal.addItemList(saItemTypeSp, 1);
                combo_EffectNVal.setSelectedItemIndex(tmpItemDetail.byAstSpEff - kItemNmTypeNum);
            }
            else
            {
                // 消耗品
                combo_EffectNVal.clear(dontSendNotification);
                combo_EffectNVal.addItemList(saItemTypeUse, 1);
                combo_EffectNVal.setSelectedItemIndex(
                    tmpItemDetail.byAstSpEff - kItemNmTypeNum - kItemSpTypeNum);
            }
            combo_EffectVVal.setText(String(tmpItemDetail.byAstSpValue)); // TODO:改为描述
            combo_EffectVVal.setEnabled(true);
            combo_TypeNamVal.setSelectedId(-1);
            combo_TypeNamVal.setEnabled(false);
            lbl_OriginVVal.setText(String::empty, dontSendNotification);
            lbl_LvDelt_Val.setText(String::empty, dontSendNotification);
        }
    }

    void changeListenerCallback(ChangeBroadcaster* source)
    {
        MainTabComponent* mainTab = (MainTabComponent*) source;
        if (mainTab->isCczRunning())
        {
            Sleep(3000);
            initData();
        }
    }

private:
    StringArray  saItemTypeNm;
    StringArray  saItemTypeSp;
    StringArray  saItemTypeUse;
    ItemDetail   tmpItemDetail;

private:
    ListBox      lstWCA;
    WCAListModel lstModel;

    Label       lbl_WCAName;
    Label       lbl_PicNum;
    Label       lbl_Price;
    Label       lbl_EffectName;
    Label       lbl_EffectVal;
    Label       lbl_TypeName;
    Label       lbl_OriginVal;
    Label       lbl_LvDelta;

    Label       lbl_WCANam_Val;
    Label       lbl_PicNum_Val;
    Label       lbl_Price__Val;
    ComboBox    combo_EffectNVal;
    ComboBox    combo_EffectVVal;
    ComboBox    combo_TypeNamVal;
    Label       lbl_OriginVVal;
    Label       lbl_LvDelt_Val;

    TextButton  btn_WriteCurSel;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WCATabPageComp)
};

#endif WCA_TAB_PAGE_COMPONENT_H_INCLUDED
