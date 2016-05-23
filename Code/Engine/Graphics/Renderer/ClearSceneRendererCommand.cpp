#include "ClearSceneRendererCommand.h"
#include "RenderManager.h"

CClearSceneRendererCommand::CClearSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
	m_Color = atts.GetBoolProperty("color", false);
	m_Depth = atts.GetBoolProperty("depth", false);
	m_Stencil = atts.GetBoolProperty("stencil", false);

	m_ClearMask= (m_Color ? D3DCLEAR_TARGET : 0) | 
		(m_Depth ? D3DCLEAR_ZBUFFER : 0) |
		(m_Stencil ? D3DCLEAR_STENCIL : 0);
}

void CClearSceneRendererCommand::Execute(CRenderManager &RM)
{
/*
#ifdef _DEBUG 
	uint32 red   	 = (uint32) (RM.GetBackBufferColor_debug().GetRed() * 255);		
    uint32 green     = (uint32) (RM.GetBackBufferColor_debug().GetGreen() * 255);
    uint32 blue   	 = (uint32) (RM.GetBackBufferColor_debug().GetBlue()* 255);
#else 
    uint32 red   	 = (uint32) (RM.GetBackBufferColor_release().GetRed() * 255);
    uint32 green    = (uint32) (RM.GetBackBufferColor_release().GetGreen() * 255);
    uint32 blue   	 = (uint32) (RM.GetBackBufferColor_release().GetBlue()* 255);
#endif
*/
	//RM.GetDevice()->Clear( 0, NULL, m_ClearMask, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
	RM.GetDevice()->Clear( 0, NULL, m_ClearMask, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0 );
}

