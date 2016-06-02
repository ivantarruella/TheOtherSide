#include "SpotLight.h"
#include "Matrix44.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Camera\Camera.h"
#include "TextureManager.h"
#include "MeshInstance.h"
#include "RenderableVertexs.h"
#include "IndexedVertexs.h"
#include <assert.h>
#include "Base.h"

CSpotLight::CSpotLight()
	: m_Angle (0.0f), m_FallOff(0.0f),
	  m_fMeshAngle(0.0f), m_fMoveTime(0.0f), m_bMoveDir(false), m_iMoves(1)
{
	m_Type = SPOT;
}


void CSpotLight::SetAngle(float Angle)
{
	m_Angle = Angle;
}

float CSpotLight::GetAngle() const
{
	return m_Angle;
}

void CSpotLight::SetFallOff(const float FallOff)
{
	m_FallOff = FallOff;
}

float CSpotLight::GetFallOff() const
{
	return m_FallOff;
}

void CSpotLight::Update(float ElapsedTime)
{
	if (!m_bEnabled || !m_bActive)
		return;

	// Propiedades movimiento
	if (m_bMoving) 
	{
		Move(ElapsedTime);
	}

	CLight::Update(ElapsedTime);
}

// Movimiento luz
void CSpotLight::Move(float ElapsedTime)
{
	m_fMoveTime += ElapsedTime;
	
	float fCurrTime = 0.01f;
	
	if (m_fMoveTime >= fCurrTime)
	{
		if (m_fMeshAngle >= 0.5f || m_fMeshAngle <= -0.5f)
			m_bMoveDir = !m_bMoveDir;

		if (m_bMoveDir)
			m_fIncMove *= -1;
			
		// Light Mesh
		m_fMeshAngle = GetLightMesh()->GetPitch() + m_fIncMove;
		GetLightMesh()->SetPitch(m_fMeshAngle);

		// Light Dir
		Mat44f mat;
		mat.SetIdentity();
		mat.RotByAngleX(this->GetPitch() + m_fIncMove);

		Vect3f new_dir = mat * this->GetDirection();
		this->SetDirection(new_dir);

		m_fMoveTime = 0.0f;
	}
}

#if 0
void CSpotLight::Move(float ElapsedTime)
{
	m_fMoveTime += ElapsedTime;
	
	float fCurrTime = 0.01f;
	if (m_iMaxMoves!=0)
		fCurrTime = 0.01f + ((float)((float)m_iMoves/(float)m_iMaxMoves))/10;
	
	if (((m_iMoves < m_iMaxMoves) || m_iMaxMoves==0) && (m_fMoveTime >= fCurrTime))
	{
		if (m_iMaxMoves!=0) {
			float fCurrAngle = 0.5f;// - ((float)((float)m_iMoves/(float)m_iMaxMoves))/10.0f;
			if (m_fMeshAngle >= fCurrAngle || m_fMeshAngle <= -fCurrAngle){
				m_bMoveDir = !m_bMoveDir;
				m_iMoves++;
			}
		}
		else {
			if (m_fMeshAngle >= 0.5f || m_fMeshAngle <= -0.5f){
				m_bMoveDir = !m_bMoveDir;
			}
		}

		if (m_bMoveDir)
			m_fIncMove *= -1;
			
		// Light Mesh
		m_fMeshAngle = GetLightMesh()->GetPitch() + m_fIncMove;
		GetLightMesh()->SetPitch(m_fMeshAngle);

		// Light Dir
		Mat44f mat;
		mat.SetIdentity();
		mat.RotByAngleX(this->GetPitch() + m_fIncMove);

		Vect3f new_dir = mat * this->GetDirection();
		this->SetDirection(new_dir);

		m_fMoveTime = 0.0f;
	}
}
#endif

void CSpotLight::RenderDebug(CRenderManager *RM)
{
	if(isVisible(*RM, &RM->GetFrustum())){
		Mat44f mat;
		mat.SetIdentity();
		mat.SetFromLookAtY(m_Position, m_Direction*100 + m_Position, Vect3f(0.0,1.0,0.0));
		RM->SetTransform(mat);

		//float radi= m_Direction.Length()*tan(mathUtils::Deg2Rad<float>((float)m_Angle));
		//RM->DrawConeInv(radi,m_EndRangeAttenuation,m_Color,8);

		//RM->DrawConeInv(radi,m_Direction.Length(),m_Color,10);
		float radi= m_EndRangeAttenuation*tan(mathUtils::Deg2Rad<float>((float)m_FallOff/2));
		RM->DrawConeInv(radi,m_EndRangeAttenuation,m_Color,8);
	}
}

void CSpotLight::SetShadowMap(CRenderManager *RM)
{
	if (FAILED(RM->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN)))
		return;
	
	CEffectManager *l_EffectManager=CORE->GetEffectManager();
	
	uint32 w,h;
	RM->GetWidthAndHeight(w,h);
	float l_Aspect = (float)w/h;
	Vect3f eye=Vect3f(m_Position-m_Direction);
	
	D3DXVECTOR3 l_Eye(eye.x,eye.y,eye.z);
	D3DXVECTOR3 l_LookAt(m_Position.x,m_Position.y,m_Position.z);
	D3DXVECTOR3 l_Up(0.0f,1.0f,0.0f); 
	
	D3DXMATRIX l_ViewMatrix;
	D3DXMATRIX l_ProjectionMatrix;

	D3DXMatrixLookAtLH(&l_ViewMatrix,&l_Eye , &l_LookAt,&l_Up);	
	D3DXMatrixPerspectiveFovLH(&l_ProjectionMatrix, D3DXToRadian(m_FallOff), l_Aspect, m_StartRangeAttenuation, m_EndRangeAttenuation);
	
	m_ViewShadowMap= Mat44f(l_ViewMatrix);
	m_ProjectionShadowMap= Mat44f(l_ProjectionMatrix);
	
	l_EffectManager->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);
	m_LightFrustum.Update(m_ViewShadowMap.GetD3DXMatrix() * m_ProjectionShadowMap.GetD3DXMatrix());

	CLight::RenderShadowMap(RM);

	// restore color writes
	RM->GetDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
}


bool CSpotLight::isVisible(CRenderManager &RM, const CFrustum* Frustum)
{
	if (!m_bEnabled || !m_Visible || !m_bActive)
		return false;

	//const CFrustum& l_Frustum = RM.GetFrustum();			
	Vect3f l_Center = GetPosition()+GetDirection()*m_EndRangeAttenuation/2;
	float l_Radius=m_EndRangeAttenuation/2;

	bool bVisible = Frustum->SphereVisible(l_Center, l_Radius);
	return bVisible;
}


float CSpotLight::GetRadius()
{
	//return m_EndRangeAttenuation*tan(mathUtils::Deg2Rad<float>((float)m_FallOff/2));
	return m_EndRangeAttenuation/2;
}

