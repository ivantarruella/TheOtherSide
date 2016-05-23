#include "EnableZTestSceneRendererCommand.h"
#include "RenderManager.h"

CEnableZTestSceneRendererCommand::CEnableZTestSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CEnableZTestSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	RM.GetDevice()->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
}