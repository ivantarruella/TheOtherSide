#ifndef __CLASS_LIFERECOVER_H__
#define __CLASS_LIFERECOVER_H__
#pragma once

#include "LogicObject.h"

class CPlayer;
class CTexture;
class CInstanceMesh;
class CLight;
class CParticleEmitter;

class CLifeRecover : public CLogicObject
{
public:
	CLifeRecover();
	CLifeRecover(CXMLTreeNode &atts);
	~CLifeRecover();

	void Update(float ElapsedTime);
	void Trigger(const std::string& action, CPlayer* player);
	
private:	
	void Restart();

private:
	CPlayer* m_player;
	CTexture *m_OnTexture, *m_OffTexture;
	CInstanceMesh *m_Mesh;
	CLight* m_Light;
	CParticleEmitter *m_Emitter, *m_Emitter2;

	float m_fTime;
	bool m_bStart;
	float m_fMaxLifeRecover;
};

#endif
