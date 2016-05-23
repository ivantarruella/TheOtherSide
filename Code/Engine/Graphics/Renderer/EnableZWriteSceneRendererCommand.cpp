#include "EnableZWriteSceneRendererCommand.h"
#include "RenderManager.h"

CEnableZWriteSceneRendererCommand::CEnableZWriteSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CEnableZWriteSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
}