#include "RenderDebugIASceneRendererCommand.h"
#include "NodeManager.h"
#include "CoverManager.h"
#include "Base.h"

CRenderDebugIASceneRendererCommand::CRenderDebugIASceneRendererCommand(CXMLTreeNode& atts)
	: CSceneRendererCommand(atts)
{
}

void CRenderDebugIASceneRendererCommand::Execute(CRenderManager& RM)
{
	// IA
	CORE->GetNodeManager()->RenderDebug();
	CORE->GetCoverManager()->RenderDebug();
}
