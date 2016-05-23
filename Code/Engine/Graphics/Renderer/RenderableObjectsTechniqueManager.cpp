
#include "RenderableObjectsTechniqueManager.h"
#include "Logger.h"
#include "Exception.h"
#include "PoolRenderableObjectTechnique.h"
#include "EffectManager.h"
#include "RenderManager.h"
#include <sstream>
#include "Renderer\RenderableObjectTechniquesSceneRendererCommand.h"
#include "Renderer\SceneRendererCommandManager.h"
#include "Base.h"

CRenderableObjectsTechniqueManager::~CRenderableObjectsTechniqueManager()
{
	this->Destroy();
}

void CRenderableObjectsTechniqueManager::InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName)
{
	//Solo hay un nombre de ROT (deffault) para cada vertice, se asigna la technique despues 
	if(CMapManager<CRenderableObjectTechnique>::GetResource(ROTName)==NULL)
	{
		CRenderableObjectTechnique *l_ROT= new CRenderableObjectTechnique(ROTName, NULL);
		AddResource(ROTName,l_ROT);
	}
}

void CRenderableObjectsTechniqueManager::Destroy()
{
	m_PoolRenderableObjectTechniques.Destroy();
	CMapManager::Destroy();
}

void CRenderableObjectsTechniqueManager::Reload(){
	if (m_FileName != "")
	{
		CRenderableObjectsTechniqueManager::Destroy();
		Load(m_FileName);
	}
}

void CRenderableObjectsTechniqueManager::Load(const std::string &FileName)
{
	m_FileName=FileName;
	CXMLTreeNode parser;
	if (!parser.LoadFile(FileName.c_str()))
	{
		std::string msg_error = "CRenderableObjectsTechniqueManager::Load->Error al intentar leer el archivo: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	
	// Obtenemos el nodo "Renderable_Object_Techniques"
	CXMLTreeNode  l_RenderableObjectTechniquesNode = parser["renderable_object_techniques"];
	if (l_RenderableObjectTechniquesNode.Exists())
	{
		// Obtenemos los datos para todos los renderable object
		int l_count_objects = l_RenderableObjectTechniquesNode.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			if (!l_RenderableObjectTechniquesNode(i).IsComment())
			{
				const char* l_Name = l_RenderableObjectTechniquesNode(i).GetPszProperty("name","");

				//Deffault techniques
				int l_count_techniques = l_RenderableObjectTechniquesNode(i).GetNumChildren();
				for (int j = 0; j < l_count_techniques; ++j)
				{
					if (!l_RenderableObjectTechniquesNode(i)(j).IsComment())
					{
						std::string l_ROTName;
						if((std::string)l_RenderableObjectTechniquesNode(i)(j).GetName()=="default_technique")
						{
							unsigned int l_VertexType = l_RenderableObjectTechniquesNode(i)(j).GetIntProperty("vertex_type",0);
							l_ROTName = GetRenderableObjectTechniqueNameByVertexType(l_VertexType);
						}
						else if((std::string)l_RenderableObjectTechniquesNode(i)(j).GetName()=="technique")
						{
							l_ROTName = l_RenderableObjectTechniquesNode(i)(j).GetPszProperty("name","");
						}
					
						const char* l_TechniqueName = l_RenderableObjectTechniquesNode(i)(j).GetPszProperty("technique","");
						InsertRenderableObjectTechnique(l_ROTName,l_TechniqueName);
					}
				}

				CPoolRenderableObjectTechnique *l_PoolRenderableObjectTechnique=new CPoolRenderableObjectTechnique(l_RenderableObjectTechniquesNode(i));
				m_PoolRenderableObjectTechniques.AddResource(l_Name, l_PoolRenderableObjectTechnique);
				
			}
		}
	}
	
		std::string msg_info = "CRenderableObjectsTechniqueManager::Load->Archivo cargado: " + FileName;
		LOGGER->AddNewLog(ELL_INFORMATION, msg_info.c_str());
				
}

std::string CRenderableObjectsTechniqueManager::GetRenderableObjectTechniqueNameByVertexType(unsigned int VertexType)
{
	std::string   name = "DeffaultROTTechnique_";

	std::stringstream ss;
	ss << VertexType;

	std::string  res=name+ss.str();
	return res;
}

CVectorMapManager<CPoolRenderableObjectTechnique>	& CRenderableObjectsTechniqueManager::GetPoolRenderableObjectTechniques() 
{
	return m_PoolRenderableObjectTechniques;
}