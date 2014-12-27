#pragma once

#ifndef CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H
#define CCZ_ASSIST_DATASTRUCTURE_INCLUDE_H

#include <WinBase.h>
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
    byte byType;
    byte byPrice;//0xFF means not-for-sale
    byte byIcon;
    byte bySpEffect;
    byte byLvOne;
    byte bySpValue;
    byte byLvInc;
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
