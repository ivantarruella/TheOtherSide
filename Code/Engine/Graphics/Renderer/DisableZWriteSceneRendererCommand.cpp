#include "DisableZWriteSceneRendererCommand.h"
#include "RenderManager.h"

CDisableZWriteSceneRendererCommand::CDisableZWriteSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CDisableZWriteSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
}