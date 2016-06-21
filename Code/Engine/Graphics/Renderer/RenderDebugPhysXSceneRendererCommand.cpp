#include "RenderDebugPhysXSceneRendererCommand.h"
#include "RenderManager.h"
#include "include\PhysicsManager.h"
#include "Base.h"

CRenderDebugPhysXSceneRendererCommand::CRenderDebugPhysXSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CRenderDebugPhysXSceneRendererCommand::Execute(CRenderManager &RM)
{
	CORE->GetPhysicsManager()->DebugRender(&RM);
}
