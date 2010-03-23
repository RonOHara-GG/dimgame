#pragma once

#include "Common\Utility\kpuMatrix.h"

class kppBox;
class kpgGeometryInstance;

class kppHuman
{
	enum eBodyPart
	{
		eBP_Head,
		eBP_Torso,
		eBP_LeftUpperArm,
		eBP_LeftLowerArm,
		eBP_LeftHand,
		eBP_LeftUpperLeg,
		eBP_LeftLowerLeg,
		eBP_LeftFoot,
		eBP_RightUpperArm,
		eBP_RightLowerArm,
		eBP_RightHand,
		eBP_RightUpperLeg,
		eBP_RightLowerLeg,
		eBP_RightFoot,

		eBP_Last
	};

public:
	kppHuman(float fHeight, float fShoulderWidth);
	virtual ~kppHuman(void);

	void SetRotation(float fX, float fY, float fZ)		{ m_mLocalToWorld.SetAxisRotation(fX, fY, fZ); }
	void SetPosition(float fX, float fY, float fZ)		{ m_mLocalToWorld.SetD(kpuVector(fX, fY, fZ, 1.0f)); }

	virtual void Build();

	virtual void Draw();

protected:
	float					m_fHeight;
	float					m_fShoulderWidth;

	kppBox*					m_pBodyBoxes[eBP_Last];
	kpgGeometryInstance*	m_pBodyInsts[eBP_Last];

	kpuMatrix				m_mLocalToWorld;
};
