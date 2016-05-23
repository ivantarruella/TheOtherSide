#include "DirectionalLight.h"
#include "Matrix44.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "Camera\Camera.h"
#include "d3dx9.h"
#include "Base.h"

void CDirectionalLight::SetDirection(const CPoint3D &Direction)
{
	m_Direction = Direction;
}

CPoint3D CDirectionalLight::GetDirection() const
{
	return m_Direction;
}

void CDirectionalLight::Update(float ElapsedTime)
{
	if (!m_bEnabled || !m_bActive)
		return;

	CLight::Update(ElapsedTime);
}

void CDirectionalLight::RenderDebug(CRenderManager *RM)
{
	if(m_Visible){
		Mat44f mat;
		mat.SetIdentity();
		mat.SetFromLookAtY(m_Position, m_Direction*100 + m_Position, Vect3f(0.0,1.0,0.0));
		RM->SetTransform(mat);
		RM->DrawCilinder(10,m_EndRangeAttenuation,m_Color,10);
		//RM->DrawCilinder(10,m_Direction.Length(),m_Color,10);

		//Esfera amarilla para ver el target de la luz. Nos sirve para comprobar la orientación (luego se borra)
		/*
		mat.SetIdentity();
		mat.Translate(m_Direction*100 + m_Position);
		RM->SetTransform(mat);
		RM->DrawSphere(5,20,colYELLOW);

		mat.SetIdentity();		
		RM->SetTransform(mat);
		*/
	}
}

void CDirectionalLight::SetShadowMap(CRenderManager *RM)
{
	CCamera *l_Camera=CORE->GetCamera();
	if (l_Camera==NULL) return;
	CEffectManager *l_EffectManager=CORE->GetEffectManager();
	
	Mat44f l_ProjectionMatrix;
	Mat44f l_ViewMatrix;
	l_ProjectionMatrix.SetZero();
	l_ViewMatrix.SetZero();

	float zn=l_Camera->GetZn();
	float zf=l_Camera->GetZf();
	
	Vect3f eye=Vect3f(m_Position-m_Direction);

	D3DXVECTOR3 l_Eye(eye.x,eye.y,eye.z);
	D3DXVECTOR3 l_LookAt(m_Position.x,m_Position.y,m_Position.z);
	D3DXVECTOR3 l_Up(0.0f,1.0f,0.0f); 
	
	D3DXMATRIX *l_ViewMatrixD3D = &l_ViewMatrix.GetD3DXMatrix();
	D3DXMATRIX *l_ProjectionMatrixD3D = &l_ProjectionMatrix.GetD3DXMatrix();

	D3DXMatrixLookAtLH(l_ViewMatrixD3D,&l_Eye , &l_LookAt,&l_Up);
	D3DXMatrixOrthoLH(l_ProjectionMatrixD3D, m_OrthoShadowMapSize.x, m_OrthoShadowMapSize.y, zn, zf);
	
	m_ViewShadowMap = Mat44f(*l_ViewMatrixD3D);
	m_ProjectionShadowMap = Mat44f(*l_ProjectionMatrixD3D);
	
	l_EffectManager->ActivateCamera(m_ViewShadowMap, m_ProjectionShadowMap, m_Position);

	CLight::RenderShadowMap(RM);
}

void CDirectionalLight::SetOrthoShadowMapSize(float w,float h)
{
	m_OrthoShadowMapSize=Vect2f(w,h);
}

float CDirectionalLight::GetRadius()
{
	assert(!"DirectionalLight has no radius!");
	return 0.0f;
}

bool CDirectionalLight::isVisible(CRenderManager &RM, const CFrustum* Frustum)
{
	return m_bEnabled && m_bActive && m_Visible;
}