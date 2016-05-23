#include "EffectTechnique.h"
#include "EffectManager.h"
#include "LightManager.h"
#include "RenderManager.h"
#include "Texture.h"
#include "VertexType.h"
#include "LightManager.h"
#include "OmniLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Core.h"
#include "Base.h"

#include <array>

CEffectTechnique::CEffectTechnique(const CXMLTreeNode& XmlData)
	: m_Effect(NULL)
{
	m_UseCameraPosition = XmlData.GetBoolProperty("use_camera_position",false);
	m_UseInverseProjMatrix = XmlData.GetBoolProperty("use_inverse_projection_matrix",false);
	m_UseInverseViewMatrix = XmlData.GetBoolProperty("use_inverse_view_matrix",false);
	m_UseInverseWorldMatrix = XmlData.GetBoolProperty("use_inverse_world_matrix",false);
	m_UseProjMatrix = XmlData.GetBoolProperty("use_projection_matrix",false);
	m_UseViewMatrix = XmlData.GetBoolProperty("use_view_matrix",false);
	m_UseWorldMatrix = XmlData.GetBoolProperty("use_world_matrix",false);
	m_UseWorldViewMatrix = XmlData.GetBoolProperty("use_world_view_matrix",false);
	m_UseWorldViewProjectionMatrix = XmlData.GetBoolProperty("use_world_view_projection_matrix",false);
	m_UseViewProjectionMatrix = XmlData.GetBoolProperty("use_projection_matrix",false);
	m_UseViewToLightProjectionMatrix = XmlData.GetBoolProperty("use_view_to_light_projection_matrix",false);
	m_UseViewProjectionInverseMatrix = XmlData.GetBoolProperty("use_view_projection_inverse_matrix",false);
	m_UseTime = XmlData.GetBoolProperty("use_time",false);
	m_UseLights = XmlData.GetBoolProperty("use_lights",false);
	m_NumOfLights = XmlData.GetIntProperty("num_of_lights",0);
	m_UseLightAmbientColor = XmlData.GetBoolProperty("use_light_ambient_color",false);
	m_UseRenderTargetSize = XmlData.GetBoolProperty("use_render_target_size",false);

	m_Effect = CORE->GetEffectManager()->GetEffect(XmlData.GetPszProperty("effect",""));
	m_TechniqueName = XmlData.GetPszProperty("name","");

	Refresh();
}


CEffectTechnique::~CEffectTechnique()
{
}

bool CEffectTechnique::BeginRender()
{
	if(m_Effect==NULL)
		return false;
	if(m_Effect->GetD3DEffect()==NULL)
		return false;
	
	CEffectManager* l_EM = CORE->GetEffectManager();

	Mat44f l_VM = l_EM->GetViewMatrix();
	Mat44f l_PM = l_EM->GetProjectionMatrix();
	Mat44f l_WM = l_EM->GetWorldMatrix();

	const D3DXMATRIX l_ViewMatrix = l_VM.GetD3DXMatrix();
	const D3DXMATRIX l_ProjMatrix = l_PM.GetD3DXMatrix();
	const D3DXMATRIX l_WorldMatrix = l_WM.GetD3DXMatrix();

	if (m_UseViewMatrix)
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetViewMatrixParameter(), &l_ViewMatrix );

	if (m_UseWorldMatrix)
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetWorldMatrixParameter(), &l_WorldMatrix );

	if (m_UseProjMatrix)
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetProjectionMatrixParameter(), &l_ProjMatrix );	

	if (m_UseInverseViewMatrix)
	{
		const D3DXMATRIX l_InverseViewMatrix = l_VM.GetInverted().GetD3DXMatrix();
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetInverseViewMatrixParameter(), &l_InverseViewMatrix );
	}

	if (m_UseInverseWorldMatrix)
	{
		const D3DXMATRIX l_InverseWorldMatrix = l_WM.GetInverted().GetD3DXMatrix();
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetInverseWorldMatrixParameter(), &l_InverseWorldMatrix );
	}

	if (m_UseInverseProjMatrix)
	{
		const D3DXMATRIX l_InverseProjMatrix = l_PM.GetInverted().GetD3DXMatrix();
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetInverseProjectionMatrixParameter(), &l_InverseProjMatrix );	
	}

	if (m_UseViewProjectionMatrix)
	{
		const D3DXMATRIX l_ViewProjMatrix = l_EM->GetViewProjectionMatrix().GetD3DXMatrix();
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetViewProjectionMatrixParameter(), &l_ViewProjMatrix );	
	}

	if (m_UseWorldViewMatrix)
	{
		const D3DXMATRIX l_WorldViewMatrix = l_WorldMatrix * l_ViewMatrix;
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetWorldViewMatrixParameter(), &l_WorldViewMatrix );	
	}

	if (m_UseWorldViewProjectionMatrix)
	{
		const D3DXMATRIX l_WorldViewProjMatrix = l_WorldMatrix*l_ViewMatrix*l_ProjMatrix;
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetWorldViewProjectionMatrixParameter(), &l_WorldViewProjMatrix );	
	}

	if (m_UseViewProjectionInverseMatrix)
	{
		const D3DXMATRIX l_ViewProjInvMatrix = l_EM->GetViewProjectionMatrix().GetInverted().GetD3DXMatrix();
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetViewProjectionInverseMatrixParameter(), &l_ViewProjInvMatrix );
	}

	if (m_UseCameraPosition)
	{
		Vect3f l_CameraEye = l_EM->GetCameraEye();
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetCameraPositionParameter(), &l_CameraEye.x, 3);
	}

	if (m_UseTime)
		m_Effect->GetD3DEffect()->SetFloat(m_Effect->GetTimeParameter(), CORE->GetTime());

	m_Effect->GetD3DEffect()->SetFloat(m_Effect->GetMirrorDarkCoefParameter(), l_EM->GetMirrorDarkCoef());
	m_Effect->GetD3DEffect()->SetFloat(m_Effect->GetMirrorAnimCoefParameter(), l_EM->GetMirrorAnimCoef());

	// Luces
	if (m_UseLights)
	{
		const Vect3f& l_Ambient = CORE->GetLightManager()->GetAmbientLight(); 
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsAmbientParameter(), &(l_Ambient.x) , 3);

		m_Effect->GetD3DEffect()->SetIntArray(m_Effect->GetLightsTypeParameter(), &m_Effect->GetLightsType()[0], m_NumOfLights);
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsPositionParameter(), &m_Effect->GetLightsPosition()[0].x, 3*m_NumOfLights);
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsColorParameter(), &m_Effect->GetLightsColor()[0].x , 3*m_NumOfLights);
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsStartRangeAttenuationParameter(), &m_Effect->GetLightsStartRangeAttenuation()[0], m_NumOfLights);
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsEndRangeAttenuationParameter(), &m_Effect->GetLightsEndRangeAttenuation()[0], m_NumOfLights);

		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsDirectionParameter(), &m_Effect->GetLightsDirection()[0].x, 3*m_NumOfLights);

		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsAngleParameter(), &m_Effect->GetLightsAngle()[0], m_NumOfLights);
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsFallOffParameter(), &m_Effect->GetLightsFallOff()[0], m_NumOfLights);
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetLightsDirectionParameter(), &m_Effect->GetLightsDirection()[0].x, 3*m_NumOfLights);
	}

	// Shadow map
	if(m_UseViewToLightProjectionMatrix)
	{
		Mat44f l_LightViewMatrix=l_EM->GetLightViewMatrix();
		Mat44f l_ShadowProjMatrix=l_EM->GetShadowProjectionMatrix();

		/* Usando métodos DirectX:
		D3DXMATRIX mViewToLightProj;
		mViewToLightProj = l_ViewMatrix;
		D3DXMatrixInverse( &mViewToLightProj, NULL, &mViewToLightProj );
		D3DXMatrixMultiply( &mViewToLightProj, &mViewToLightProj, &l_LightViewMatrix.GetD3DXMatrix() );
		D3DXMatrixMultiply( &mViewToLightProj, &mViewToLightProj, &l_ShadowProjMatrix.GetD3DXMatrix() );
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetViewToLightProjectionMatrixParameter(),&mViewToLightProj);
		*/

		D3DXMATRIX l_ViewToLightProj=l_ViewMatrix;
		l_ViewToLightProj=Mat44f(l_ViewToLightProj).GetInverted().GetD3DXMatrix();
		l_ViewToLightProj=l_ViewToLightProj*l_LightViewMatrix.GetD3DXMatrix();
		l_ViewToLightProj=l_ViewToLightProj*l_ShadowProjMatrix.GetD3DXMatrix();
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->GetViewToLightProjectionMatrixParameter(),&l_ViewToLightProj);
	}

	if (m_UseRenderTargetSize)
	{
		uint32 w,h;
		CORE->GetRenderManager()->GetWidthAndHeight(w,h);
		Vect2f TargetSize((float)w, (float)h);
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->GetRenderTargetSizeParameter(), &TargetSize[0], 2);
	}

	return true;
}


bool CEffectTechnique::Refresh()
{
	bool bOk = false;
	if (m_Effect!=NULL) {
		m_D3DTechnique = m_Effect->GetTechniqueByName(m_TechniqueName);
		bOk = true;
	}

	return (bOk && m_D3DTechnique!=NULL);
}


//DirectX Methods Interface

D3DXHANDLE CEffectTechnique::GetD3DTechnique() 
{
	return m_D3DTechnique;
}


bool CEffectTechnique::setLightMap(CTexture &Texture)
{
	if (!m_Effect)
		return false;

	if(m_Effect->GetD3DEffect()==NULL || !m_UseViewToLightProjectionMatrix) 
		return false;
	
	m_Effect->GetD3DEffect()->SetTexture(m_Effect->GetViewToLightProjectionMatrixParameter(),Texture.GetTexture());
	return true;
}
