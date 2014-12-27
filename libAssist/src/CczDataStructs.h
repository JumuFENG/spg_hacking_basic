#pragma once

#ifndef CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H
#define CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H

#include <WinBase.h>

enum ItemProperty{
    // Nm_: Normal Sp_: Special
    Nm_Sword = 0, Sp_Sword, Nm_Spear, Sp_Spear, Nm_Arrow, Sp_Arrow, Nm_Stick, Sp_Stick, Nm_Stone, 
    Sp_Stone, Nm_Fan, Sp_Fan, Nm_Bao, Sp_Bao, Nm_Armor, Sp_Armor, Nm_Suit, Sp_Suit, 
    
    Ef_RenewHP, Ef_RenewMP, Ef_Wakeup, Ef_GetExp, Ef_WeaponExp,
    Ef_ClothExp, Ef_Atk, Ef_Vgr, Ef_Def, Ef_Erupt, Ef_Luck, Ef_GainHP, Ef_GainMP,
    Ef_GainExp, Ef_AddMv, Ef_Assault, Ef_Adverse, Ef_Confus, Ef_Poison, Ef_Paralysis,
    Ef_NoMagic, Ef_AddHtRt, Ef_StrikeBk, Ef_CritHit, Ef_FarAtk, Ef_PenetrateAtk, 
    Ef_NoAntiAtk, Ef_HouseAtk, Ef_GoOnAtk, Ef_AddFireMagic, Ef_AddWindMagic,
    Ef_SavMP, Ef_Summon, Ef_MagicSimu, Ef_AddMagicRt, Ef_RaiseAtkDef, Ef_RaisMagicDef,
    Ef_RaisAllDef, Ef_IgnorFarAtk, Ef_IgnorCritAtk, Ef_Ignor2ndAtk, Ef_LessMagicHurt,
    Ef_MPasHP, Ef_LeesFarAtk, Ef_AutoUseFrt,
    Use_4HP, Use_4MP, Use_4Confus, Use_4Poison, Use_4Paralysis, Use_4Magic, Use_4AllBad,
    Use_UpFourc, Use_UpSpirit, Use_UpLeadCap, Use_UpAgileCap, Use_UpMorale, 
    Use_UpLv, Use_UpArmLv
};

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
    L"(无)"
};

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
    byte bySpEffect;
    union{
        byte byLvOne;     // 武具表示一级初始值
        byte byAstSpValue;// 辅助表示特殊效果值
    };
    byte bySpValue;
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

#endif // CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H
