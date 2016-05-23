#pragma once
#ifndef __CLASS_RENDER_DEBUG_CAMERA_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_CAMERA_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;


class CRenderDebugCameraSceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugCameraSceneRendererCommand(CXMLTreeNode &atts);
	void Execute(CRenderManager &RM);
};


#endif
