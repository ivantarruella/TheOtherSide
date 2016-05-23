#pragma once
#ifndef __CLASS_RENDER_DEBUG_LIGHTS_SCENE_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_LIGHTS_SCENE_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CRenderDebugLightsSceneSceneRendererCommand : public CSceneRendererCommand
{
private:

public:
	CRenderDebugLightsSceneSceneRendererCommand();
	CRenderDebugLightsSceneSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
