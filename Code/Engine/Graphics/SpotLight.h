#pragma once
#ifndef __CLASS_SPOTLIGHT_H__
#define __CLASS_SPOTLIGHT_H__

#include "DirectionalLight.h"

class CFrustum;

class CSpotLight : public CDirectionalLight
{
protected:
	float m_Angle;
	float m_FallOff;

	// movimiento luz
	float m_fMeshAngle;
	float m_fMoveTime;
	bool m_bMoveDir;
	int  m_iMoves;

public:
	CSpotLight();

	virtual void Update(float ElapsedTime);

	void SetAngle(float Angle);
	float GetAngle() const;
	void SetFallOff(const float FallOff);
	float GetFallOff() const;

	void Move(float ElapsedTime);

	void RenderDebug(CRenderManager *RM);
	//virtual void RenderDeferred(CRenderManager* _pRM, CEffectTechnique* EffectTechnique);
	void SetShadowMap(CRenderManager *RM);
	
	bool isVisible(CRenderManager &RM, const CFrustum* Frustum);
	float GetRadius();
};

#endif