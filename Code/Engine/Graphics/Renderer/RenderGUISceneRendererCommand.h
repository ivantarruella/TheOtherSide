#pragma once
#ifndef __CLASS_RENDER_GUI_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_GUI_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CRenderGUISceneRendererCommand : public CSceneRendererCommand
{
private:
	bool m_bActive;

public:
	CRenderGUISceneRendererCommand();
	CRenderGUISceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};
#endif
