#include "SceneRendererCommand.h"
#include "RenderManager.h"


CSceneRendererCommand::CSceneRendererCommand(CXMLTreeNode &atts)
{
	SetName(atts.GetPszProperty("name","", false));
}

CSceneRendererCommand::~CSceneRendererCommand()
{
}
