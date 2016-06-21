#pragma once
#ifndef __CLASS_RENDER_DEBUG_PHYSX_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_PHYSX_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CRenderDebugPhysXSceneRendererCommand : public CSceneRendererCommand
{
private:

public:
	CRenderDebugPhysXSceneRendererCommand();
	CRenderDebugPhysXSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
