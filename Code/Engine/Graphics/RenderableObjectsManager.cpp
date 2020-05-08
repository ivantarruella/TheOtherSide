#include "StaticMeshManager.h"
#include "AnimatedModelsManager.h"
#include "MeshInstance.h"
#include "Logger.h"
#include "Exception.h"
#include "RenderableObjectsManager.h"
#include "RenderManager.h"
#include "include\PhysicActor.h"
#include "Weapon\Weapon.h"
#include "Base.h"

CRenderableObjectsManager::CRenderableObjectsManager()
	: m_bLayerEnabled(true)
{
}


CRenderableObjectsManager::~CRenderableObjectsManager()
{
	CleanUp();
}

void CRenderableObjectsManager::Update(float ElapsedTime)
{
	for (size_t i=0;i<m_RenderableObjects.size();++i)
	{
		if (m_RenderableObjects[i]->GetEnabled())
		{
			m_RenderableObjects[i]->Update(ElapsedTime);
		}
	}
}

void CRenderableObjectsManager::Render(CRenderManager *RM, bool forwardRender)
{
	const CFrustum& l_Frustum = RM->GetFrustum();
	for (size_t i=0;i<m_RenderableObjects.size();++i)
	{
		if (m_RenderableObjects[i]->GetEnabled() && m_RenderableObjects[i]->GetVisible())
		{
			m_RenderableObjects[i]->Render(RM, &l_Frustum, forwardRender);
		}
	}
}

void CRenderableObjectsManager::RenderDebug(CRenderManager *RM)
{
	for (size_t i=0;i<m_RenderableObjects.size();++i)
	{
		if (m_RenderableObjects[i]->GetEnabled())
		{
			m_RenderableObjects[i]->RenderDebug(RM);
		}
	}
}

void CRenderableObjectsManager::RenderShadow(CRenderManager *RM, CLight* Light)
{
	for (size_t i=0;i<m_RenderableObjects.size();++i)
	{
		if (m_RenderableObjects[i]->GetEnabled() && m_RenderableObjects[i]->GetVisible())
		{
			m_RenderableObjects[i]->RenderShadow(RM, Light);
		}
	}
}

void CRenderableObjectsManager::RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum)
{
	for (size_t i=0;i<m_RenderableObjects.size();++i)
	{
		if (m_RenderableObjects[i]->GetEnabled() && m_RenderableObjects[i]->GetVisible())
		{
			m_RenderableObjects[i]->RenderReflected(RM, Mesh, Frustum);
		}
	}
}

void CRenderableObjectsManager:: AddMeshInstance(const CXMLTreeNode& XmlData)
{
	std::string l_InstanceName=XmlData.GetPszProperty("name","");
	std::string l_CoreName=XmlData.GetPszProperty("core","");
	if (l_InstanceName!="" && l_CoreName!="")
	{
		if (1/*GetResource(l_InstanceName) == NULL*/)
		{
			CStaticMesh *l_StaticMesh = CORE->GetStaticMeshManager()->GetResource(l_CoreName);
			if (l_StaticMesh == NULL)
			{
				std::string msg_error = "CRenderableObjectsManager::AddMeshInstance()-> Imposible encontrar core mesh " + l_CoreName + " para instancia " + l_InstanceName;
				LOGGER->AddNewLog(ELOG_LEVEL::ELL_WARNING, msg_error.c_str());
			}
			else
			{

				CInstanceMesh* l_pInstanceMesh = new CInstanceMesh(XmlData);
				AddResource(l_InstanceName, l_pInstanceMesh);
				std::string msg_error = "CRenderableObjectsManager::AddMeshInstance()-> Instancia  " + l_InstanceName + " de core " + l_CoreName + " cargada Ok";
				LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_error.c_str());
			}
		}
		else
		{
			std::string msg_error = "CRenderableObjectsManager::AddMeshInstance()-> Instance mesh " + l_InstanceName + " repetida! IMPORTANTE: CAMBIAR SU NOMBRE!\n";
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_WARNING, msg_error.c_str());
		}
	}
}

void CRenderableObjectsManager:: AddAnimatedModelInstance(const std::string &CoreName ,const std::string &Name, const Vect3f &pos)
{
	std::string l_ModelInstanceName=Name;
	std::string l_ModelCoreName=CoreName;
	
	if (l_ModelCoreName!="" && l_ModelInstanceName!="")
	{		
		CAnimatedInstanceModel * l_pInstanceModel = CORE->GetAnimatedModelsManager()->CreateInstance(CoreName, Name, pos);
		
		if(l_pInstanceModel==NULL)
		{
			std::string msg_error = "CRenderableObjectsManager::AddAnimatedModelInstance()-> Imposible encontrar animated model core " + l_ModelCoreName + " para animated model instance " + l_ModelInstanceName;
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_WARNING, msg_error.c_str());
		}
		else
		{
			AddResource(l_ModelInstanceName, l_pInstanceModel);
			std::string msg_error = "CAnimatedModelsManager::CreateInstance()-> Instancia  " + l_ModelInstanceName + " de animated core model " + l_ModelCoreName + " cargada Ok";
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_error.c_str());
		}
		
	}
}

void CRenderableObjectsManager:: AddAnimatedModelInstance(const CXMLTreeNode& XmlData)
{
	std::string l_ModelInstanceName=XmlData.GetPszProperty("name","");
	std::string l_ModelCoreName=XmlData.GetPszProperty("core","");
	
	if (l_ModelCoreName!="" && l_ModelInstanceName!="")
	{
		
			CAnimatedInstanceModel * l_pInstanceModel = CORE->GetAnimatedModelsManager()->CreateInstance(XmlData);
		
		if(l_pInstanceModel==NULL)
		{
			std::string msg_error = "CRenderableObjectsManager::AddAnimatedModelInstance()-> Imposible encontrar animated model core " + l_ModelCoreName + " para animated model instance " + l_ModelInstanceName;
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_WARNING, msg_error.c_str());
		}
		else
		{
			AddResource(l_ModelInstanceName, l_pInstanceModel);
			std::string msg_error = "CAnimatedModelsManager::CreateInstance()-> Instancia  " + l_ModelInstanceName + " de animated core model " + l_ModelCoreName + " cargada Ok";
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, msg_error.c_str());
		}
		
	}
}

bool CRenderableObjectsManager::AddResource(const std::string &Name, CRenderableObject *RenderableObject)
{
	if (CMapManager<CRenderableObject>::AddResource(Name, RenderableObject))
	{
		m_RenderableObjects.push_back(RenderableObject);
		return true;
	}

	return false;
}

bool CRenderableObjectsManager::RemoveResource(const std::string &Name)
{
	CRenderableObject* l_object = CMapManager<CRenderableObject>::GetResource(Name);

	for(unsigned int i=0; i<m_RenderableObjects.size(); i++)
	{
		if(m_RenderableObjects[i] == l_object)
		{
			m_RenderableObjects.erase(m_RenderableObjects.begin()+i);
			i=m_RenderableObjects.size();
		}
	}

	if(CMapManager<CRenderableObject>::DestroyResource(Name))
	{		
		return true;
	}
	return false;
}
/*
bool CRenderableObjectsManager::isAResource(CPhysicUserData * userData)
{
	for(unsigned int i=0; i < m_Resources.size(); i++)
	{
		CRenderableObject* l_obj = GetResource(i);
		if(l_obj->GetPhysicUserData() == userData)
		{
			return true;
		}
	}
	return false;
}
*/
void CRenderableObjectsManager::CleanUp()
{
	Destroy();
	m_RenderableObjects.clear();
}

void CRenderableObjectsManager::Load(const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CRenderableObjectsManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	// Obtenemos el nodo "Renderable_Objects"
	CXMLTreeNode  l_RenderableObjectsNode = parser["Renderable_Objects"];
	if (l_RenderableObjectsNode.Exists())
	{
		// Obtenemos los datos para todos los renderable object
		int l_count_objects = l_RenderableObjectsNode.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			if (!l_RenderableObjectsNode.IsComment())
			{
				std::string l_Type = l_RenderableObjectsNode(i).GetName();

				if (l_Type=="mesh_instance")
				{
					AddMeshInstance(l_RenderableObjectsNode(i));
				}
				else if (l_Type=="animated_model_instance")
				{
					AddAnimatedModelInstance(l_RenderableObjectsNode(i));
				}
			}
		}
	}
}

CRenderableObject * CRenderableObjectsManager::GetInstance(const std::string &Name) const
{
	return (CMapManager<CRenderableObject>::GetResource(Name));
}


void CRenderableObjectsManager::EnablePhysicLayer(bool enable)
{
	for (size_t i=0;i<m_RenderableObjects.size();++i)
	{
		if (m_RenderableObjects[i] != NULL && m_RenderableObjects[i]->GetCreatePhysics() && m_RenderableObjects[i]->GetCreatePhysics())
		{
			CPhysicActor* l_Actor = m_RenderableObjects[i]->GetPhysicActor();
			if (l_Actor != NULL)
				l_Actor->SetDynamicActorState(!enable);
		}
	}
}