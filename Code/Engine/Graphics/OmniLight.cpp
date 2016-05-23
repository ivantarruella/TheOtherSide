#include "OmniLight.h"
#include "Matrix44.h"
#include "RenderManager.h"
#include <assert.h>
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
	Vect3f m_Direction(1.0f, 0.0f, 0.0f);
	Vect3f eye=m_Position-m_Direction;
	D3DXVECTOR3 l_Eye(eye.x,eye.y,eye.z);
	D3DXVECTOR3 l_LookAt(m_Position.x,m_Position.y,m_Position.z);
	D3DXVECTOR3 l_Up(0.0f,1.0f,0.0f); 

	updateViewProjMat(EM, Aspect, l_Eye, l_LookAt, l_Up);
}

void COmniLight::createCamForNegativeX(CEffectManager *EM, float Aspect)
{
	Vect3f m_Direction(-1.0f, 0.0f, 0.0f);
	Vect3f eye=m_Position-m_Direction;
	D3DXVECTOR3 l_Eye(eye.x,eye.y,eye.z);
	D3DXVECTOR3 l_LookAt(m_Position.x,m_Position.y,m_Position.z);
	D3DXVECTOR3 l_Up(0.0f,1.0f,0.0f); 

	updateViewProjMat(EM, Aspect, l_Eye, l_LookAt, l_Up);
}

void COmniLight::createCamForPositiveY(CEffectManager *EM, float Aspect)
{
	// no hacen falta sombras en el techo
}

void COmniLight::createCamForNegativeY(CEffectManager *EM, float Aspect)
{
	Vect3f m_Direction(0.0f, -1.0f, 0.0f);
	Vect3f eye=m_Position-m_Direction;
	D3DXVECTOR3 l_Eye(eye.x,eye.y,eye.z);
	D3DXVECTOR3 l_LookAt(m_Position.x,m_Position.y,m_Position.z);
	D3DXVECTOR3 l_Up(0.0f,0.0f,1.0f); 
	
	updateViewProjMat(EM, Aspect, l_Eye, l_LookAt, l_Up);
}

void COmniLight::createCamForPositiveZ(CEffectManager *EM, float Aspect)
{
	Vect3f m_Direction(0.0f, 0.0f, 1.0f);
	Vect3f eye=m_Position-m_Direction;
	D3DXVECTOR3 l_Eye(eye.x,eye.y,eye.z);
	D3DXVECTOR3 l_LookAt(m_Position.x,m_Position.y,m_Position.z);
	D3DXVECTOR3 l_Up(0.0f,1.0f,0.0f); 
	
	updateViewProjMat(EM, Aspect, l_Eye, l_LookAt, l_Up);
}

void COmniLight::createCamForNegativeZ(CEffectManager *EM, float Aspect)
{
	Vect3f m_Direction(0.0f, 0.0f, -1.0f);
	Vect3f eye=m_Position-m_Direction;
	D3DXVECTOR3 l_Eye(eye.x,eye.y,eye.z);
	D3DXVECTOR3 l_LookAt(m_Position.x,m_Position.y,m_Position.z);
	D3DXVECTOR3 l_Up(0.0f,1.0f,0.0f); 
	
	updateViewProjMat(EM, Aspect, l_Eye, l_LookAt, l_Up);
}

void COmniLight::SetShadowMap(CRenderManager *RM)
{
#if 0	
	CEffectManager *l_EM=CORE->GetEffectManager();
	
	uint32 w,h;
	RM->GetWidthAndHeight(w,h);
	float l_Aspect = (float)(w/h);
	
	// Y-
	createCamForNegativeY(l_EM, l_Aspect);
	CLight::RenderShadowMap(RM);

	// X+ no va bien
	//createCamForPositiveX(l_EM, l_Aspect);
	//CLight::RenderShadowMap(RM);
	// X- no va bien
	//createCamForNegativeX(l_EM, l_Aspect);
	//CLight::RenderShadowMap(RM);

	// Z+ no va bien
	//createCamForPositiveZ(l_EM, l_Aspect);
	//CLight::RenderShadowMap(RM);
	// Z- no va bien
	//createCamForNegativeZ(l_EM, l_Aspect);
	//CLight::RenderShadowMap(RM);
#endif
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

