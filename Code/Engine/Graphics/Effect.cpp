#include "Effect.h"
#include "LightManager.h"
#include "Light.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "OmniLight.h"
#include "RenderManager.h"
#include "Logger.h"
#include "Texture.h"
#include "Base.h"

CEffect::CEffect()
	: m_FileName(""), m_Effect(NULL), m_WorldMatrixParameter(NULL), 
	m_ViewMatrixParameter(NULL), 
	m_ProjectionMatrixParameter(NULL),
	m_WorldViewMatrixParameter(NULL),
	m_ViewProjectionMatrixParameter(NULL),
	m_WorldViewProjectionMatrixParameter(NULL),
	m_ViewProjectionInverseParameter(NULL),
	m_ViewToLightProjectionMatrixParameter(NULL),
	m_LightEnabledParameter(NULL), 
	m_LightsTypeParameter(NULL),
	m_LightsPositionParameter(NULL), 
	m_LightsDirectionParameter(NULL),
	m_LightsAngleParameter(NULL), 
	m_LightsColorParameter(NULL),
	m_LightsFallOffParameter(NULL),
	m_LightsStartRangeAttenuationParameter(NULL),
	m_LightsEndRangeAttenuationParameter(NULL),
	m_AmbientLightParameter(NULL),
	m_CameraPositionParameter(NULL),
	m_BonesParameter(NULL),
	m_TimeParameter(NULL),	
	m_MirrorDarkCoefParameter(NULL),
	m_MirrorAnimCoefParameter(NULL),
	m_UseShadowMaskTextureParameter(NULL),
	m_UseStaticShadowmapParameter(NULL),
	m_UseDynamicShadowmapParameter(NULL),
	m_UseShadowMaskTextureParameter2(NULL),
	m_UseStaticShadowmapParameter2(NULL),
	m_UseDynamicShadowmapParameter2(NULL),
	m_UseRenderTargetSizeParameter(NULL)
{
	memset(m_LightsEnabled, 0, sizeof(m_LightsEnabled));
	memset(m_LightsType, 0, sizeof(m_LightsType));
	memset(m_LightsAngle, 0, sizeof(m_LightsAngle));
	memset(m_LightsFallOff, 0, sizeof(m_LightsFallOff));
	memset(m_LightsStartRangeAttenuation, 0, sizeof(m_LightsStartRangeAttenuation));
	memset(m_LightsEndRangeAttenuation, 0, sizeof(m_LightsEndRangeAttenuation));
	memset(m_LightsPosition, 0, sizeof(m_LightsPosition));
	memset(m_LightsDirection, 0, sizeof(m_LightsDirection));
	memset(m_LightsColor, 0, sizeof(m_LightsColor));
}


CEffect::~CEffect()
{
	Unload();
}

void CEffect::SetNullParameters()
{
	m_WorldMatrixParameter=NULL; 
	m_ViewMatrixParameter=NULL; 
	m_ProjectionMatrixParameter=NULL;
	m_WorldViewMatrixParameter=NULL;
	m_ViewProjectionMatrixParameter=NULL;
	m_WorldViewProjectionMatrixParameter=NULL;
	m_ViewToLightProjectionMatrixParameter=NULL;
	m_ViewProjectionInverseParameter=NULL;
	m_LightEnabledParameter=NULL; 
	m_LightsTypeParameter=NULL;
	m_LightsPositionParameter=NULL; 
	m_LightsDirectionParameter=NULL;
	m_LightsAngleParameter=NULL; 
	m_LightsColorParameter=NULL;
	m_LightsFallOffParameter=NULL;
	m_LightsStartRangeAttenuationParameter=NULL;
	m_LightsEndRangeAttenuationParameter=NULL;
	m_CameraPositionParameter=NULL;
	m_BonesParameter=NULL;
	m_TimeParameter=NULL;
	m_MirrorDarkCoefParameter=NULL;
	m_UseShadowMaskTextureParameter=NULL;
	m_UseStaticShadowmapParameter=NULL;
	m_UseDynamicShadowmapParameter=NULL;
	m_UseRenderTargetSizeParameter=NULL;
}

bool CEffect::LoadEffect()
{
	LPD3DXBUFFER l_ErrorBuffer=NULL;

	HRESULT l_HR=D3DXCreateEffectFromFile(CORE->GetRenderManager()->GetDevice(), m_FileName.c_str(), NULL,
		NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &m_Effect, &l_ErrorBuffer);

	if (l_ErrorBuffer)
	{
		std::string msg_info = "Error creating effect '" + m_FileName+"': "+std::string((char *)l_ErrorBuffer->GetBufferPointer());
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_info.c_str());
		CHECKED_RELEASE(l_ErrorBuffer);
		return false;
	}	

	GetParameterBySemantic("WORLD",m_WorldMatrixParameter);
	GetParameterBySemantic("VIEW",m_ViewMatrixParameter);
	GetParameterBySemantic("PROJECTION",m_ProjectionMatrixParameter);
	GetParameterBySemantic("WORLDINVERSE",m_InverseWorldMatrixParameter);
	GetParameterBySemantic("VIEWINVERSE",m_InverseViewMatrixParameter);
	GetParameterBySemantic("PROJECTIONINVERSE",m_InverseProjectionMatrixParameter);

	GetParameterBySemantic("WORLDVIEW",m_WorldViewMatrixParameter);
	GetParameterBySemantic("VIEWPROJECTION",m_ViewProjectionMatrixParameter);
	GetParameterBySemantic("WORLDVIEWPROJECTION",m_WorldViewProjectionMatrixParameter);
	GetParameterBySemantic("VIEWPROJECTIONINVERSE",m_ViewProjectionInverseParameter);
	GetParameterBySemantic("TIME",m_TimeParameter);
	GetParameterBySemantic("MIRRORDARKCOEF",m_MirrorDarkCoefParameter);
	GetParameterBySemantic("MIRRORANIMCOEF",m_MirrorAnimCoefParameter);
	GetParameterBySemantic("WORLD_CAMERA_POSITION",m_CameraPositionParameter);
	GetParameterBySemantic("BONES",m_BonesParameter);
	GetParameterBySemantic("RENDERTARGETSIZE",m_UseRenderTargetSizeParameter);

	// Luces
	GetParameterBySemantic("LIGHTS_TYPE",m_LightsTypeParameter);	
	GetParameterBySemantic("LIGHTS_POSITION",m_LightsPositionParameter);
	GetParameterBySemantic("LIGHTS_DIRECTION",m_LightsDirectionParameter);
	GetParameterBySemantic("LIGHTS_COLOR",m_LightsColorParameter);
	GetParameterBySemantic("CONSTANTATTENUATION",m_LightsStartRangeAttenuationParameter);
	GetParameterBySemantic("LINEARATTENUATION",m_LightsEndRangeAttenuationParameter);
	GetParameterBySemantic("FALLOFFANGLE",m_LightsAngleParameter);
	GetParameterBySemantic("FALLOFFEXPONENT",m_LightsFallOffParameter);
	GetParameterBySemantic("VIEWTOLIGHT",m_ViewToLightProjectionMatrixParameter);
	GetParameterBySemantic("AMBIENTLIGHT",m_AmbientLightParameter);

	// Shadow map
	GetParameterBySemantic("DYNAMICSHADOWMAP", m_UseDynamicShadowmapParameter);
	GetParameterBySemantic("STATICSHADOWMAP", m_UseStaticShadowmapParameter);
	GetParameterBySemantic("SHADOWMAPMASK", m_UseShadowMaskTextureParameter);

	GetParameterBySemantic("DYNAMICSHADOWMAP2", m_UseDynamicShadowmapParameter2);
	GetParameterBySemantic("STATICSHADOWMAP2", m_UseStaticShadowmapParameter2);
	GetParameterBySemantic("SHADOWMAPMASK2", m_UseShadowMaskTextureParameter2);

	return true;
}

void CEffect::Unload()
{
	SetNullParameters();
	CHECKED_RELEASE(m_Effect);
}

bool CEffect::Load(const std::string &FileName)
{
	m_FileName = FileName;
	if(LoadEffect())
	{
		return true;
	}
	return false;
}

bool CEffect::Reload()
{
	Unload();
	if(LoadEffect())
	{
		return true;
	}
	return false;
}

LPD3DXEFFECT CEffect::GetD3DEffect() const
{
	return m_Effect;
}

void CEffect::SetLights(size_t MAXShaderLights, const Vect3f& center, const CFrustum* Frustum)
{
	CRenderManager* l_RM = CORE->GetRenderManager();
	const std::vector<CLight *>& l_Lights = CORE->GetLightManager()->GetLights();
	size_t l_NumTotalLights = CORE->GetLightManager()->getNumLights();

	if (l_NumTotalLights < MAXShaderLights)
		MAXShaderLights = l_Lights.size();

	memset(m_LightsEnabled, 0, sizeof(m_LightsEnabled));
	memset(m_LightsType, 0, sizeof(m_LightsType));
	memset(m_LightsAngle, 0, sizeof(m_LightsAngle));
	memset(m_LightsFallOff, 0, sizeof(m_LightsFallOff));
	memset(m_LightsStartRangeAttenuation, 0, sizeof(m_LightsStartRangeAttenuation));
	memset(m_LightsEndRangeAttenuation, 0, sizeof(m_LightsEndRangeAttenuation));
	memset(m_LightsPosition, 0, sizeof(m_LightsPosition));
	memset(m_LightsDirection, 0, sizeof(m_LightsDirection));
	memset(m_LightsColor, 0, sizeof(m_LightsColor));
	
	// de todas las luces, nos quedamos con el conjunto de luces no ambiente que esten activas y sean visibles por la camara
	CLight* l_LightsON[MAX_LEVEL_ACTIVE_LIGHTS];	// std::vector<> más lento, arrays convencionales mejoran rendimiento
	memset(l_LightsON, 0, sizeof(l_LightsON));
	for (size_t l=0; l<l_NumTotalLights && l<MAX_LEVEL_ACTIVE_LIGHTS; l++)
	{
		CLight* l_Light = l_Lights[l];
		if (l_Light->GetEnabled() && !l_Light->isAmbient() && l_Light->isVisible(*l_RM, Frustum))
		{
			l_LightsON[l] = l_Light;
		}
	}

	// de este conjunto de luces nos quedaremos con las 4 más cercanas al centro del objeto
	size_t i = 0, n = 1;
	CLight* l_LightsOK[MAX_LIGHTS_BY_SHADER];
	memset(l_LightsOK, 0, sizeof(l_LightsOK));
	while (i < MAX_LIGHTS_BY_SHADER && n < MAX_LIGHT_ITERATIONS)
	{
		for (size_t l=0; l<MAX_LEVEL_ACTIVE_LIGHTS && i<MAXShaderLights; l++)
		{
			CLight* l_Light = l_LightsON[l];
			
			if (l_Light != NULL)
			{
				Vect3f l_LightPos = l_Light->GetPosition();
				float l_Dist = l_LightPos.Distance(center);
				float l_Att = l_Light->GetEndRangeAttenuation()*((float)n); 
			
				if (l_Dist < l_Att)
				{
					l_LightsOK[i] = l_Light;
					i++;
					l_LightsON[l] = NULL;
				}
			}
		}

		n++;
	}

	// finalmente seteamos las luces encontradas al shader
	for (size_t l=0; l<MAX_LIGHTS_BY_SHADER;++l)
		if (l_LightsOK[l] != NULL) {
			SetLight(l, l_LightsOK[l]);
			l_LightsOK[l]->BeginRenderEffectManagerShadowMap(this, l);
		}
}

bool CEffect::SetLight(size_t Index, CLight* Light)
{
	if (Light != NULL && Index < MAX_LIGHTS_BY_SHADER)
	{
		m_LightsEnabled[Index] = Light->GetVisible();
		m_LightsType[Index] = Light->GetType();
		m_LightsColor[Index] = Vect3f (Light->GetColor().GetRed(), Light->GetColor().GetGreen(), Light->GetColor().GetBlue());
		m_LightsStartRangeAttenuation[Index] = Light->GetStartRangeAttenuation();
		if (Light->GetEndRangeAttenuation() < Light->GetStartRangeAttenuation())
			m_LightsEndRangeAttenuation[Index] = Light->GetStartRangeAttenuation();
		else
			m_LightsEndRangeAttenuation[Index] = Light->GetEndRangeAttenuation();

		m_LightsPosition[Index] = Light->GetPosition();

		if (m_LightsType[Index] == Light->SPOT)
		{
			m_LightsAngle[Index] = cosf((((CSpotLight*)Light)->GetAngle() * D3DX_PI / 180.0f)/2.0f);
			m_LightsFallOff[Index] = cosf((((CSpotLight*)Light)->GetFallOff() * D3DX_PI / 180.0f)/2.0f);
			m_LightsDirection[Index] = ((CSpotLight*)Light)->GetDirection();
		}

		if ( m_LightsType[Index] == Light->DIRECTIONAL)
		{
			m_LightsDirection[Index] = ((CDirectionalLight*)Light)->GetDirection();
		}
	}
	else
		return false;

	return true;
}

void CEffect::SetShadowMapParameters(bool UseShadowMaskTexture, bool UseStaticShadowmap, bool UseDynamicShadowmap, size_t idx)
{
	if (m_Effect != NULL)
	{
		if (idx == 0) {
			m_Effect->SetBool(m_UseShadowMaskTextureParameter, UseShadowMaskTexture ? TRUE : FALSE);
			m_Effect->SetBool(m_UseStaticShadowmapParameter, UseStaticShadowmap ? TRUE : FALSE);
			m_Effect->SetBool(m_UseDynamicShadowmapParameter, UseDynamicShadowmap ? TRUE : FALSE);
		} 
		else {
			m_Effect->SetBool(m_UseShadowMaskTextureParameter2, UseShadowMaskTexture ? TRUE : FALSE);
			m_Effect->SetBool(m_UseStaticShadowmapParameter2, UseStaticShadowmap ? TRUE : FALSE);
			m_Effect->SetBool(m_UseDynamicShadowmapParameter2, UseDynamicShadowmap ? TRUE : FALSE);
		}
	}
}

//DirectX Methods Interface

D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{
	if (m_Effect!=NULL)
	{
		D3DXHANDLE l_EffectTechnique=m_Effect!=NULL ? m_Effect->GetTechniqueByName(TechniqueName.c_str()) : NULL;
		return l_EffectTechnique;
	}

	return NULL;
}

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle)
{
	if (m_Effect!=NULL) {
		l_Handle = m_Effect->GetParameterBySemantic(NULL,SemanticName.c_str());

		if (l_Handle==NULL)
		{
			std::string msg_info = "Parameter by semantic " + SemanticName + " wasn't found on effect " + m_FileName;
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_WARNING, msg_info.c_str());
		}
	}
}

