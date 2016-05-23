#include "RenderDebugSceneSceneRendererCommand.h"
#include "RenderableObjectsLayersManager.h"
#include "RenderManager.h"
#include "Base.h"

CRenderDebugSceneSceneRendererCommand::CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
	std::string l_LayerName = atts.GetPszProperty("layer","");
	m_Layer = CORE->GetRenderableObjectsLayersManager()->GetResource(l_LayerName);
}

void CRenderDebugSceneSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (m_Layer != NULL)
	{
		m_Layer->RenderDebug(&RM);
	}
}
