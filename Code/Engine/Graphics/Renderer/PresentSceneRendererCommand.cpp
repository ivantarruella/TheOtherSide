#include "PresentSceneRendererCommand.h"
#include "RenderManager.h"

CPresentSceneRendererCommand::CPresentSceneRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

void CPresentSceneRendererCommand::Execute(CRenderManager &RM)
{
    // Present the backbuffer contents to the display
	RM.GetDevice()->Present( NULL, NULL, NULL, NULL );
}