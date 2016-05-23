#include "RenderDebugLightsSceneSceneRendererCommand.h"
#include "RenderManager.h"
#include "LightManager.h"
#include "Base.h"

CRenderDebugLightsSceneSceneRendererCommand::CRenderDebugLightsSceneSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CRenderDebugLightsSceneSceneRendererCommand::Execute(CRenderManager &RM)
{
	CORE->GetLightManager()->Render(&RM);
}
