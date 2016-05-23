#include "RenderableObjectsLayersManager.h"
#include "RenderManager.h"
#include "Logger.h"
#include "Exception.h"
#include "Base.h"

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
	: m_FileName(""), m_DefaultRenderableObjectManager(NULL)
{
}


CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
	Destroy();
}

void CRenderableObjectsLayersManager::Destroy()
{
	CVectorMapManager::Destroy();
	m_DefaultRenderableObjectManager = NULL;
}

void CRenderableObjectsLayersManager::Load(const std::string &FileName)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CRenderableObjectsLayersManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_FileName = FileName;
	//Destroy();
	// Obtenemos el nodo "Renderable_Objects"
	CXMLTreeNode  l_RenderableObjectsLayersNode = parser["Renderable_Objects"];
	if (l_RenderableObjectsLayersNode.Exists())
	{
		// Obtenemos los datos para todos los renderable object
		int l_count_objects = l_RenderableObjectsLayersNode.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			if (!l_RenderableObjectsLayersNode.IsComment())
			{
				std::string l_Type = l_RenderableObjectsLayersNode(i).GetName();

				if (l_Type=="layer")
				{
					const char* l_LayerName = l_RenderableObjectsLayersNode(i).GetPszProperty("name","");
					CRenderableObjectsManager* l_RenderableObjectsManager = new CRenderableObjectsManager;
					CVectorMapManager::AddResource(l_LayerName, l_RenderableObjectsManager);
					bool l_DefaultProp = l_RenderableObjectsLayersNode(i).GetBoolProperty("default",false);
					if (l_DefaultProp == true)
					{
						m_DefaultRenderableObjectManager = l_RenderableObjectsManager;
					}
				}
				else if (l_Type=="mesh_instance")
				{
					CRenderableObjectsManager* l_RenderableObjectsManager = GetRenderableObjectManager(l_RenderableObjectsLayersNode(i));
					if (l_RenderableObjectsManager!=NULL)
					{
						l_RenderableObjectsManager->AddMeshInstance(l_RenderableObjectsLayersNode(i));
					}
					else
					{
						std::string msg_error = "CRenderableObjectsLayersManager::Load()-> Layer no encontrada!\n";
						LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
					}
				}
				else if (l_Type=="animated_model_instance")
				{
					CRenderableObjectsManager* l_RenderableObjectsManager = GetRenderableObjectManager(l_RenderableObjectsLayersNode(i));
					if (l_RenderableObjectsManager!=NULL)
					{
						l_RenderableObjectsManager->AddAnimatedModelInstance(l_RenderableObjectsLayersNode(i));
					}
					else
					{
						std::string msg_error = "CRenderableObjectsLayersManager::Load()-> Layer no encontrada!\n";
						LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
					}
				}
			}
		}
	}
}

void CRenderableObjectsLayersManager::Reload()
{
	if (m_FileName != "")
	{
		Destroy();
		Load (m_FileName);
	}
}

void CRenderableObjectsLayersManager::Update(float ElapsedTime)
{
	size_t l_Size = CVectorMapManager::GetResourcesVector().size();
	TVectorResources& l_VectorResources = CVectorMapManager::GetResourcesVector();
	for (size_t i=0; i<l_Size; ++i)
	{
		if (l_VectorResources[i]->GetEnabled())
		{
			l_VectorResources[i]->Update(ElapsedTime);
		}
	}
}

void CRenderableObjectsLayersManager::Render(CRenderManager *RM, bool forwardRender)
{
	size_t l_Size = CVectorMapManager::GetResourcesVector().size();
	TVectorResources& l_VectorResources = CVectorMapManager::GetResourcesVector();
	for (size_t i=0; i<l_Size; ++i)
	{
		if (l_VectorResources[i]->GetEnabled())
		{
			l_VectorResources[i]->Render(RM, forwardRender);
		}
	}
}

void CRenderableObjectsLayersManager::Render(CRenderManager *RM, const std::string &LayerName, bool forwardRender)
{
	CRenderableObjectsManager* l_RenderableObjectManager = CVectorMapManager::GetResource(LayerName);
	if (l_RenderableObjectManager!=NULL && l_RenderableObjectManager->GetEnabled())
	{
		l_RenderableObjectManager->Render(RM, forwardRender);
	}
}

void CRenderableObjectsLayersManager::RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum)
{
	size_t l_Size = CVectorMapManager::GetResourcesVector().size();
	TVectorResources& l_VectorResources = CVectorMapManager::GetResourcesVector();
	for (size_t i=0; i<l_Size; ++i)
	{
		if (l_VectorResources[i]->GetEnabled())
		{
			l_VectorResources[i]->RenderReflected(RM, Mesh, Frustum);
		}
	}
}

void CRenderableObjectsLayersManager::RenderReflected(CRenderManager *RM, const std::string &LayerName, CRenderableObject* Mesh, const CFrustum* Frustum)
{
	CRenderableObjectsManager* l_RenderableObjectManager = CVectorMapManager::GetResource(LayerName);
	if (l_RenderableObjectManager!=NULL && l_RenderableObjectManager->GetEnabled())
	{
		l_RenderableObjectManager->RenderReflected(RM, Mesh, Frustum);
	}
}

CRenderableObjectsManager * CRenderableObjectsLayersManager::GetRenderableObjectManager(CXMLTreeNode &Node){

	const char* l_LayerName = Node.GetPszProperty("layer","");
	if (l_LayerName != "")
		return CVectorMapManager::GetResource(l_LayerName);
	else
		return m_DefaultRenderableObjectManager;

}

CRenderableObjectsManager * CRenderableObjectsLayersManager::GetRenderableObjectManager(const std::string &layer){

	const std::string l_LayerName = layer;
	if (l_LayerName != "")
		return CVectorMapManager::GetResource(l_LayerName);
	else
		return m_DefaultRenderableObjectManager;

}

void CRenderableObjectsLayersManager::SetLayerEnabled(const std::string& LayerName, bool enabled)
{
	CRenderableObjectsManager* l_RenderableObjectManager = CVectorMapManager::GetResource(LayerName);
	if (l_RenderableObjectManager!=NULL)
	{
		l_RenderableObjectManager->SetEnabled(enabled);
	}
}

bool CRenderableObjectsLayersManager::GetLayerEnabled(const std::string& LayerName)
{
	CRenderableObjectsManager* l_RenderableObjectManager = CVectorMapManager::GetResource(LayerName);
	if (l_RenderableObjectManager!=NULL)
	{
		return l_RenderableObjectManager->GetEnabled();
	}

	return false;
}

void CRenderableObjectsLayersManager::EnablePhysicLayer(const std::string& LayerName, bool enabled)
{
	CRenderableObjectsManager* l_RenderableObjectManager = CVectorMapManager::GetResource(LayerName);

	if (l_RenderableObjectManager!=NULL)
	{
		l_RenderableObjectManager->EnablePhysicLayer(enabled);
	}
}