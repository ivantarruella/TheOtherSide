#ifndef __CLASS_ENERGYRECOVER_H__
#define __CLASS_ENERGYRECOVER_H__
#pragma once

#include "LogicObject.h"

class CPlayer;
class CTexture;
class CInstanceMesh;
class CLight;

class CEnergyRecover : public CLogicObject
{
public:
	CEnergyRecover();
	CEnergyRecover(CXMLTreeNode &atts);
	~CEnergyRecover();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);
	
private:
	void DisplayUse(bool show);
	void Restart();

private:
	std::string m_UseItemFile;
	bool m_bUseActive;

	CTexture *m_OnTexture, *m_OffTexture;
	CInstanceMesh *m_Mesh;
	CLight* m_Light;

	float m_fTime;
	bool m_bStart;
	CPlayer* m_player;
	float m_fMaxEnergyRecover;
	int m_orientation;
};

#endif
