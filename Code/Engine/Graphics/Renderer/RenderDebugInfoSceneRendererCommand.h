#pragma once
#ifndef __CLASS_RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_INFO_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CRenderDebugInfoSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugInfoSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif