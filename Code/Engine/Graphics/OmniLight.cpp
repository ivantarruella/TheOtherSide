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

void COmniLight::updateViewProjMat(CEffectManager *EM, float Aspect, D3DXVECTOR3 Eye, D3DXVECTOR3 LookAt, D3DXVECTOR3 Up)
{
	D3DXMATRIX l_ViewMatrix;
	D3DXMATRIX l_ProjectionMatrix;
	D3DXMatrixLookAtLH(&l_ViewMatrix,&Eye , &LookAt,&Up);	
	D3DXMatrixPerspectiveFovLH( &l_ProjectionMatrix, D3DX_PI /2.0f , Aspect, m_StartRangeAttenuation, m_EndRangeAttenuation); 
	m_ViewShadowMap= Mat44f(l_ViewMatrix);
	m_ProjectionShadowMap= Mat44f(l_ProjectionMatrix);
	EM->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);
}

void COmniLight::createCamForPositiveX(CEffectManager *EM, float Aspect)
{
	updateViewProjMat(EM, Aspect, m_NegativeLookZ, m_PositiveLookX, m_PositiveLookY);
}

void COmniLight::createCamForNegativeX(CEffectManager *EM, float Aspect)
{
	updateViewProjMat(EM, Aspect, m_PositiveLookZ, m_NegativeLookX, m_PositiveLookY);
}

void COmniLight::createCamForPositiveY(CEffectManager *EM, float Aspect)
{
	updateViewProjMat(EM, Aspect, m_PositiveLookX, m_PositiveLookY, m_NegativeLookZ);
}

void COmniLight::createCamForNegativeY(CEffectManager *EM, float Aspect)
{
	updateViewProjMat(EM, Aspect, m_NegativeLookX, m_NegativeLookY, m_NegativeLookZ);
}

void COmniLight::createCamForPositiveZ(CEffectManager *EM, float Aspect)
{
	updateViewProjMat(EM, Aspect, m_PositiveLookX, m_PositiveLookZ, m_PositiveLookY);
}

void COmniLight::createCamForNegativeZ(CEffectManager *EM, float Aspect)
{
	updateViewProjMat(EM, Aspect, m_NegativeLookX, m_NegativeLookZ, m_PositiveLookY);
}

void COmniLight::SetShadowMap(CRenderManager *RM)
{
	CEffectManager *l_EM=CORE->GetEffectManager();
	uint32 w,h;
	RM->GetWidthAndHeight(w,h);
	float l_Aspect = (float)(w/h);

	// render the scene depth to positive X side of the cube map
	cubeFace = m_CubeTexture->GetCubeFacePX();
	m_face = D3DCUBEMAP_FACE_POSITIVE_X;
	createCamForPositiveX(l_EM, l_Aspect);
	RenderShadowMap(RM);

	// render the scene depth to positive Y side of the cube map
	cubeFace = m_CubeTexture->GetCubeFacePY();
	m_face = D3DCUBEMAP_FACE_POSITIVE_Y;
	createCamForPositiveY(l_EM, l_Aspect);
	RenderShadowMap(RM);

	// render the scene depth to positive Z side of the cube map
	cubeFace = m_CubeTexture->GetCubeFacePZ();
	m_face = D3DCUBEMAP_FACE_POSITIVE_Z;
	createCamForPositiveZ(l_EM, l_Aspect);
	RenderShadowMap(RM);

	// render the scene depth to negative X side of the cube map
	cubeFace = m_CubeTexture->GetCubeFaceNX();
	m_face = D3DCUBEMAP_FACE_NEGATIVE_X;
	createCamForNegativeX(l_EM, l_Aspect);
	RenderShadowMap(RM);

	// render the scene depth to negative Y side of the cube map
	cubeFace = m_CubeTexture->GetCubeFaceNY();
	m_face = D3DCUBEMAP_FACE_NEGATIVE_Y;
	createCamForNegativeY(l_EM, l_Aspect);
	RenderShadowMap(RM);

	// render the scene depth to negative Z side of the cube map
	cubeFace = m_CubeTexture->GetCubeFaceNZ();
	m_face = D3DCUBEMAP_FACE_NEGATIVE_Z;
	createCamForNegativeZ(l_EM, l_Aspect);
	RenderShadowMap(RM);
}

void COmniLight::RenderShadowMap(CRenderManager *RM)
{
	// Generate cube depth map texture	
	m_CubeTexture->SetAsRenderTarget(m_face);
	RM->Clear(true, true, true, 0xffffffff);
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

