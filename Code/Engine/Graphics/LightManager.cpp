#include "LightManager.h"
#include "RenderManager.h"
#include "OmniLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Logger.h"
#include "Exception.h"


CLightManager::CLightManager()
	: m_Path("")
{
}


CLightManager::~CLightManager()
{
	Destroy();
}

void CLightManager::Load(const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CLightManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_Path = FileName;

	// Obtenemos el nodo "Lights"
	CXMLTreeNode  l_LightsNode = parser["Lights"];
	if (l_LightsNode.Exists())
	{
		// Obtenemos los datos para todos los renderable object
		int l_count_objects = l_LightsNode.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			if (!l_LightsNode.IsComment())
			{
				std::string l_Type=l_LightsNode(i).GetPszProperty("type","");
				if (l_Type!="")
				{
					CLight* l_Light=NULL;

					if (l_Type=="omni")
					{
						l_Light = new COmniLight();
					}
					else if (l_Type=="spot")
					{
						Vect3f l_Dir=l_LightsNode(i).GetVect3fProperty("dir",true);
						float l_Angle=l_LightsNode(i).GetFloatProperty("angle",0.0f);
						float l_FallOff=l_LightsNode(i).GetFloatProperty("fall_off",0.0f);
						l_Light = new CSpotLight();	
						((CSpotLight*)l_Light)->SetDirection(l_Dir);
						((CSpotLight*)l_Light)->SetAngle(l_Angle);
						((CSpotLight*)l_Light)->SetFallOff(l_FallOff);
					}
					else if (l_Type=="directional")
					{
						Vect3f l_Dir=l_LightsNode(i).GetVect3fProperty("dir",true);
						l_Light = new CDirectionalLight();
						((CDirectionalLight*)l_Light)->SetDirection(l_Dir);
						//Ortho shadow map size
						float l_OSMS_w=l_LightsNode(i).GetFloatProperty("ortho_shadow_map_size_width",0.0f);
						float l_OSMS_h=l_LightsNode(i).GetFloatProperty("ortho_shadow_map_size_height",0.0f);
						((CDirectionalLight*)l_Light)->SetOrthoShadowMapSize(l_OSMS_w, l_OSMS_h);
					}

					if (l_Light!=NULL) 
					{
						std::string l_Name = l_LightsNode(i).GetPszProperty("name","");
						l_Light->SetParameters(l_LightsNode(i));
						
						// Añadimos resource al mapa de luces
						AddResource(l_Name, l_Light);
						std::string msg_info = "CLightManager::Load()-> Luz " + l_Light->GetName() + " de tipo " + l_Type + " añadida al manager de luces";
						LOGGER->AddNewLog(ELL_INFORMATION, msg_info.c_str());
					}
					else
					{
						std::string msg_error = "CLightManager::Load()-> Imposible crear tipo de luz " + l_Type;
						LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
					}
				}
			}
		}
	}
}

void CLightManager::Reload()
{
	if (m_Path!="")
	{
		Destroy();
		Load(m_Path);
	}
}

void CLightManager::Update(float ElapsedTime)
{
	for (size_t i=0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CLightManager::Render(CRenderManager *RM)
{
	const CFrustum& l_Frustum = RM->GetFrustum();
	for (size_t i=0; i < m_ResourcesVector.size(); ++i)
	{
		if (m_ResourcesVector[i]->isVisible(*RM, &l_Frustum))
		{
			m_ResourcesVector[i]->RenderDebug(RM);
		}
	}
}

void CLightManager::GenerateShadowMaps(CRenderManager *RM)
{
	const CFrustum& l_Frustum = RM->GetFrustum();
	for (size_t i=0; i < m_ResourcesVector.size(); ++i)
	{
		CLight* l_Light = m_ResourcesVector[i];
		if (l_Light->GetRenderShadows() && l_Light->isVisible(*RM, &l_Frustum))
		{
			l_Light->GenerateShadowMap(RM);
		}
	}
}

void CLightManager::BlurShadowMapsH(CRenderManager *RM)
{
	const CFrustum& l_Frustum = RM->GetFrustum();
	for (size_t i=0; i < m_ResourcesVector.size(); ++i)
	{
		CLight* l_Light = m_ResourcesVector[i];
		if (l_Light->GetRenderShadows() && l_Light->isVisible(*RM, &l_Frustum))
		{
			l_Light->BlurShadowMapH(RM);
		}
	}
}

void CLightManager::BlurShadowMapsV(CRenderManager *RM)
{
	const CFrustum& l_Frustum = RM->GetFrustum();
	for (size_t i=0; i < m_ResourcesVector.size(); ++i)
	{
		CLight* l_Light = m_ResourcesVector[i];
		if (l_Light->GetRenderShadows() && l_Light->isVisible(*RM, &l_Frustum))
		{
			l_Light->BlurShadowMapV(RM);
		}
	}
}

CLight* CLightManager::GetLightByName(std::string Name)
{
	return GetResource(Name);
}

void CLightManager::SetLightEnabled(std::string Name, bool enabled)
{
	CLight* l_Light = GetResource(Name);
	if (l_Light!=NULL)
		l_Light->SetEnabled(enabled);
}

void CLightManager::SetLightActive(std::string Name, bool active)
{
	CLight* l_Light = GetResource(Name);
	if (l_Light!=NULL) 
	{
		l_Light->SetActive(active);
		l_Light->SetEnabled(active);
	}
}

void CLightManager::SetLightMoving(std::string Name, bool move)
{
	CLight* l_Light = GetResource(Name);
	if (l_Light!=NULL) {
		l_Light->SetStartMove(move);
	}
}

void CLightManager::ChangeWorldLighting(CRenderManager *RM, int world, bool enabled)
{
	for (size_t i=0; i < m_ResourcesVector.size(); ++i)
	{
		CLight* l_Light = m_ResourcesVector[i];
		if (l_Light->GetActive() && l_Light->GetWorld() == world)
		{
			l_Light->SetEnabled(enabled);
		}
	}
}