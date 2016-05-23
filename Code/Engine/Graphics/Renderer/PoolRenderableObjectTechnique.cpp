#include "PoolRenderableObjectTechnique.h"
#include "RenderableObjectsTechniqueManager.h"
#include "EffectTechnique.h"
#include "EffectManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger.h"
#include "Exception.h"
#include "Core.h"
#include "Base.h"


CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique()
{
}

CPoolRenderableObjectTechnique::CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode)
{
	CRenderableObjectsTechniqueManager *l_ROTM = CORE->GetRenderableObjectsTechniqueManager();
	CEffectManager* l_EM = CORE->GetEffectManager();

	std::string l_PoolName = TreeNode.GetPszProperty("name","");
	m_Name=l_PoolName;

	if (TreeNode.Exists()){
		int l_count_objects = TreeNode.GetNumChildren();
		for (int i = 0; i < l_count_objects; ++i)
		{
			if (!TreeNode(i).IsComment())
			{
				CXMLTreeNode &l_ChildNode=TreeNode(i);
				std::string l_EffectTechniqueName=l_ChildNode.GetPszProperty("technique");
				CRenderableObjectTechnique *l_ROTOnROTM;
				std::string l_ROTName;
				if((std::string)l_ChildNode.GetName()=="default_technique")
				{
					l_ROTName=l_ROTM->GetRenderableObjectTechniqueNameByVertexType(l_ChildNode.GetIntProperty("vertex_type"));


				}
				else if((std::string)l_ChildNode.GetName()=="technique")
				{
					l_ROTName=l_ChildNode.GetPszProperty("name");

				}
				l_ROTOnROTM=l_ROTM->GetResource(l_ROTName,l_EffectTechniqueName);	
				AddElement(l_ROTName, l_EffectTechniqueName, l_ROTOnROTM);
			}
		}
	}
}

CPoolRenderableObjectTechnique::~CPoolRenderableObjectTechnique()
{
	Destroy();
}

void CPoolRenderableObjectTechnique::Destroy()
{
	for(unsigned int i=0; i<m_RenderableObjectTechniqueElements.size(); ++i)
	{
		CHECKED_DELETE(m_RenderableObjectTechniqueElements[i]);
	}
	m_RenderableObjectTechniqueElements.clear();
}

void CPoolRenderableObjectTechnique::AddElement(const std::string &Name, const std::string &TechniqueName,CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager)
{
	CEffectTechnique *l_Technique = CORE->GetEffectManager()->GetEffectTechnique(TechniqueName);
	CPoolRenderableObjectTechniqueElement * l_Element;
	l_Element = new CPoolRenderableObjectTechniqueElement(Name, l_Technique, ROTOnRenderableObjectTechniqueManager);
	m_RenderableObjectTechniqueElements.push_back(l_Element);
}

void CPoolRenderableObjectTechnique::Apply()
{
	for(unsigned int i=0; i<m_RenderableObjectTechniqueElements.size(); ++i)
	{
		m_RenderableObjectTechniqueElements[i]->m_OnRenderableObjectTechniqueManager->SetEffectTechnique(m_RenderableObjectTechniqueElements[i]->m_RenderableObjectTechnique.GetEffectTechnique());
	}
}
