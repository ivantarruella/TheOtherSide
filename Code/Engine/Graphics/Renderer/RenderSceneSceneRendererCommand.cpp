#include "RenderSceneSceneRendererCommand.h"
#include "RenderableObjectsLayersManager.h"
#include "RenderManager.h"
#include "Process.h"
#include "Base.h"

CRenderSceneSceneRendererCommand::CRenderSceneSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts), m_bForwardRendering(false)
{
	std::string l_Layer = atts.GetPszProperty("layer","");
	bool l_Active = atts.GetBoolProperty("active", false);
	m_bForwardRendering = atts.GetBoolProperty("forward", false);

	if (l_Layer!="")
	{
		m_Layer = CORE->GetRenderableObjectsLayersManager()->GetResource(l_Layer);
	}
}

void CRenderSceneSceneRendererCommand::Execute(CRenderManager &RM)
{
	if (!CORE->GetProcess()->GetStartGame())
		return;		// evita pintado de layers en GUI
	
	if (m_Layer != NULL && m_Layer->GetEnabled())
	{
		m_Layer->Render(&RM, m_bForwardRendering);
	}
}