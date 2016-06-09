#include "Light.h"
#include "SpotLight.h"
#include "Texture.h"
#include "CubeTexture.h"
#include "Effect.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "MeshInstance.h"
#include "Renderer\DrawQuadRendererCommand.h"
#include "Renderer\RenderableObjectsTechniqueManager.h"
#include "Camera\Camera.h"
#include "VertexType.h"
#include "Random.h"
#include "Base.h"

CLight::CLight()
	: m_Color(colBLACK), m_Type(OMNI), m_RenderShadows(false), m_StartRangeAttenuation(0.0f), m_EndRangeAttenuation(0.0f), m_StaticShadowMap(NULL), m_DynamicShadowMap(NULL), 
	m_ShadowMaskTexture(NULL), m_DynamicShadowMapBlurH(NULL), m_DynamicShadowMapBlurV(NULL), m_CubeTextNear(NULL), m_CubeTextMedium(NULL), m_CubeTextFar(NULL),
	m_GenerateDynamicShadowMap(false), m_GenerateStaticShadowMap(false), m_MustUpdateStaticShadowMap(false), m_SoftShadow(false),m_BlurShadowMap(false), m_bIsIntermittent(false), 
	m_fTime(0.f), m_fChange(0.f), m_fTMin(0.f), m_fTMax(0.f), m_World(0), m_fVarTime(0.f), m_fOffTime(0.0f), m_bVarIntensity(false), m_bVarAtt(false), m_fAttTime(0.0f), 
	m_fCurrAttTime(0.0f), m_fAttInc(0.0f), m_bRisingAtt(true), m_LightMesh(NULL), m_TextureON(NULL), m_TextureOFF(NULL), m_bActive(true), m_bMoving(false), m_iMaxMoves(0), 
	m_fIncMove(0.0f), m_EmitterName(""), m_Emitter(NULL)
{
}


CLight::~CLight()
{
	if (m_GenerateDynamicShadowMap)
	{
		CHECKED_DELETE(m_DynamicShadowMap);
		CHECKED_DELETE(m_CubeTextNear);
		CHECKED_DELETE(m_CubeTextMedium);
		CHECKED_DELETE(m_CubeTextFar);
	}
	if (m_SoftShadow)
	{
		CHECKED_DELETE(m_DynamicShadowMapBlurH);
		CHECKED_DELETE(m_DynamicShadowMapBlurV);
	}
	if (m_GenerateStaticShadowMap)
	{
		CHECKED_DELETE(m_StaticShadowMap);
	}
	if (m_ShadowMaskTexture!=NULL)
	{
		CHECKED_DELETE(m_ShadowMaskTexture);
	}
}

void CLight::Update(float ElapsedTime)
{
	CRandom * l_Random = CORE->GetRandom();

	// Propiedades intermitencia, atenuación, intensidad...
	if (m_bIsIntermittent)
	{
		m_CurrStartRangeAtt = m_StartRangeAttenuation;
		m_CurrEndRangeAtt = m_EndRangeAttenuation;

		m_fTime += ElapsedTime;
		
		if (m_fTime >= m_fChange)
		{
			m_fChange = l_Random->getRandFloat(m_fTMin, m_fTMax);
			m_Visible = !m_Visible;
			m_fTime = 0.f;
		}
	}
	else if (m_bVarIntensity && m_Name != "LASER" && m_Name != "LINTERNA")		// variamos intensidad de luz
	{
		m_fVarTime += ElapsedTime;

		if (m_fVarTime >= m_fTMin)
		{
			m_CurrStartRangeAtt = m_StartRangeAttenuation;
			m_CurrEndRangeAtt = l_Random->getRandFloat(m_StartRangeAttenuation+m_EndRangeAttenuation/3, m_EndRangeAttenuation);
			m_fVarTime = 0.f;
		}
	}
	else if (m_bVarAtt)		// variamos atenuación de luz
	{
		m_fCurrAttTime += ElapsedTime;

		if (m_fCurrAttTime >= m_fAttTime)
		{
			m_fCurrAttTime = 0.f;

			if (m_CurrEndRangeAtt > m_EndRangeAttenuation || m_CurrEndRangeAtt < m_StartRangeAttenuation) {
				m_bRisingAtt = !m_bRisingAtt;
			}

			if (m_bRisingAtt)
				m_CurrEndRangeAtt += m_fAttInc;
			else
				m_CurrEndRangeAtt -= m_fAttInc;
		}
	}
	else
	{
		m_CurrStartRangeAtt = m_StartRangeAttenuation;
		m_CurrEndRangeAtt = m_EndRangeAttenuation;
	}

	// Cambia textura de la malla asociada segun estado de la luz
	if (m_LightMesh!= NULL && m_TextureON!=NULL && m_TextureOFF!=NULL)
	{
		if (m_bIsIntermittent || m_bVarAtt || m_bVarIntensity)
		{
			bool bOnOff = false;
			if (m_fOffTime > 0.0f && (m_bVarAtt || m_bVarIntensity))
				bOnOff = (m_CurrEndRangeAtt > m_fOffTime);
			else if (m_bIsIntermittent)
				bOnOff = m_Visible;

			m_LightMesh->GetStaticMesh()->SetTexture(bOnOff ? m_TextureON : m_TextureOFF, 1);
		}
	}

	// Cambia el estado del emisor de particulas asociado a la luz
	if (m_EmitterName != "") {
		if (m_Emitter==NULL)
			m_Emitter = (CParticleEmitter*) CORE->GetParticleManager()->GetParticleEmitter(m_EmitterName);	
		if (m_Emitter!=NULL)
			m_Emitter->SetEnabled(m_Visible);
	}
}

void CLight::SetColor(const CColor &Color)
{
	m_Color = Color;
}

const CColor & CLight::GetColor() const
{
	return m_Color;
}

void CLight::SetStartRangeAttenuation(const float StartRangeAttenuation)
{
	m_StartRangeAttenuation = StartRangeAttenuation;
}

float CLight::GetStartRangeAttenuation() const
{
	//return m_StartRangeAttenuation;
	return m_CurrStartRangeAtt;
}

void CLight::SetEndRangeAttenuation(const float EndRangeAttenuation)
{
	m_EndRangeAttenuation = EndRangeAttenuation;
}

float CLight::GetEndRangeAttenuation() const
{
	//return m_EndRangeAttenuation;
	return m_CurrEndRangeAtt;
}

bool CLight::GetRenderShadows() const
{
	if (m_RenderShadows)
	{
		return true;
	}
	return false;
}

// Shadow map
void CLight::SetGenerateDynamicShadowMap(bool GenerateDynamicShadowMap)
{
	m_GenerateDynamicShadowMap=GenerateDynamicShadowMap;
}

bool CLight::GetGenerateDynamicShadowMap() const
{
	return m_GenerateDynamicShadowMap;
}

void CLight::SetGenerateStaticShadowMap(bool GenerateStaticShadowMap)
{
	m_GenerateStaticShadowMap=GenerateStaticShadowMap;
}

bool CLight::GetGenerateStaticShadowMap() const
{
	return m_GenerateStaticShadowMap;
}

void CLight::SetMustUpdateStaticShadowMap(bool MustUpdateStaticShadowMap)
{
	m_MustUpdateStaticShadowMap=MustUpdateStaticShadowMap;
}

bool CLight::GetMustUpdateStaticShadowMap() const
{
	return m_MustUpdateStaticShadowMap;
}

CTexture * CLight::GetStaticShadowMap() const
{
	return m_StaticShadowMap;
}

CTexture * CLight::GetDynamicShadowMap() const
{
	return m_DynamicShadowMap;
}

CTexture * CLight::GetShadowMaskTexture() const
{
	return m_ShadowMaskTexture;
}

unsigned int CLight::calcShadowMapQuality()
{
	int quality = 0;		// default maximum quality

	CCamera* l_Cam = CORE->GetCamera();
	if (l_Cam!=NULL) {
		float l_Dist = l_Cam->GetEye().Distance(GetPosition());
		if (l_Dist > m_EndRangeAttenuation && l_Dist <= m_EndRangeAttenuation*1.5f)
			quality = 1;	//medium shadow map quality
		else if (l_Dist > m_EndRangeAttenuation*4)
			quality = 2;	//lowest shadow map quality
	}

	return quality;
}

CCubeTexture * CLight::GetCubeShadowMap(unsigned int quality) const
{
	if (quality==0)
		return m_CubeTextNear;
	if (quality==1)
		return m_CubeTextMedium;
	
	return m_CubeTextFar;
}

std::vector<CRenderableObjectsManager *> & CLight::GetStaticShadowMapRenderableObjectsManagers()
{
	return m_StaticShadowMapRenderableObjectsManagers;
}

std::vector<CRenderableObjectsManager *> & CLight::GetDynamicShadowMapRenderableObjectsManagers()
{
	return m_DynamicShadowMapRenderableObjectsManagers;
}

const Mat44f & CLight::GetViewShadowMap() const
{
	return m_ViewShadowMap;
}

const Mat44f & CLight::GetProjectionShadowMap() const
{
	return m_ProjectionShadowMap;
}

void CLight::GenerateShadowMap(CRenderManager *RM)
{
	CCamera* l_Cam = CORE->GetCamera();
	if (l_Cam!=NULL) {	
		float l_Dist = CORE->GetCamera()->GetEye().Distance(GetPosition());
		//if (l_Dist <= m_EndRangeAttenuation*4.0f) {
			if (m_SoftShadow) {		// opt.:don't blur far shadows
				const float MAX_DIST_SHADOW_BLUR = 8.0f;
				m_BlurShadowMap = (l_Dist < max(MAX_DIST_SHADOW_BLUR, m_EndRangeAttenuation - m_StartRangeAttenuation));
			}

			SetShadowMap(RM);
		//}
	}
}

void CLight::RenderShadowMap(CRenderManager *RM)
{
	if(m_GenerateStaticShadowMap && m_MustUpdateStaticShadowMap)
	{
		m_StaticShadowMap->SetAsRenderTarget(0);
		RM->Clear(true, true, true, 0xffffffff);
		for(size_t i=0;i<m_StaticShadowMapRenderableObjectsManagers.size();++i)
			m_StaticShadowMapRenderableObjectsManagers[i]->RenderShadow(RM, this);
		m_MustUpdateStaticShadowMap=false;
		m_StaticShadowMap->UnsetAsRenderTarget(0);
	}
	if(m_DynamicShadowMapRenderableObjectsManagers.size()>0)
	{
		// Generate dynamic shadow map texture
		m_DynamicShadowMap->SetAsRenderTarget(0);
		RM->Clear(true, true, true, 0xffffffff);
		for(size_t i=0;i<m_DynamicShadowMapRenderableObjectsManagers.size();++i)
			m_DynamicShadowMapRenderableObjectsManagers[i]->RenderShadow(RM, this);
		m_DynamicShadowMap->UnsetAsRenderTarget(0);
	}
}

void CLight::BlurShadowMapH(CRenderManager *RM)
{
	if (m_RenderShadows && m_SoftShadow && m_BlurShadowMap)
	{
		// Make sure the render target isn't still set as a source texture
		for (size_t i=0; i<16; ++i) CORE->GetRenderManager()->GetDevice()->SetTexture(i, NULL);

		// blur H dynamic shadow map texture
		RECT l_Rect;
		l_Rect.top=l_Rect.left=(long)0.0;
		l_Rect.right=m_DynamicShadowMap->GetWidth()-1;
		l_Rect.bottom=m_DynamicShadowMap->GetHeight()-1;
	
		m_DynamicShadowMap->Activate(1);
		CRenderableObjectTechnique *l_Rot= CORE->GetRenderableObjectsTechniqueManager()->GetResource(CORE->GetRenderableObjectsTechniqueManager()->GetRenderableObjectTechniqueNameByVertexType(SCREEN_TEXTURE_COLORED_VERTEX::GetVertexType()),"");
		if (l_Rot != NULL) {
			m_DynamicShadowMapBlurH->SetAsRenderTarget(0);
			RM->Clear(true, true, true, 0xff0000ff);
			DrawColoredQuad2DTexturedInPixelsByEffectTechnique(RM, l_Rot->GetEffectTechnique(), l_Rect, colWHITE, NULL);
			m_DynamicShadowMapBlurH->UnsetAsRenderTarget(0);
		}
	}
}

void CLight::BlurShadowMapV(CRenderManager *RM)
{
	if (m_RenderShadows && m_SoftShadow && m_BlurShadowMap)
	{	
		// Make sure the render target isn't still set as a source texture
		for (size_t i=0; i<16; ++i) CORE->GetRenderManager()->GetDevice()->SetTexture(i, NULL);

		// blur V dynamic shadow map texture
		RECT l_Rect;
		l_Rect.top=l_Rect.left=(long)0.0;
		l_Rect.right=m_DynamicShadowMap->GetWidth()-1;
		l_Rect.bottom=m_DynamicShadowMap->GetHeight()-1;
	
		m_DynamicShadowMapBlurH->Activate(1);
		CRenderableObjectTechnique *l_Rot= CORE->GetRenderableObjectsTechniqueManager()->GetResource(CORE->GetRenderableObjectsTechniqueManager()->GetRenderableObjectTechniqueNameByVertexType(SCREEN_TEXTURE_COLORED_VERTEX::GetVertexType()),"");
		if (l_Rot != NULL) {
			m_DynamicShadowMapBlurV->SetAsRenderTarget(0);
			RM->Clear(true, true, true, 0xff0000ff);
			DrawColoredQuad2DTexturedInPixelsByEffectTechnique(RM, l_Rot->GetEffectTechnique(), l_Rect, colWHITE, NULL);
			m_DynamicShadowMapBlurV->UnsetAsRenderTarget(0);
		}
	}
}

void CLight::BeginRenderEffectManagerShadowMap(CEffect *Effect)
{
	if(m_GenerateDynamicShadowMap)
	{
		CEffectManager *l_EM=CORE->GetEffectManager();
		l_EM->SetLightViewMatrix(m_ViewShadowMap);
		l_EM->SetShadowProjectionMatrix(m_ProjectionShadowMap);
		if(m_ShadowMaskTexture!=NULL)
			m_ShadowMaskTexture->Activate(SHADOW_MAP_MASK_STAGE);
		if(m_GenerateStaticShadowMap)
			m_StaticShadowMap->Activate(STATIC_SHADOW_MAP_STAGE);
		
		if (m_Type != OMNI) {
			if (m_SoftShadow && m_BlurShadowMap)
				m_DynamicShadowMapBlurV->Activate(DYNAMIC_SHADOW_MAP_STAGE);
			else
				m_DynamicShadowMap->Activate(DYNAMIC_SHADOW_MAP_STAGE);
		}
		else 
			GetCubeShadowMap(calcShadowMapQuality())->Activate(CUBE_MAP_STAGE);

		Effect->SetShadowMapParameters(m_ShadowMaskTexture!=NULL, m_GenerateStaticShadowMap, m_GenerateDynamicShadowMap && m_DynamicShadowMapRenderableObjectsManagers.size()!=0);
	}
}

void CLight::DeleteShadowMap(CEffect *Effect)
{
	if(m_GenerateDynamicShadowMap)
	{
		CEffectManager *l_EM=CORE->GetEffectManager();
		l_EM->SetLightViewMatrix(m_ViewShadowMap);
		l_EM->SetShadowProjectionMatrix(m_ProjectionShadowMap);
		if(m_ShadowMaskTexture!=NULL)
			m_ShadowMaskTexture->Deactivate(SHADOW_MAP_MASK_STAGE);
		if(m_GenerateStaticShadowMap)
			m_StaticShadowMap->Deactivate(STATIC_SHADOW_MAP_STAGE);
		
		if (m_Type != OMNI) {
			if (m_SoftShadow && m_BlurShadowMap)
				m_DynamicShadowMapBlurV->Deactivate(DYNAMIC_SHADOW_MAP_STAGE);
			else
				m_DynamicShadowMap->Deactivate(DYNAMIC_SHADOW_MAP_STAGE);
		}
		else
			GetCubeShadowMap(calcShadowMapQuality())->Deactivate(CUBE_MAP_STAGE);

		Effect->SetShadowMapParameters(false, false, false);
		//m_LightFrustum.Update(m_ViewShadowMap.GetD3DXMatrix() * m_ProjectionShadowMap.GetD3DXMatrix());
	}
}

void CLight::GetShadowsType(const std::string& shadows_type, unsigned int& width, unsigned int& height)
{
	if(shadows_type=="LOW") {
		width = height = 512;
	}
	else if(shadows_type=="MEDIUM") {
		width = height = 1024;
	}
	else if(shadows_type=="HIGH") {
		width = height = 2048;
	}
}

void CLight::SetParameters(CXMLTreeNode &LightsNode, const std::string& shadows_type)
{
	// LLenamos todos los parametros que afectan a todos los tipos de luces
	m_Name = LightsNode.GetPszProperty("name","");
	m_Position=LightsNode.GetVect3fProperty("pos",true);

	CColor l_Color=(CColor)LightsNode.GetVect3fProperty("color",true);
	m_Color.Set(l_Color.GetRed()/255, l_Color.GetGreen()/255, l_Color.GetBlue()/255);

	m_StartRangeAttenuation=LightsNode.GetFloatProperty("att_start_range",0.0f);
	m_EndRangeAttenuation=LightsNode.GetFloatProperty("att_end_range",0.0f);
	if (shadows_type != "OFF")
		m_RenderShadows=LightsNode.GetBoolProperty("render_shadows",false);
	m_World=LightsNode.GetIntProperty("world", 0);
	
	//Shadow Map
	if (m_RenderShadows)
	{
		m_GenerateDynamicShadowMap=LightsNode.GetBoolProperty("generate_shadow_map",false);
		std::string l_DynamicShadowMapFormatType=LightsNode.GetPszProperty("shadow_map_format_type","");
		m_SoftShadow=LightsNode.GetBoolProperty("soft_shadow",false);
		
		CTexture::TFormatType l_FormatType;
		bool error=false;
		if (m_GenerateDynamicShadowMap)
		{
			if (m_Type != OMNI) {	// spot & directional lights
				unsigned int l_DynamicShadowMapWidth=0;
				unsigned int l_DynamicShadowMapHeight=0;
				if(shadows_type=="PREDEFINED") {
					l_DynamicShadowMapWidth=LightsNode.GetIntProperty("shadow_map_width",0);
					l_DynamicShadowMapHeight=LightsNode.GetIntProperty("shadow_map_height",0);
				}
				else
					GetShadowsType(shadows_type, l_DynamicShadowMapWidth, l_DynamicShadowMapHeight);

				l_FormatType =  m_DynamicShadowMap->GetFormatTypeFromString(l_DynamicShadowMapFormatType);
				m_DynamicShadowMap=new CTexture();
				error=m_DynamicShadowMap->Create("DynamicShadowMapTexture_"+m_Name,l_DynamicShadowMapWidth,l_DynamicShadowMapHeight,1,CTexture::RENDERTARGET,CTexture::DEFAULT,l_FormatType);
				if(error)
				{
					std::string msg_error = "CLight::SetParameters()-> Imposible crear textura DynamicShadowMap ";
					LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
				}

				if (m_SoftShadow)
				{
					m_DynamicShadowMapBlurH=new CTexture();
					error=m_DynamicShadowMapBlurH->Create("DynamicShadowMapTextureBlurH_"+m_Name,l_DynamicShadowMapWidth,l_DynamicShadowMapHeight,1,CTexture::RENDERTARGET,CTexture::DEFAULT,l_FormatType);
					if(error)
					{
						std::string msg_error = "CLight::SetParameters()-> Imposible crear textura DynamicShadowMapBlurH_"+m_Name;
						LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
					}

					m_DynamicShadowMapBlurV=new CTexture();
					error=m_DynamicShadowMapBlurV->Create("DynamicShadowMapTextureBlurV_"+m_Name,l_DynamicShadowMapWidth,l_DynamicShadowMapHeight,1,CTexture::RENDERTARGET,CTexture::DEFAULT,l_FormatType);
					if(error)
					{
						std::string msg_error = "CLight::SetParameters()-> Imposible crear textura DynamicShadowMapBlurV_"+m_Name;
						LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
					}
				}
			}
			else {		// omni lights
				unsigned int l_Size = 0;
				if(shadows_type=="PREDEFINED")
					l_Size=LightsNode.GetIntProperty("shadow_map_width",0);
				else
					GetShadowsType(shadows_type, l_Size, l_Size);

				m_CubeTextNear=new CCubeTexture();
				m_CubeTextMedium=new CCubeTexture();
				m_CubeTextFar=new CCubeTexture();
				CCubeTexture::TFormatType formatType =  m_CubeTextNear->GetFormatTypeFromString(l_DynamicShadowMapFormatType);
				
				// omnidirectional shadow map maximum quality
				if(!m_CubeTextNear->Create("CubeMapTextureMax_"+m_Name,l_Size,1,CCubeTexture::RENDERTARGET,CCubeTexture::DEFAULT,formatType))
				{
					std::string msg_error = "CLight::SetParameters()-> Imposible crear textura CubeMap ";
					LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
				}
				// omnidirectional shadow map medium quality
				if(!m_CubeTextMedium->Create("CubeMapTextureMed_"+m_Name,l_Size/2,1,CCubeTexture::RENDERTARGET,CCubeTexture::DEFAULT,formatType))
				{
					std::string msg_error = "CLight::SetParameters()-> Imposible crear textura CubeMap ";
					LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
				}
				// omnidirectional shadow map minimum quality
				if(!m_CubeTextFar->Create("CubeMapTextureMin_"+m_Name,l_Size/4,1,CCubeTexture::RENDERTARGET,CCubeTexture::DEFAULT,formatType))
				{
					std::string msg_error = "CLight::SetParameters()-> Imposible crear textura CubeMap ";
					LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
				}
			}
		}

		m_GenerateStaticShadowMap=LightsNode.GetBoolProperty("generate_static_shadow_map",false);

		if (m_GenerateStaticShadowMap)
		{
			std::string l_StaticShadowMapFormatType=LightsNode.GetPszProperty("static_shadow_map_format_type","");
			l_FormatType =  m_DynamicShadowMap->GetFormatTypeFromString(l_StaticShadowMapFormatType);
			
			unsigned int l_StaticShadowMapWidth=0;
			unsigned int l_StaticShadowMapHeight=0;
			if(shadows_type=="PREDEFINED") {
				l_StaticShadowMapWidth=LightsNode.GetIntProperty("static_shadow_map_width",0);
				l_StaticShadowMapHeight=LightsNode.GetIntProperty("static_shadow_map_height",0);
			}
			else
				GetShadowsType(shadows_type, l_StaticShadowMapWidth, l_StaticShadowMapHeight);

			m_StaticShadowMap=new CTexture();
			error=m_StaticShadowMap->Create("StaticShadowMapTexture",l_StaticShadowMapWidth,l_StaticShadowMapHeight,1,CTexture::RENDERTARGET,CTexture::DEFAULT,l_FormatType);
			if(error)
			{
				std::string msg_error = "CLight::SetParameters()-> Imposible crear textura StaticShadowMap ";
				LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
			}
		}

		std::string l_ShadowTextureMaskFileName=LightsNode.GetPszProperty("shadow_texture_mask","");
		if (l_ShadowTextureMaskFileName != "")
		{
			m_ShadowMaskTexture= new CTexture();
			bool itsOK=m_ShadowMaskTexture->Load(l_ShadowTextureMaskFileName);
			if(!itsOK)
			{
				std::string msg_error = "CLight::SetParameters()-> Imposible cargar ShadowMaskTexture: "+l_ShadowTextureMaskFileName;
				LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
			}
		}

		int l_manager_objects = LightsNode.GetNumChildren();
		for (int j = 0; j < l_manager_objects; ++j)
		{
			if (!LightsNode(j).IsComment())
			{
				std::string l_Layer=LightsNode(j).GetPszProperty("renderable_objects_manager","");
				CRenderableObjectsLayersManager* l_ROLM=CORE->GetRenderableObjectsLayersManager();
				if (l_ROLM != NULL)
				{
					CRenderableObjectsManager * l_ROM=l_ROLM->GetResource(l_Layer);
					if(l_ROM!=NULL)
					{
						if(strcmp(LightsNode(j).GetName(),"static")==0)
						{
							m_StaticShadowMapRenderableObjectsManagers.push_back(l_ROM);
						}
						else if(strcmp(LightsNode(j).GetName(),"dynamic")==0)
						{
							m_DynamicShadowMapRenderableObjectsManagers.push_back(l_ROM);
						}
					}
					else
					{
						std::string msg_error = "CLight::SetParameters()-> No se puede encontrar RenderableObjectsManager para la capa " + l_Layer;
						LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
					}
				}
				else
				{
					std::string msg_error = "CLight::SetParameters()-> RenderableObjectsLayersManager no cargado!";
					LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
				}
			}
		}
	}

	// Propiedades intermitencia 
	m_bIsIntermittent = LightsNode.GetBoolProperty("intermittent", false);
	m_fTMin = LightsNode.GetFloatProperty("tmin", 0.0f);
	m_fTMax = LightsNode.GetFloatProperty("tmax", 0.0f);
						
	// Propiedades intensidad variable
	m_bVarIntensity = LightsNode.GetBoolProperty("varible_intensity", false);
					
	// Propiedades atenuacion variable
	m_bVarAtt = LightsNode.GetBoolProperty("varible_attenuation", false);
	m_fAttInc = LightsNode.GetFloatProperty("inc", 0.0f);
	m_fAttTime = LightsNode.GetFloatProperty("time", 0.0f);

	m_fOffTime = LightsNode.GetFloatProperty("offtime", 0.0f);

	m_CurrStartRangeAtt = m_StartRangeAttenuation;
	m_CurrEndRangeAtt = m_CurrStartRangeAtt;

	// Mesh asociado a la luz
	std::string l_MeshName = LightsNode.GetPszProperty("light_mesh","");
	std::string l_MeshLayer = LightsNode.GetPszProperty("layer","");
	std::string l_TextureON = LightsNode.GetPszProperty("texture_on","");
	std::string l_TextureOFF = LightsNode.GetPszProperty("texture_off","");
						
	if (l_MeshName != ""  && l_MeshLayer != "")
		m_LightMesh = (CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_MeshLayer)->GetResource(l_MeshName);
	if (l_TextureON != "")
		m_TextureON = CORE->GetTextureManager()->GetTexture(l_TextureON);
	if (l_TextureOFF != "")
		m_TextureOFF = CORE->GetTextureManager()->GetTexture(l_TextureOFF);

	//Particulas
	m_EmitterName = LightsNode.GetPszProperty("particles","");

	// Propiedades de movimiento
	m_bMoving = LightsNode.GetBoolProperty("moving", false);
	m_iMaxMoves = LightsNode.GetIntProperty("max_moves", 0);
	m_fIncMove = LightsNode.GetFloatProperty("move_speed", 0.f);
}

bool CLight::isAmbient()
{
	return (m_Name == "AMBIENT_MR" || m_Name == "AMBIENT_ME");
}


