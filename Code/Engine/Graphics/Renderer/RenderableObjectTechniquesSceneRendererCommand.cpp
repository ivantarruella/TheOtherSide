#include "RenderableObjectTechniquesSceneRendererCommand.h"
#include "RenderableObjectsTechniqueManager.h"
#include "RenderManager.h"
#include "Base.h"

CRenderableObjectTechniquesSceneRendererCommand::CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
	std::string l_PROTName = atts.GetPszProperty("pool","");
	if (l_PROTName!="")
	{
		m_PoolRenderableObjectTechnique = CORE->GetRenderableObjectsTechniqueManager()->GetPoolRenderableObjectTechniques().GetResource(l_PROTName);
	}
}

void CRenderableObjectTechniquesSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (m_PoolRenderableObjectTechnique != NULL)
	{
		m_PoolRenderableObjectTechnique->Apply();
	}
}
