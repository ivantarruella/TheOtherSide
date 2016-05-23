#pragma once
#ifndef __CLASS_RENDER_MIRRORS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_MIRRORS_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CRenderMirrorsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderMirrorsSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
