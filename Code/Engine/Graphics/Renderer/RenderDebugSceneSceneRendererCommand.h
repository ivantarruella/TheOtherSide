#pragma once
#ifndef __CLASS_RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_SCENE_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderableObjectsManager;
class CRenderManager;

class CRenderDebugSceneSceneRendererCommand : public CSceneRendererCommand
{
private:
	CRenderableObjectsManager * m_Layer;

public:
	CRenderDebugSceneSceneRendererCommand();
	CRenderDebugSceneSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif