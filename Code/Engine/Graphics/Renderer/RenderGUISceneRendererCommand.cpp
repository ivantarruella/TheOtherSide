#include "RenderGUISceneRendererCommand.h"
#include "RenderManager.h"
#include "GUIManager.h"
#include "Base.h"

CRenderGUISceneRendererCommand::CRenderGUISceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
	m_bActive = atts.GetBoolProperty("active",false);
}

void CRenderGUISceneRendererCommand::Execute(CRenderManager &RM)
{
	if (m_bActive)
	{
		CORE->GetGUIManager()->Render(&RM, CORE->GetFontManager());
	}
}
