#pragma once
#ifndef __CLASS_RENDER_SCENE_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_SCENE_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderableObjectsManager;
class CRenderManager;

class CRenderSceneSceneRendererCommand : public CSceneRendererCommand
{
private:
	CRenderableObjectsManager *m_Layer;
	bool m_bForwardRendering;

public:
	CRenderSceneSceneRendererCommand();
	CRenderSceneSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif