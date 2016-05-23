#pragma once
#ifndef __CLASS_BEGIN_RENDER_SCENE_RENDERER_COMMAND_H__
#define __CLASS_BEGIN_RENDER_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CBeginRenderSceneRendererCommand : public CSceneRendererCommand
{
public:
	CBeginRenderSceneRendererCommand();
	CBeginRenderSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif