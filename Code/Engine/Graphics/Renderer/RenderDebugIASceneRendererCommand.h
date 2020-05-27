#pragma once
#ifndef __CLASS_RENDER_DEBUG_IA_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_IA_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CRenderDebugIASceneRendererCommand : public CSceneRendererCommand
{
public:
	CRenderDebugIASceneRendererCommand(CXMLTreeNode& atts);

	void Execute(CRenderManager& RM);
};

#endif
