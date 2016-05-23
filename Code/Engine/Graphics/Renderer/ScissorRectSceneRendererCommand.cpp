#include "ScissorRectSceneRendererCommand.h"
#include "RenderManager.h"
#include "Base.h"

CScissorRectSceneRendererCommand::CScissorRectSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
	m_Rect.left = atts.GetIntProperty("left",0);
	m_Rect.top = atts.GetIntProperty("top",0);
	m_Rect.right = atts.GetIntProperty("right",0);
	m_Rect.bottom = atts.GetIntProperty("bottom",0);
}

void CScissorRectSceneRendererCommand::Execute(CRenderManager &RM)
{
	RM.GetDevice()->SetScissorRect(&m_Rect);
	RM.GetDevice()->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
}

