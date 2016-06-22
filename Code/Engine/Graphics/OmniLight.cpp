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

void COmniLight::updateViewProjMat(CEffectManager *EM, D3DXVECTOR3 LookAt, D3DXVECTOR3 Up)
{
	D3DXMATRIX l_ViewMatrix;
	D3DXMATRIX l_ProjectionMatrix;

	D3DXVECTOR3 Eye(m_Position.x, m_Position.y, m_Position.z);
	D3DXVECTOR3 lookAt(m_Position.x + LookAt.x, m_Position.y + LookAt.y, m_Position.z + LookAt.z);
	D3DXMatrixLookAtLH(&l_ViewMatrix, &Eye, &lookAt, &Up);	
	D3DXMatrixPerspectiveFovLH(&l_ProjectionMatrix, D3DX_PI /2.0f, 1.0f, 0.01f, m_EndRangeAttenuation); 
	m_ViewShadowMap= Mat44f(l_ViewMatrix);
	m_ProjectionShadowMap= Mat44f(l_ProjectionMatrix);
	EM->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);
	m_LightFrustum.Update(m_ViewShadowMap.GetD3DXMatrix() * m_ProjectionShadowMap.GetD3DXMatrix());
}

void COmniLight::createCamForPositiveX(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookX, m_PositiveLookY);
}

void COmniLight::createCamForNegativeX(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookX, m_PositiveLookY);
}

void COmniLight::createCamForPositiveY(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookY, m_NegativeLookZ);
}

void COmniLight::createCamForNegativeY(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookY, m_PositiveLookZ);
}

void COmniLight::createCamForPositiveZ(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookZ, m_PositiveLookY);
}

void COmniLight::createCamForNegativeZ(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookZ, m_PositiveLookY);
}

void COmniLight::SetShadowMap(CRenderManager *RM)
{
	CCubeTexture* cubeText = GetCubeShadowMap(calcShadowMapQuality());
	if (cubeText==NULL)
		return;

	LPDIRECT3DDEVICE9 l_Device= RM->GetDevice();
	if (FAILED(l_Device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED)))
		return;

	CEffectManager *l_EM=CORE->GetEffectManager();

	// render the scene depth to positive Y side of the cube map
	createCamForPositiveY(l_EM);		
	RenderShadowMap(RM, D3DCUBEMAP_FACE_POSITIVE_Y, cubeText, cubeText->GetCubeFacePY());
	// render the scene depth to negative Y side of the cube map
	createCamForNegativeY(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_NEGATIVE_Y, cubeText, cubeText->GetCubeFaceNY());

	// render the scene depth to positive Z side of the cube map
	createCamForPositiveZ(l_EM);		
	RenderShadowMap(RM, D3DCUBEMAP_FACE_POSITIVE_Z, cubeText, cubeText->GetCubeFacePZ());
	// render the scene depth to negative Z side of the cube map
	createCamForNegativeZ(l_EM);		
	RenderShadowMap(RM, D3DCUBEMAP_FACE_NEGATIVE_Z, cubeText, cubeText->GetCubeFaceNZ());

	// render the scene depth to positive X side of the cube map
	createCamForPositiveX(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_POSITIVE_X, cubeText, cubeText->GetCubeFacePX());
	// render the scene depth to negative X side of the cube map
	createCamForNegativeX(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_NEGATIVE_X, cubeText, cubeText->GetCubeFaceNX());

	// restore color writes
	RM->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
}

void COmniLight::RenderShadowMap(CRenderManager *RM, D3DCUBEMAP_FACES face, CCubeTexture* cubeText, LPDIRECT3DSURFACE9 inCubeFaceSurface)
{
	LPDIRECT3DDEVICE9 l_Device= RM->GetDevice();
	
	if (cubeText->SetAsRenderTarget(face, inCubeFaceSurface)) 
	{
		l_Device->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, NULL);
		for(size_t i=0;i<m_DynamicShadowMapRenderableObjectsManagers.size();++i)
			m_DynamicShadowMapRenderableObjectsManagers[i]->RenderShadow(RM, this);
		
		cubeText->UnsetAsRenderTarget();
	}
}

bool COmniLight::isVisible(CRenderManager &RM, const CFrustum* Frustum)
{
	if (!m_bEnabled || !m_Visible || !m_bActive)
		return false;

	CCamera *l_Camera=CORE->GetCamera();
	if (l_Camera==NULL) 
		return false;

	Vect3f l_Center = GetPosition();
	Vect3f l_CamPos = l_Camera->GetEye();
	float y_dist = abs(l_Center.y - l_CamPos.y);
	
	if (y_dist >= 2.0f)
		return false;

	//const CFrustum& l_Frustum = RM.GetFrustum();			
	float l_Radius=GetRadius();

	return Frustum->SphereVisible(l_Center, l_Radius);
}

float COmniLight::GetRadius()
{
	return m_EndRangeAttenuation;
}

