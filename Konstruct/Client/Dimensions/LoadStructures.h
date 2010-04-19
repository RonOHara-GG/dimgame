#include "Common/Graphics/kpgModel.h"

#ifndef LOADSTRUCTURES_H
#define LOADSTRUCTURES_H


typedef struct _EnemyLoadStructure
{
	int iNameLength;
	int iFileLength;
    char szName[128];	
    char szModel[2048];

    int iLevel;
    int iHealth;
    float fSpeed;

    int iCrushRes;
    int iSlashRes;
    int iPierceRes;
    int iMentalRes;
    int iHeatRes;
    int iColdRes;
    int iElectRes;
    int iWaterRes;
    int iAcidRes;
    int iViralRes;
    int iHolyRes;
    int iDeathRes;

    int iDamage;
    float fAggroRange;
	float fAttackRange;
    float fAttackSpeed;
    int iDamageType;


}EnemyLoadStructure;

#endif