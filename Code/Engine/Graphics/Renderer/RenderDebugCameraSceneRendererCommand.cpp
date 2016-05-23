#include "RenderDebugCameraSceneRendererCommand.h"
#include "RenderManager.h"
#include "Matrix44.h"
#include "Process.h"
#include "Console.h"
#include "Camera\Camera.h"
#include "Base.h"

CRenderDebugCameraSceneRendererCommand::CRenderDebugCameraSceneRendererCommand(CXMLTreeNode &atts)
: CSceneRendererCommand(atts)
{
}


void CRenderDebugCameraSceneRendererCommand::Execute(CRenderManager &RM)
{
	if(CORE->GetCamera()!=NULL)
		CORE->GetCamera()->RenderDebug(RM);
}
