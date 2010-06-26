#ifndef LOADSTRUCTURES_H
#define LOADSTRUCTURES_H

class kpgModel;

typedef struct _PlayerLoadStructure
{



}PlayerLoadStructure;

typedef struct _EnemyLoadStructure
{
	int iNameLength;
	int iFileLength;
    char* pszName;//[128];	

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

    int	  iDamage;
    float fAggroRange;
	float fAttackRange;
    float fAttackSpeed;
    int iDamageType;
	kpgModel* pModel;
	kpgModel* pCollision;


}EnemyLoadStructure;

#endif