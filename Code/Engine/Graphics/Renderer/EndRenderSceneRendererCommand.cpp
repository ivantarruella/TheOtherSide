#include "EndRenderSceneRendererCommand.h"
#include "RenderManager.h"

CEndRenderSceneRendererCommand::CEndRenderSceneRendererCommand (CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CEndRenderSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->EndScene();
}