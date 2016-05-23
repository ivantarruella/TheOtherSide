#include "DisableZTestSceneRendererCommand.h"
#include "RenderManager.h"

CDisableZTestSceneRendererCommand::CDisableZTestSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CDisableZTestSceneRendererCommand::Execute(CRenderManager &RM)
{
    RM.GetDevice()->SetRenderState( D3DRS_ZENABLE,D3DZB_FALSE);
}