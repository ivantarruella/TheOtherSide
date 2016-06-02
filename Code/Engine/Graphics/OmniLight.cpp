#include "OmniLight.h"
#include "Matrix44.h"
#include "RenderManager.h"
#include <assert.h>
#include "CubeTexture.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Camera\Camera.h"
#include "RenderableVertexs.h"
#include "IndexedVertexs.h"
#include "RenderableObjectsManager.h"
#include "base.h"

void COmniLight::Update(float ElapsedTime)
{
	if (!m_bEnabled || !m_bActive)
		return;

	CLight::Update(ElapsedTime);
}

void COmniLight::RenderDebug(CRenderManager *RM)
{
	if(isVisible(*RM, &RM->GetFrustum())){
		Mat44f mat;
		mat.SetIdentity();
		mat.Translate(m_Position);
		RM->SetTransform(mat);
		RM->DrawSphere(GetRadius(), 8, m_Color);
	}
}

void COmniLight::updateViewProjMat(CEffectManager *EM, D3DXVECTOR3 LookAt, D3DXVECTOR3 Up, D3DXVECTOR3 Eye)
{
	D3DXMATRIX l_ViewMatrix;
	D3DXMATRIX l_ProjectionMatrix;
	D3DXMatrixLookAtLH(&l_ViewMatrix, &Eye, &LookAt, &Up);	
	D3DXMatrixPerspectiveFovLH(&l_ProjectionMatrix, D3DX_PI /2.0f, 1.0f, m_StartRangeAttenuation, m_EndRangeAttenuation); 
	m_ViewShadowMap= Mat44f(l_ViewMatrix);
	m_ProjectionShadowMap= Mat44f(l_ProjectionMatrix);
	EM->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);
	m_LightFrustum.Update(m_ViewShadowMap.GetD3DXMatrix() * m_ProjectionShadowMap.GetD3DXMatrix());
}

void COmniLight::createCamForPositiveX(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookX, m_PositiveLookY, m_NegativeLookZ);
}

void COmniLight::createCamForNegativeX(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookX, m_PositiveLookY, m_PositiveLookZ);
}

void COmniLight::createCamForPositiveY(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookY, m_NegativeLookZ, m_PositiveLookX);
}

void COmniLight::createCamForNegativeY(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookY, m_NegativeLookZ, m_NegativeLookX);
}

void COmniLight::createCamForPositiveZ(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookZ, m_PositiveLookY, m_PositiveLookX);
}

void COmniLight::createCamForNegativeZ(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookZ, m_PositiveLookY, m_NegativeLookX);
}

void COmniLight::SetShadowMap(CRenderManager *RM)
{
	if (FAILED(RM->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED)))
		return;

	LPDIRECT3DDEVICE9 l_Device= RM->GetDevice();
	LPDIRECT3DSURFACE9 frameBuffer;
	l_Device->GetRenderTarget(0, &frameBuffer);
	
	CEffectManager *l_EM=CORE->GetEffectManager();

	// render the scene depth to positive X side of the cube map
	createCamForPositiveX(l_EM);
	RenderShadowMap(RM, GetCubeShadowMap()->GetCubeFacePX());

	// render the scene depth to positive Y side of the cube map
	createCamForPositiveY(l_EM);
	RenderShadowMap(RM, GetCubeShadowMap()->GetCubeFacePY());

	// render the scene depth to positive Z side of the cube map
	createCamForPositiveZ(l_EM);
	RenderShadowMap(RM, GetCubeShadowMap()->GetCubeFacePZ());

	// render the scene depth to negative X side of the cube map
	createCamForNegativeX(l_EM);
	RenderShadowMap(RM, GetCubeShadowMap()->GetCubeFaceNX());

	// render the scene depth to negative Y side of the cube map
	createCamForNegativeY(l_EM);
	RenderShadowMap(RM, GetCubeShadowMap()->GetCubeFaceNY());

	// render the scene depth to negative Z side of the cube map
	createCamForNegativeZ(l_EM);
	RenderShadowMap(RM, GetCubeShadowMap()->GetCubeFaceNZ());

	// restore frame buffer
	l_Device->SetRenderTarget(0, frameBuffer);

	// restore color writes
	RM->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
}

void COmniLight::RenderShadowMap(CRenderManager *RM, LPDIRECT3DSURFACE9 inCubeFaceSurface)
{
	// Generate cube depth map texture	
	LPDIRECT3DDEVICE9 l_Device= RM->GetDevice();
	if(SUCCEEDED(l_Device->SetRenderTarget( (DWORD)0, inCubeFaceSurface ))) {
		l_Device->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, NULL);
		for(size_t i=0;i<m_DynamicShadowMapRenderableObjectsManagers.size();++i)
			m_DynamicShadowMapRenderableObjectsManagers[i]->RenderShadow(RM, this);
	}
}

bool COmniLight::isVisible(CRenderManager &RM, const CFrustum* Frustum)
{
	if (!m_bEnabled || !m_Visible || !m_bActive)
		return false;

	Vect3f l_Center = GetPosition();

	//const CFrustum& l_Frustum = RM.GetFrustum();			
	float l_Radius=GetRadius();

	return Frustum->SphereVisible(l_Center, l_Radius);
}

float COmniLight::GetRadius()
{
	return m_EndRangeAttenuation;
}

