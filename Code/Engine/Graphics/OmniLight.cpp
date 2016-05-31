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

void COmniLight::updateViewProjMat(CEffectManager *EM, D3DXVECTOR3 Eye, D3DXVECTOR3 LookAt, D3DXVECTOR3 Up)
{
	D3DXMATRIX l_ViewMatrix;
	D3DXMATRIX l_ProjectionMatrix;
	D3DXMatrixLookAtLH(&l_ViewMatrix,&Eye , &LookAt,&Up);	
	D3DXMatrixPerspectiveFovLH( &l_ProjectionMatrix, D3DX_PI /2.0f , 1.0f, m_StartRangeAttenuation, m_EndRangeAttenuation); 
	m_ViewShadowMap= Mat44f(l_ViewMatrix);
	m_ProjectionShadowMap= Mat44f(l_ProjectionMatrix);
	EM->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);
}

void COmniLight::createCamForPositiveX(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookZ, m_PositiveLookX, m_PositiveLookY);
}

void COmniLight::createCamForNegativeX(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookZ, m_NegativeLookX, m_PositiveLookY);
}

void COmniLight::createCamForPositiveY(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookX, m_PositiveLookY, m_NegativeLookZ);
}

void COmniLight::createCamForNegativeY(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookX, m_NegativeLookY, m_NegativeLookZ);
}

void COmniLight::createCamForPositiveZ(CEffectManager *EM)
{
	updateViewProjMat(EM, m_PositiveLookX, m_PositiveLookZ, m_PositiveLookY);
}

void COmniLight::createCamForNegativeZ(CEffectManager *EM)
{
	updateViewProjMat(EM, m_NegativeLookX, m_NegativeLookZ, m_PositiveLookY);
}

void COmniLight::SetShadowMap(CRenderManager *RM)
{
	if (FAILED(RM->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN)))
		return;

	CEffectManager *l_EM=CORE->GetEffectManager();

	// render the scene depth to positive X side of the cube map
	createCamForPositiveX(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_POSITIVE_X);

	// render the scene depth to positive Y side of the cube map
	createCamForPositiveY(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_POSITIVE_Y);

	// render the scene depth to positive Z side of the cube map
	createCamForPositiveZ(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_POSITIVE_Z);

	// render the scene depth to negative X side of the cube map
	createCamForNegativeX(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_NEGATIVE_X);

	// render the scene depth to negative Y side of the cube map
	createCamForNegativeY(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_NEGATIVE_Y);

	// render the scene depth to negative Z side of the cube map
	createCamForNegativeZ(l_EM);
	RenderShadowMap(RM, D3DCUBEMAP_FACE_NEGATIVE_Z);

	// restore color writes
	RM->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
}

void COmniLight::RenderShadowMap(CRenderManager *RM, D3DCUBEMAP_FACES face)
{
	// Generate cube depth map texture	
	m_CubeTexture->SetAsRenderTarget(face);
	RM->GetDevice()->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, NULL);
	for(size_t i=0;i<m_DynamicShadowMapRenderableObjectsManagers.size();++i)
		m_DynamicShadowMapRenderableObjectsManagers[i]->RenderShadow(RM, this);
	m_CubeTexture->UnsetAsRenderTarget();
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

