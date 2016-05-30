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

		m_PositiveLookX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		m_PositiveLookY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_PositiveLookZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		m_NegativeLookX = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		m_NegativeLookY = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		m_NegativeLookZ = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}
	
	virtual void Update(float ElapsedTime);

	void RenderDebug(CRenderManager *RM);
	void SetShadowMap(CRenderManager *RM);
	void RenderShadowMap(CRenderManager *RM);
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

private:
	LPDIRECT3DSURFACE9 cubeFace;
	D3DCUBEMAP_FACES m_face;

	D3DXVECTOR3 m_PositiveLookX;
	D3DXVECTOR3 m_PositiveLookY;
	D3DXVECTOR3 m_PositiveLookZ;
	D3DXVECTOR3 m_NegativeLookX;
	D3DXVECTOR3 m_NegativeLookY;
	D3DXVECTOR3 m_NegativeLookZ;
};

#endif