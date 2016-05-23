#include "UnsetScissorRectSceneRendererCommand.h"
#include "RenderManager.h"

CUnsetScissorRectSceneRendererCommand::CUnsetScissorRectSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{

}

void CUnsetScissorRectSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

