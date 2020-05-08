#include "EffectManager.h"
#include "EffectTechnique.h"
#include "EffectsDef.h"
#include "XML\XMLTreeNode.h"
#include "Logger.h"
#include "Exception.h"
#include "VertexType.h"
#include "Base.h"


CEffectManager::CEffectManager()
	:m_CameraEye(0.0,0.0,0.0), m_StaticMeshTechnique(NULL), m_AnimatedModelTechnique(NULL), m_Path(""), m_fMirrorDarkCoef(0.0f), m_fMirrorAnimCoef(0.0f)
{
}

CEffectManager::~CEffectManager()
{
	m_StaticMeshTechnique=NULL;
	m_AnimatedModelTechnique=NULL;
	CleanUp();
}

void CEffectManager::CleanUp()
{
	Destroy();
	m_Effects.Destroy();
	m_DefaultTechniqueEffectMap.clear();
}

void CEffectManager::SetWorldMatrix(const Mat44f &Matrix)
{
	m_WorldMatrix = Matrix;
}

void CEffectManager::SetProjectionMatrix(const Mat44f &Matrix)
{
	m_ProjectionMatrix = Matrix;
}

void CEffectManager::SetViewMatrix(const Mat44f &Matrix)
{
	m_ViewMatrix = Matrix;
}

void CEffectManager::SetViewProjectionMatrix(const Mat44f &ViewProjectionMatrix)
{
	m_ViewProjectionMatrix = ViewProjectionMatrix;
}

void CEffectManager::SetLightViewMatrix(const Mat44f &Matrix)
{
	m_LightViewMatrix = Matrix;
}

void CEffectManager::SetShadowProjectionMatrix(const Mat44f &Matrix)
{
	m_ShadowProjectionMatrix = Matrix;
}

void CEffectManager::SetCameraEye(const Vect3f &CameraEye)
{
	m_CameraEye = CameraEye;
}

size_t CEffectManager::GetMaxLights() const
{
	return MAX_LIGHTS_BY_SHADER;
}

CEffect * CEffectManager::GetEffect(const std::string &Name)
{
	return m_Effects.GetResource(Name);
}
	
CEffectTechnique * CEffectManager::GetEffectTechnique(const std::string &Name)
{
	return (CMapManager<CEffectTechnique>::GetResource(Name));
}

CEffectTechnique * CEffectManager::GetStaticMeshTechnique() const
{
	return m_StaticMeshTechnique;
}

void CEffectManager::SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique)
{
	m_StaticMeshTechnique = StaticMeshTechnique;
}

CEffectTechnique * CEffectManager::GetAnimatedModelTechnique() const
{
	return m_AnimatedModelTechnique;
}

void CEffectManager::SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique)
{
	m_AnimatedModelTechnique = AnimatedModelTechnique;
}

void CEffectManager::ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye)
{
	SetViewMatrix(ViewMatrix);
	SetProjectionMatrix(ProjectionMatrix);
	SetViewProjectionMatrix(ProjectionMatrix*ViewMatrix);
	SetCameraEye(CameraEye);
}

std::string CEffectManager::GetTechniqueEffectNameByVertexDefault(unsigned short VertexType)
{
	return m_DefaultTechniqueEffectMap[VertexType];
}


float CEffectManager::GetMirrorDarkCoef() 
{
	return m_fMirrorDarkCoef; 
}

void CEffectManager::SetMirrorDarkCoef(float coef) 
{ 
	m_fMirrorDarkCoef = coef; 
}

float CEffectManager::GetMirrorAnimCoef() 
{
	return m_fMirrorAnimCoef; 
}

void CEffectManager::SetMirrorAnimCoef(float coef) 
{ 
	m_fMirrorAnimCoef = coef; 
}

void CEffectManager::Load(const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CEffectManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_Path = FileName;

	// Obtenemos el nodo "Effects"
	CXMLTreeNode  l_EffectsNode = parser["effects"];
	if (l_EffectsNode.Exists())
	{
		int l_count_objects = l_EffectsNode.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			if (!l_EffectsNode.IsComment())
			{
				std::string l_Type=l_EffectsNode(i).GetName();
				std::string l_Name = l_EffectsNode(i).GetPszProperty("name","");

				if(l_Type=="default_technique") 
				{
					int l_VertexType = l_EffectsNode(i).GetIntProperty("vertex_type",0);
					std::string l_Technique = l_EffectsNode(i).GetPszProperty("technique","");
					m_DefaultTechniqueEffectMap[l_VertexType] = l_Technique;
				} 
				else if(l_Type=="effect") 
				{
					CEffect *l_pEffect = m_Effects.GetResource(l_Name);
					if (l_pEffect!=NULL)
					{
						// El efecto ya estaba, lo recargamos
						l_pEffect->Reload();
						std::string msg_info = "CEffectManager::Reload()-> Effect " + l_Name  + " recargado";
						LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_info.c_str());
					}
					else
					{
						// El efecto no esta en el mapa actualmente, lo añadimos
						bool bOk = false;
						l_pEffect = new CEffect();
						if (l_pEffect != NULL)
						{
							bOk = l_pEffect->Load(l_EffectsNode(i).GetPszProperty("file",""));
							if (bOk) 
							{
								m_Effects.AddResource(l_Name,l_pEffect);
								std::string msg_info = "CEffectManager::Load()-> Effect " + l_Name  + " añadido al manager de effects";
								LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_info.c_str());
							}
						}
						
						if (!bOk || !l_pEffect)
						{
							std::string msg_info = "CEffectManager::Load()-> Imposible cargar efecto " + l_Name;
							LOGGER->AddNewLog(ELOG_LEVEL::ELL_WARNING, msg_info.c_str());
						}
					}
				} 
				else if(l_Type=="technique")
				{
					CEffectTechnique *l_pEffectTechnique = GetResource(l_Name);
					if (l_pEffectTechnique != NULL)
					{
						// La technique ya estaba cargada, la refrescamos
						l_pEffectTechnique->Refresh();
						std::string msg_info = "CEffectManager::Reload()-> Technique " + l_Name  + " recargada";
						LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_info.c_str());
					}
					else
					{
						// La technique no está actualmente en el mapa, la añadimos
						l_pEffectTechnique = new CEffectTechnique(l_EffectsNode(i));
						if (l_pEffectTechnique!=NULL)
						{
							std::string msg_info = "CEffectManager::Load()-> Technique " + l_Name  + " añadida al manager de effects";
							LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_info.c_str());
							AddResource(l_Name,l_pEffectTechnique);
						}
						else
						{
							std::string msg_error = "CEffectManager::Load()-> Imposible crear technique " + l_Name;
							LOGGER->AddNewLog(ELOG_LEVEL::ELL_WARNING, msg_error.c_str());
						}
					}
				}
			}
		}
	}
}


void CEffectManager::Reload ()
{
	if (m_Path != "")
	{
		m_DefaultTechniqueEffectMap.clear();
		Load (m_Path);
	}
}