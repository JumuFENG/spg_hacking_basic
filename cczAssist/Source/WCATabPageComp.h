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
#include "SaveControlPanelComp.h"

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
        return itemsModified.at(rowNum).getItemDetailPure();
    }

    void setModify(int rowNum, const ClsItemDetail& idtl )
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

        if (rowNumber < items.size())
        {
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
    }

    void setListListener(WCAListListener* wcaLln)
    {
        listener = wcaLln;
    }

private:
    void getItems(bool bRepeat = false)
    {
        items = cczAssistLibLoader::getInstance()->GetCczItems(bRepeat);
//         if (items.empty())
//         {
//             ItemDetail id;
//             memcpy(id.szName, "Test", 17);
//             id.bIsSpItem = true;
//             id.byAstSpEff = Ef_FarAtk;
//             id.byAstSpValue = 6;
//             id.byAstFitArmy = 7;
//             id.byPrice = 255;
//             id.byIcon = 55;
//             id.byAst21ph = 255;
//             id.byAst23ph = 0;
//             items.push_back(id);
//         }
        for (std::vector<ItemDetail>::iterator it = items.begin();
            it != items.end(); ++it)
        {
            itemsModified.push_back(ClsItemDetail(*it));
        }
    }

private:
    WCAListListener*           listener;
    std::vector<ItemDetail>    items;
    std::vector<ClsItemDetail> itemsModified;
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
        radioItypeNormal.setButtonText(UILC::Get_UI_Text("cczWCA_Radio_itype_Normal"));
        radioItypeSpecia.setButtonText(UILC::Get_UI_Text("cczWCA_Radio_itype_Specia"));
        radioItypeAssist.setButtonText(UILC::Get_UI_Text("cczWCA_Radio_itype_Assist"));
        radioItypeUseitm.setButtonText(UILC::Get_UI_Text("cczWCA_Radio_itype_UseItm"));
        if (ITypeRadioGroupId == 0)
        {
            ITypeRadioGroupId = 101;
        }
        radioItypeNormal.setRadioGroupId(ITypeRadioGroupId);
        radioItypeSpecia.setRadioGroupId(ITypeRadioGroupId);
        radioItypeAssist.setRadioGroupId(ITypeRadioGroupId);
        radioItypeUseitm.setRadioGroupId(ITypeRadioGroupId);

        radioItypeNormal.addListener(this);
        radioItypeSpecia.addListener(this);
        radioItypeAssist.addListener(this);
        radioItypeUseitm.addListener(this);

        radioItypeNormal.setLookAndFeel(&lookfellv3);
        radioItypeSpecia.setLookAndFeel(&lookfellv3);
        radioItypeAssist.setLookAndFeel(&lookfellv3);
        radioItypeUseitm.setLookAndFeel(&lookfellv3);

        UILC::Set_Comp_Size(&radioItypeNormal, "UL_Radio_itype_Normal");
        UILC::Set_Comp_Size(&radioItypeSpecia, "UL_Radio_itype_Specia");
        UILC::Set_Comp_Size(&radioItypeAssist, "UL_Radio_itype_Assist");
        UILC::Set_Comp_Size(&radioItypeUseitm, "UL_Radio_itype_UseItm");

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

        saItemFitArmys.add(String(kArmyFitAll.c_str()));
        for (int i = 0; i < kArmyTypeNum; ++i)
        {
            saItemFitArmys.add(String(kArmyNamesDesc[i].c_str()));
        }

        lbl_Price__Val.setEditable(true);
        lbl_Price__Val.setColour(Label::backgroundColourId, Colours::white);
        lbl_Price__Val.addListener(this);
        combo_EffectNVal.addItemList(saItemTypeSp, 1);
        combo_TypeNamVal.addItemList(saItemTypeNm, 1);
        lbl_OriginVVal.setEditable(true);
        lbl_OriginVVal.setColour(Label::backgroundColourId, Colours::white);
        lbl_OriginVVal.addListener(this);
        lbl_LvDelt_Val.setEditable(true);
        lbl_LvDelt_Val.setColour(Label::backgroundColourId, Colours::white);
        lbl_LvDelt_Val.addListener(this);

        combo_EffectNVal.addListener(this);
        combo_EffectVVal.setEditableText(true);
        combo_EffectVVal.addListener(this);
        combo_TypeNamVal.addListener(this);

        lstModel.setListListener(this);
        lstWCA.setModel(&lstModel);
        lstWCA.setMultipleSelectionEnabled(false);
        lstWCA.selectRow(0);

        UILC::Set_Comp_Size(&lbl_WCANam_Val, "UL_WCA_lbl_WCANam_Val");
        UILC::Set_Comp_Size(&lbl_PicNum_Val, "UL_WCA_lbl_PicNum_Val");
        UILC::Set_Comp_Size(&lbl_Price__Val, "UL_WCA_lbl_Price__Val");
        UILC::Set_Comp_Size(&combo_EffectNVal, "UL_WCA_lbl_EffectNVal");
        UILC::Set_Comp_Size(&combo_EffectVVal, "UL_WCA_lbl_EffectVVal");
        UILC::Set_Comp_Size(&lbl_EffValTips, "UL_WCA_lbl_EffectVTip");
        UILC::Set_Comp_Size(&combo_TypeNamVal, "UL_WCA_lbl_TypeNamVal");
        UILC::Set_Comp_Size(&lbl_OriginVVal, "UL_WCA_lbl_OriginVVal");
        UILC::Set_Comp_Size(&lbl_LvDelt_Val, "UL_WCA_lbl_LvDelt_Val");

        btn_WriteCurSel.setButtonText(UILC::Get_UI_Text("cczAssistMain_Text_WriteIn"));
        btn_WriteCurSel.addListener(this);
        UILC::Set_Comp_Size(&btn_WriteCurSel, "UL_WCA_btn_Write_Cur");

        addAndMakeVisible(radioItypeNormal);
        addAndMakeVisible(radioItypeSpecia);
        addAndMakeVisible(radioItypeAssist);
        addAndMakeVisible(radioItypeUseitm);

        addAndMakeVisible(lstWCA);
        addAndMakeVisible(lbl_WCAName);
        addAndMakeVisible(lbl_PicNum);
        addAndMakeVisible(lbl_Price);
        addAndMakeVisible(lbl_EffectName);
        addAndMakeVisible(lbl_EffectVal);

        addAndMakeVisible(lbl_WCANam_Val);
        addAndMakeVisible(lbl_PicNum_Val);
        addAndMakeVisible(lbl_Price__Val);
        addAndMakeVisible(combo_EffectNVal);
        addAndMakeVisible(combo_EffectVVal);
        addAndMakeVisible(lbl_EffValTips);

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

        UILC::Set_Comp_Pos(&radioItypeNormal, "UL_Radio_itype_Normal");
        UILC::Set_Comp_Pos(&radioItypeSpecia, "UL_Radio_itype_Specia");
        UILC::Set_Comp_Pos(&radioItypeAssist, "UL_Radio_itype_Assist");
        UILC::Set_Comp_Pos(&radioItypeUseitm, "UL_Radio_itype_UseItm");

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
        UILC::Set_Comp_Pos(&lbl_EffValTips, "UL_WCA_lbl_EffectVTip");
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
            if (prc < 0 || prc > 255 )
            {
                prc = 255;
                LOG("Invalid price value!, Set to 255!");
            }
            if (prc == 255)
            {
                lbl_Price__Val.setText(UILC::Get_UI_Text(
                    "cczWCA_Text_NoPrice"), dontSendNotification);
            }
            tmpItemDetail.setItemPrice(prc);
        }
        else if (labelThatHasChanged == &lbl_OriginVVal)
        {
            int oriVal = lbl_OriginVVal.getText().getIntValue();
            if (oriVal >= 0 && oriVal <= 255)
            {
                tmpItemDetail.setItemOriginVal(oriVal);
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
                tmpItemDetail.setLvDeltaVal(lvDlt);
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
            ItemProperty selSp = (ItemProperty)(combo_EffectNVal.
                getSelectedItemIndex() + kItemNmTypeNum);
            byte effVal = (Valid_Enums == ClsItemDetail::valueTypeOfSpEffect((byte)selSp)) ?
                  combo_EffectVVal.getSelectedItemIndex()
                : combo_EffectVVal.getText().getIntValue();
            if (selSp == Ef_Summon)
            {
                effVal += 64;
            }
            if (effVal >= 0 && effVal <= 255)
            {
                tmpItemDetail.setItemSpecialEffValue(effVal);
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
            if (selIdx >= 0 && selIdx < combo_TypeNamVal.getNumItems() )
            {
                if (tmpItemDetail.isNormalItem() || tmpItemDetail.isSpecialItem())
                {
                    tmpItemDetail.setItemTypeValue(selIdx);
                }
                else if (tmpItemDetail.isAssistItem())
                {
                    tmpItemDetail.setFitArmyType((byte)selIdx - 1);
                }
            }
            else
            {
                tmpItemDetail.changeToAssist();
                LOG("Change The Item to Assit!");
                return;
            }
        }
        else if (comboBoxThatHasChanged == &combo_EffectNVal)
        {
            int selIdx = combo_EffectNVal.getSelectedItemIndex();
            bool bSpIsUse = radioItypeUseitm.getToggleState();
            if (selIdx == combo_EffectNVal.getNumItems() - 1)
            {
                // 选择了“无”，取消特殊效果和值
                tmpItemDetail.removeSpcialEff();
            }
            else if (selIdx >= 0 && selIdx < combo_EffectNVal.getNumItems())
            {
                ItemProperty iprty = (ItemProperty)(selIdx + kItemNmTypeNum);
                if (bSpIsUse)
                {
                    iprty = (ItemProperty)(iprty + kItemSpTypeNum);
                }
                String tips;
                combo_EffectVVal.clear(dontSendNotification);
                lbl_EffValTips.setText(String::empty, dontSendNotification);
                switch (ClsItemDetail::valueTypeOfSpEffect((byte)iprty))
                {
                case Valid_Enums:
                    saItemSpEffVal.clear();
                    switch ( (byte)iprty)
                    {
                    case Ef_FarAtk:
                        for (int i = 0; i < kFarAttackRangeNum; ++i)
                        {
                            saItemSpEffVal.add(String(kFarawayAttackDescpt[i].c_str()));
                        }
                        break;
                    case Ef_MultiAtk: 
                        for (int i = 0; i < kMultiAttkRangeNum; ++i)
                        {
                            saItemSpEffVal.add(String(kMutiAttackDescpt[i].c_str()));
                        }
                        break;
                    case Ef_Summon:
                        for (int i = 0; i < kSummonNum; ++i)
                        {
                            saItemSpEffVal.add(String(kSummonDescpt[i].c_str()));
                        }
                        break;
                    }
                    combo_EffectVVal.addItemList(saItemSpEffVal, 1);
                    break;
                case Valid_10BaseRate:
                    lbl_EffValTips.setText(UILC::Get_UI_Text(
                        "cczWCA_Label_EffValTips_10Base"), dontSendNotification);
                    break;
                case Valid_Rate:
                    lbl_EffValTips.setText(UILC::Get_UI_Text(
                        "cczWCA_Label_EffValTips_Rate"), dontSendNotification);
                    break;
                case Valid_Integer:
                    lbl_EffValTips.setText(UILC::Get_UI_Text(
                        "cczWCA_Label_EffValTips_Integ"), dontSendNotification);
                    break;
                case Valid_NoValid:
                default:
                    break;
                }
                
                tmpItemDetail.setItemSpEffct((byte)iprty);
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
                lstWCA.getSelectedRow(), tmpItemDetail.getItemDetailPure());
        }
        else if (btnThatClicked->getToggleState() && btnThatClicked != lastSelectedRadioBtn)
        {
            lastSelectedRadioBtn = btnThatClicked;
            if (btnThatClicked == &radioItypeNormal || btnThatClicked == &radioItypeSpecia)
            {
                if (btnThatClicked == &radioItypeNormal)
                {
                    combo_EffectNVal.setSelectedItemIndex(combo_EffectNVal.getNumItems());
                    combo_EffectVVal.setSelectedId(-1);
                    combo_EffectVVal.setEnabled(false);
                }
                else
                {
                    combo_EffectVVal.setEnabled(true);
                }
                lbl_TypeName.setText(UILC::Get_UI_Text("cczWCA_Label_TypeName"), dontSendNotification);
                combo_TypeNamVal.clear();
                combo_TypeNamVal.addItemList(saItemTypeNm, 1);
                addAndMakeVisible(lbl_OriginVal);
                addAndMakeVisible(lbl_OriginVVal);
                addAndMakeVisible(lbl_LvDelta);
                addAndMakeVisible(lbl_LvDelt_Val);
            }
            else if (btnThatClicked == &radioItypeAssist || btnThatClicked == &radioItypeUseitm)
            {
                combo_EffectVVal.setEnabled(true);
                lbl_OriginVal.setVisible(false);
                lbl_OriginVVal.setVisible(false);
                lbl_LvDelta.setVisible(false);
                lbl_LvDelt_Val.setVisible(false);
                tmpItemDetail.changeToAssist();
                if (btnThatClicked == &radioItypeAssist)
                {
                    lbl_TypeName.setText(UILC::Get_UI_Text("cczWCA_Label_FitArmy"), dontSendNotification);
                    combo_TypeNamVal.clear();
                    combo_TypeNamVal.addItemList(saItemFitArmys, 1);
                }
                else if (btnThatClicked == &radioItypeUseitm)
                {
                    lbl_TypeName.setVisible(false);
                    combo_TypeNamVal.setVisible(false);
                }
            }
            if (btnThatClicked == &radioItypeUseitm)
            {
                combo_EffectNVal.clear();
                combo_EffectNVal.addItemList(saItemTypeUse, 1);
            }
            else
            {
                combo_EffectNVal.clear();
                combo_EffectNVal.addItemList(saItemTypeSp, 1);
                addAndMakeVisible(lbl_TypeName);
                addAndMakeVisible(combo_TypeNamVal);
            }
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
        tmpItemDetail.setItemDetail(lstModel.getItemDetail(rowSelected));
        lbl_WCANam_Val.setText(InputStringConverter::ConvertGBKToUtf8Str(
            tmpItemDetail.getItemName(), 17), dontSendNotification);
        lbl_PicNum_Val.setText(String(tmpItemDetail.getItemIcon()), dontSendNotification);
        lbl_Price__Val.setText(tmpItemDetail.getItemPrice() == 255 ? 
            UILC::Get_UI_Text("cczWCA_Text_NoPrice")
            : String(tmpItemDetail.getItemPrice()), dontSendNotification);

        setUISpcialEffect();
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
    void setSpcialEffVal()
    {
        byte sp = tmpItemDetail.getItemSpecial();
        byte spVal = tmpItemDetail.getItemSpecialValue();
        switch (ClsItemDetail::valueTypeOfSpEffect(sp))
        {
        case Valid_10BaseRate:
        case Valid_Integer:
        case Valid_NoValid:
        case Valid_Rate:
            combo_EffectVVal.setText(String(spVal));
            break;
        case Valid_Enums:
            switch (sp)
            {
            case Ef_Summon:
                combo_EffectVVal.setSelectedItemIndex(spVal - 64);
                break;
            case Ef_FarAtk:
            case Ef_MultiAtk:
                combo_EffectVVal.setSelectedItemIndex(spVal);
                break;
            }
            break;
        }
    }

    void setUISpcialEffect()
    {
        if (tmpItemDetail.isNormalItem())
        {
            if (lastSelectedRadioBtn != &radioItypeNormal)
            {
                radioItypeNormal.setToggleState(true, dontSendNotification);
                buttonClicked(&radioItypeNormal);
            }
            combo_TypeNamVal.setSelectedItemIndex(tmpItemDetail.getItemType());
            lbl_OriginVVal.setText(String(tmpItemDetail.getItemOriginVal()), dontSendNotification);
            lbl_LvDelt_Val.setText(String(tmpItemDetail.getItemLvDelta()), dontSendNotification);
        }
        else if (tmpItemDetail.isSpecialItem())
        {
            if (lastSelectedRadioBtn != &radioItypeSpecia)
            {
                radioItypeSpecia.setToggleState(true, dontSendNotification);
                buttonClicked(&radioItypeSpecia);
            }
            combo_EffectNVal.setSelectedItemIndex(tmpItemDetail.getItemSpecial() - kItemNmTypeNum,
                sendNotificationSync);
            setSpcialEffVal();
            combo_TypeNamVal.setSelectedItemIndex(tmpItemDetail.getItemType());
            lbl_OriginVVal.setText(String(tmpItemDetail.getItemOriginVal()), dontSendNotification);
            lbl_LvDelt_Val.setText(String(tmpItemDetail.getItemLvDelta()), dontSendNotification);
        }
        else if (tmpItemDetail.isAssistItem())
        {
            if (lastSelectedRadioBtn != &radioItypeAssist)
            {
                radioItypeAssist.setToggleState(true, dontSendNotification);
                buttonClicked(&radioItypeAssist);
            }
            combo_EffectNVal.setSelectedItemIndex(
                tmpItemDetail.getItemSpecial() - kItemNmTypeNum,
                sendNotificationSync);
            setSpcialEffVal();
            byte ifit = tmpItemDetail.getAssistFitArmy() + 1;
            combo_TypeNamVal.setSelectedItemIndex( (0 <= ifit && ifit <= kArmyTypeNum + 1)
                ? ifit : 0); // 适合兵种
        }
        else if (tmpItemDetail.isAstUseItem())
        {
            if (lastSelectedRadioBtn != &radioItypeUseitm)
            {
                radioItypeUseitm.setToggleState(true, dontSendNotification);
                buttonClicked(&radioItypeUseitm);
            }
            combo_EffectNVal.setSelectedItemIndex(
                tmpItemDetail.getItemSpecial() - kItemNmTypeNum - kItemSpTypeNum,
                sendNotificationSync);
            combo_EffectVVal.setText(String(tmpItemDetail.getItemSpecialValue())); 
        }
    }

private:
    int          ITypeRadioGroupId;
    StringArray  saItemTypeNm;
    StringArray  saItemTypeSp;
    StringArray  saItemTypeUse;
    StringArray  saItemFitArmys;
    StringArray  saItemSpEffVal;
    ClsItemDetail   tmpItemDetail;
    Button* lastSelectedRadioBtn;

private:
    LookAndFeel_V3 lookfellv3;

    ListBox      lstWCA;
    WCAListModel lstModel;

    ToggleButton radioItypeNormal;
    ToggleButton radioItypeSpecia;
    ToggleButton radioItypeAssist;
    ToggleButton radioItypeUseitm;

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
    Label       lbl_EffValTips;
    ComboBox    combo_TypeNamVal;
    Label       lbl_OriginVVal;
    Label       lbl_LvDelt_Val;

    TextButton  btn_WriteCurSel;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WCATabPageComp)
};

#endif WCA_TAB_PAGE_COMPONENT_H_INCLUDED
