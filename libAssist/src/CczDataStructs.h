#pragma once

#ifndef CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H
#define CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H

#include <WinBase.h>
#include <assert.h>

enum ItemProperty{
    // Nm_: Normal Sp_: Special
    Nm_Sword = 0, Sp_Sword, Nm_Spear, Sp_Spear, Nm_Arrow, Sp_Arrow, Nm_Stick, Sp_Stick, Nm_Stone, 
    Sp_Stone, Nm_Fan, Sp_Fan, Nm_Bao, Sp_Bao, Nm_Armor, Sp_Armor, Nm_Suit, Sp_Suit, 
    
    Ef_RenewHP, Ef_RenewMP, Ef_Wakeup, Ef_GetExp, Ef_WeaponExp,
    Ef_ClothExp, Ef_Atk, Ef_Vgr, Ef_Def, Ef_Erupt, Ef_Luck, Ef_GainHP, Ef_GainMP,
    Ef_GainExp, Ef_AddMv, Ef_Assault, Ef_Adverse, Ef_Confus, Ef_Poison, Ef_Paralysis,
    Ef_NoMagic, Ef_AddHtRt, Ef_StrikeBk, Ef_CritHit, Ef_FarAtk, Ef_MultiAtk, 
    Ef_NoAntiAtk, Ef_HouseAtk, Ef_GoOnAtk, Ef_AddFireMagic, Ef_AddWindMagic,
    Ef_SavMP, Ef_Summon, Ef_MagicSimu, Ef_AddMagicRt, Ef_RaiseAtkDef, Ef_RaisMagicDef,
    Ef_RaisAllDef, Ef_IgnorFarAtk, Ef_IgnorCritAtk, Ef_Ignor2ndAtk, Ef_LessMagicHurt,
    Ef_MPasHP, Ef_LeesFarAtk, Ef_AutoUseFrt,
    Use_4HP, Use_4MP, Use_4Confus, Use_4Poison, Use_4Paralysis, Use_4Magic, Use_4AllBad,
    Use_UpFourc, Use_UpSpirit, Use_UpLeadCap, Use_UpAgileCap, Use_UpMorale, 
    Use_UpLv, Use_UpArmLv
};

const static int kItemNmTypeNum = 18;
const static int kItemSpTypeNum = 45;
const static int kItemUseTypeNum = 14;
const static std::wstring kItemProperUnknown = L"无";

const static unsigned long kItemsOffset = 0xA1140;
const static int kItemsNum = 104;

const std::wstring kItemProperName[] ={
    L"普通剑",
    L"特殊剑",
    L"普通枪",
    L"特殊枪",
    L"普通弓",
    L"特殊弓",
    L"普通棍",
    L"特殊棍",
    L"普通投石机",
    L"特殊投石机",
    L"普通扇",
    L"特殊扇",
    L"普通宝剑",
    L"特殊宝剑",
    L"普通铠甲",
    L"特殊铠甲",
    L"普通衣服",
    L"特殊衣服",
    L"每回合恢复HP(百分数)",
    L"每回合恢复MP(百分数)",
    L"每回合恢复状态",
    L"每回合获得Exp(整数)",
    L"每回合获得武器Exp(整数)",
    L"每回合获得道具Exp(整数)",
    L"辅助攻击力(整数)",
    L"辅助精神力(整数)",
    L"辅助防御力(整数)",
    L"辅助爆发力(整数)",
    L"辅助运气(整数)",
    L"辅助HP增加(整数)",
    L"辅助MP增加(整数)",
    L"辅助获得Exp(十分数)",
    L"辅助移动力(整数)",
    L"突击移动",
    L"恶路移动",
    L"混乱攻击",
    L"中毒攻击",
    L"麻痹攻击",
    L"封杀策略",
    L"辅助攻击命中(百分数)",
    L"反击后反击",
    L"致命一击攻击",
    L"远距离攻击(没羽箭)",
    L"穿透效果",
    L"无反击攻击",
    L"骑马攻击(百分数)",
    L"引导攻击",
    L"辅助火类策略(百分数)",
    L"辅助风类策略(百分数)",
    L"节约MP(百分数)",
    L"召唤策略",
    L"策略模仿",
    L"辅助策略命中(百分数)",
    L"辅助攻击防御(百分数)",
    L"辅助策略防御(百分数)",
    L"辅助全防御",
    L"防御远程攻击",
    L"防止致命一击",
    L"防止二次攻击",
    L"减轻策略损伤(百分数)",
    L"mp辅助防御",
    L"减轻远距离损伤(百分数)",
    L"自动使用道具",
    L"获取HP(整数)",
    L"获取MP(整数)",
    L"治疗混乱",
    L"治疗中毒",
    L"治疗麻痹",
    L"治疗禁咒",
    L"治疗一切异常",
    L"武力上升(整数)",
    L"智力上升(整数)",
    L"统帅上升(整数)",
    L"敏捷上升(整数)",
    L"士气上升(整数)",
    L"等级上升",
    L"兵种上升",
};

const std::wstring kFarawayAttackDescpt[] =
{
    L"轻骑兵",
    L"步兵",
    L"弓兵",
    L"弩兵",
    L"连弩兵",
    L"爆炎"
    L"没羽箭",
    L"炮车",
    L"重炮车"
    L"弓骑兵",
    L"全屏",
    L"无",
    L"骑兵+弩兵",
    L"骑兵+连弩兵",
    L"小方框",
    L"大方框"
};

const std::wstring kMutiAttackDescpt[] =
{
    L"正常",
    L"十字",
    L"九宫",
    L"没羽箭",
    L"穿透2格",
    L"穿透6格",
    L"片伤" // 骑兵 + 连弩兵
};

const std::wstring kDescFitArmy[] = {
    L"群雄",
    L"步兵",// 弩兵 骑兵 弓骑兵 炮车 武术家 贼兵 策士 风水士 道士 
    L""     // 11骑马参谋 舞娘 西凉骑兵 黄巾军 海盗 训熊师 训虎师 都督
    //19 咒术士 仙人 辎重队 粮草队 木人
    //24 土偶 皇帝 百姓
    //255 全部可用
}

#pragma pack(1)

#define NAME_COPY_LEN 0x11

typedef struct _MapPos
{
    byte byX;
    byte byY;
}MapPos, *PMapPos;

typedef struct _FiveDimBY
{
    byte byAtk;
    byte byDef;
    byte byPow;
    byte byDex;
    byte byCrt;
}FiveDimBY, *PFiveDimBY;

typedef struct _FiveDimWD
{
    WORD wAtk;
    WORD wDef;
    WORD wPow;
    WORD wDex;
    WORD wCrt;
}FiveDimWD, *PFiveDimWD;

enum EArmyFlg{EPlyFlg=0, EFrdFlg, EEmyFlg};

typedef struct _TroopStatus
{
    WORD wNameIdx;
    WORD wNonuse;
    byte byStageIdx;
    byte byArmyFlg;//0:player, 1:friend, 2:enemy
    MapPos mapPos;
    byte byAIStageIdx;
    MapPos mapAIPos;
    byte byAlwaysFF;
    byte byLifeCode;//2:alive, 3:dead
    byte byPicCode;//0~6
    byte byAICode;//0~7, 7:player
    byte byDirection;
    DWORD dwHP;
    DWORD dwMP;
    FiveDimBY status;
    byte byMov;
    char szRest[6];
} TroopStatus, *PTroopStatus;

typedef struct _ItemInfo
{
    byte byIdx;
    byte byLv;
    byte byExp;
}ItemInfo, *PItemInfo;

typedef struct _BaseData
{
    WORD wCodeIdx;//from 0x1000
    WORD wClothIdx;
    WORD wPicIdx;
    WORD wNonuse;
    char szName[9];
    FiveDimWD fdDW;
    byte bySzEnd;
    DWORD dwHP;
    byte byMP;
    FiveDimBY base;
    byte byUnknown;
    byte byFightCount;
    byte byWinCount;
    byte byRetreatCount;
    byte byArmyFlg;//0:player, FF:enemy
    ItemInfo infoSelf;
    ItemInfo infoWeapon;
    ItemInfo infoArmour;
    ItemInfo infoAssist;
    char szRest[NAME_COPY_LEN];
}BaseData, *PBaseData;

typedef struct _ItemDetail
{
    char szName[17];
    union{
        byte byType;     // 武具表示类型
        byte byAstSpEff; // 辅助表示特殊效果
    };
    byte byPrice;//0xFF means not-for-sale
    byte byIcon;
    union{
        byte bySpEffect;
        byte byAst21ph;   // 辅助该项无意义，均为FF
    };
    union{
        byte byLvOne;     // 武具表示一级初始值
        byte byAstSpValue;// 辅助表示特殊效果值
    };
    union{
        byte bySpValue;
        byte byAst23ph;   // 辅助该项无意义，均为00
    };
    union{
        byte byLvInc;     // 武具表示升级成长值
        byte byAstFitArmy;// 辅助表示适用兵种
    };
    bool bIsSpItem;//01:yes
} ItemDetail, *PItemDetail;


typedef struct _JobData
{
    byte byMov;
    byte byAtkAear;
    FiveDimBY fdBY;//CBASX+
    byte byHP;
    byte byMP;
    bool aIsSuit[18];
}JobData, PJobData;

#pragma pack()

enum CczItemType{
    Item_Unknown,
    Item_Generic,   // 武具
    Item_Assist,    // 辅助
    Item_Use        // 消耗品
};

enum CczValidSpEffVal{
    Valid_NoValid,
    Valid_Rate,
    Valid_10BaseRate,  // 10的倍数，孟德新书 1.5倍，其值为15
    Valid_Integer,
    Valid_Enums
};

class ClsItemDetail{
public:
    ClsItemDetail()
    {

    }

    ClsItemDetail(const ItemDetail& idtl)
    {
        setItemDetail(idtl);
    }

    ~ClsItemDetail()
    {

    }

    ItemDetail getItemDetailPure()
    {
        if (m_iType == Item_Unknown)
        {
            if ( m_byType >= Nm_Sword && m_byType <= Sp_Suit)
            {
                m_iType = Item_Generic;
            }
            else
            {
                if (m_bySpEffect >= Ef_RenewHP && m_bySpEffect <= Ef_AutoUseFrt)
                {
                    m_iType = Item_Assist;
                }
                else if (m_bySpEffect >= Use_4HP && m_bySpEffect <= Use_UpArmLv)
                {
                    m_iType = Item_Use;
                }
            }
        }
        ItemDetail rtIdtl = {0};
        memcpy(rtIdtl.szName, m_szName, 17);
        rtIdtl.byIcon = m_byIcon;
        rtIdtl.byPrice = m_byPrice;
        rtIdtl.bIsSpItem = m_bIsSpecial;
        if (m_iType == Item_Generic)
        {
            rtIdtl.byType = m_byType;
            rtIdtl.bySpEffect = m_bySpEffect;
            rtIdtl.bySpValue = m_bySpValue;
            rtIdtl.byLvOne = m_byLvOne;
            rtIdtl.byLvInc = m_byLvInc;
        }
        else
        {
            rtIdtl.byAstSpEff = m_bySpEffect;
            rtIdtl.byAstSpValue = m_bySpValue;
            rtIdtl.byAstFitArmy = m_byLvInc;
            rtIdtl.byAst21ph = 0xFF;
            rtIdtl.byAst23ph = 0x00;
        }
        return rtIdtl;
    }

    void setItemDetail(ItemDetail idtl)
    {
        m_bIsSpecial = idtl.bIsSpItem;
        m_byIcon = idtl.byIcon;
        m_byPrice = idtl.byPrice;
        memcpy(m_szName, idtl.szName, 17);

        byte tp = idtl.byType;
        if ( tp >= Nm_Sword && tp <= Sp_Suit)
        {
            m_iType = Item_Generic;
        }
        else
        {
            tp = idtl.byAstSpEff;
            if (tp >= Ef_RenewHP && tp <= Ef_AutoUseFrt)
            {
                m_iType = Item_Assist;
            }
            else if (tp >= Use_4HP && tp <= Use_UpArmLv)
            {
                m_iType = Item_Use;
            }
        }

        if (m_iType == Item_Generic)
        {
            m_byType = idtl.byType;
            m_bySpEffect = idtl.bySpEffect;
            m_bySpValue = idtl.bySpValue;
            m_byLvOne = idtl.byLvOne;
            m_byLvInc = idtl.byLvInc;
        }
        else if (m_iType == Item_Use || m_iType == Item_Assist)
        {
            m_byType = 0;
            m_byLvOne = 0;
            m_bySpEffect = idtl.byAstSpEff;
            m_bySpValue = idtl.byAstSpValue;
            m_byLvInc = idtl.byAstFitArmy;
        }
    }

    void changeToGeneric(byte tp)
    {
        assert( Nm_Sword <= tp && tp <= Sp_Suit);
        setItemTypeValue(tp);
    }

    void changeToAssist()
    {
        if (m_bySpEffect >= Ef_RenewHP && m_bySpEffect <= Ef_AutoUseFrt)
        {
            m_iType = Item_Assist;
        }
        else if (m_bySpEffect >= Use_4HP && m_bySpEffect <= Use_UpArmLv)
        {
            m_iType = Item_Use;
        }
    }

    void setItemTypeValue(byte val)
    {
        // 武具才调用该接口，否则调用别的接口
        assert (val >= Nm_Sword && val <= Sp_Suit );
        m_byType = val;
        m_iType = Item_Generic;
    }

    void setItemSpEffct(byte sp)
    {
        assert (sp >= Ef_RenewHP && sp <= Use_UpArmLv);
        assert (m_iType != Item_Unknown);
        m_bySpEffect = sp;
    }

    void setItemSpecialEffValue(byte spval)
    {
        assert(m_iType != Item_Unknown);
        m_bySpValue = spval;
    }

    void setItemPrice(byte prc)
    {
        m_byPrice = prc;
    }

    void setItemOriginVal(byte val)
    {
        m_byLvOne = val;
    }

    void setIsSpecialItem(bool isp)
    {
        m_bIsSpecial = isp;
    }

    void setIconPicId(byte icid)
    {
        m_byIcon = icid;
    }

    void setLvDeltaVal(bool val)
    {
        m_byLvInc = val;
    }

    void setFitArmyType(byte val)
    {
        setLvDeltaVal(val);
    }

    const char* getItemName() { return m_szName; }
    byte getItemIcon() { return m_byIcon; }
    byte getItemPrice() { return m_byPrice; }
    byte getItemSpecial() { return m_bySpEffect; }
    byte getItemSpecialValue() { return m_bySpValue; }
    byte getItemType() { return m_byType; }
    byte getItemOriginVal() { return m_byLvOne; }
    byte getItemLvDelta() { return m_byLvInc; }
    byte getAssistFitArmy() { return m_byLvInc; }

    bool isGenericItem(){ return m_iType == Item_Generic; }
    bool isAssistItem() { return m_iType == Item_Assist; }
    bool isAstUseItem() { return m_iType == Item_Use; }

public:
    static CczValidSpEffVal valueTypeOfSpEffect(byte spEff)
    {
        switch (spEff)
        {
        case Ef_GainExp:
            return Valid_10BaseRate;

        case Ef_Wakeup: case Ef_Assault:   case Ef_Adverse: 
        case Ef_Confus: case Ef_Poison:    case Ef_Paralysis:
        case Ef_NoMagic:case Ef_GoOnAtk:   case Ef_MagicSimu:
        case Ef_MPasHP: case Ef_NoAntiAtk: case Ef_IgnorFarAtk:
        case Ef_CritHit:case Ef_StrikeBk:  case Ef_IgnorCritAtk:
        case Ef_Ignor2ndAtk:
        case Use_4Confus: case Use_4Poison: case Use_4Paralysis:
        case Use_4Magic:  case Use_4AllBad: 
        case Ef_AutoUseFrt: case Use_UpLv: case Use_UpArmLv:
            return Valid_NoValid;

        case Ef_GetExp: case Ef_WeaponExp: case Ef_ClothExp:
        case Ef_Atk:    case Ef_Vgr:       case Ef_Def:
        case Ef_Erupt:  case Ef_Luck:      case Ef_GainHP:
        case Ef_GainMP: case Ef_AddMv: 
        case Use_4HP:       case Use_4MP:      case Use_UpFourc:
        case Use_UpSpirit:  case Use_UpLeadCap:case Use_UpAgileCap:
        case Use_UpMorale:
            return Valid_Integer;

        case Ef_RenewHP: case Ef_RenewMP:     case Ef_AddHtRt:
        case Ef_HouseAtk:case Ef_AddFireMagic:case Ef_AddWindMagic:
        case Ef_SavMP:   case Ef_AddMagicRt:  case Ef_RaiseAtkDef:
        case Ef_RaisMagicDef:
        case Ef_RaisAllDef:   case Ef_LessMagicHurt: case Ef_LeesFarAtk:
            return Valid_Rate;

        case Ef_FarAtk:  case Ef_MultiAtk: 
        case Ef_Summon:
            return Valid_Enums;
        }
    }

    static byte getValidSpValue(byte spEff, byte val)
    {
        CczValidSpEffVal cvlid = valueTypeOfSpEffect(spEff);
        switch (cvlid)
        {
        case Valid_Rate:
            return (0 <= val && val <= 100) ? val : 100;
        case Valid_Enums:
            {
                if (spEff == Ef_Summon)
                {
                    return (64 <= val && val <= 67) ? val : 64;
                }
                if (spEff == Ef_FarAtk)
                {
                    return (val >= 16) ? 11 : val;
                }
                if (spEff == Ef_MultiAtk)
                {
                    return (val > 6) ? 0 : val;
                }
            }
        case Valid_10BaseRate:
        case Valid_NoValid:
        case Valid_Integer:
            return val;
        }
    }

private:
    char    m_szName[17];
    byte    m_byType;
    byte    m_byPrice;
    byte    m_byIcon;
    byte    m_bySpEffect;
    byte    m_bySpValue;
    byte    m_byLvOne;
    byte    m_byLvInc;      // 升级成长值，辅助表示适合兵种
    bool    m_bIsSpecial;
    CczItemType m_iType;
};

#endif // CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H
