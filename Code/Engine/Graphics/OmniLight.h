#pragma once
#ifndef __CLASS_OMNILIGHT_H__
#define __CLASS_OMNILIGHT_H__

#include "Light.h"

class CEffectTechnique;
class CEffectManager;
class CFrustum;

class COmniLight : public CLight
{
public:
	COmniLight() 
		: CLight() 
	{
		m_Type = OMNI;
	}
	
	virtual void Update(float ElapsedTime);

	void RenderDebug(CRenderManager *RM);
	void SetShadowMap(CRenderManager *RM);
	bool isVisible(CRenderManager &RM, const CFrustum* Frustum);
	float GetRadius();

private:
	void updateViewProjMat(CEffectManager *EM, float Aspect, D3DXVECTOR3 l_Eye, D3DXVECTOR3 l_LookAt, D3DXVECTOR3 l_Up);
	void createCamForPositiveX(CEffectManager *EM, float Aspect);
	void createCamForNegativeX(CEffectManager *EM, float Aspect);
	void createCamForPositiveY(CEffectManager *EM, float Aspect);
	void createCamForNegativeY(CEffectManager *EM, float Aspect);
	void createCamForPositiveZ(CEffectManager *EM, float Aspect);
	void createCamForNegativeZ(CEffectManager *EM, float Aspect);
};

#endif