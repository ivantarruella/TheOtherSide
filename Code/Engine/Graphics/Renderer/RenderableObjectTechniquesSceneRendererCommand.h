#pragma once
#ifndef __CLASS_RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDERABLE_OBJECT_TECHNIQUES_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CPoolRenderableObjectTechnique;
class CRenderManager;

class CRenderableObjectTechniquesSceneRendererCommand : public CSceneRendererCommand
{
private:
	CPoolRenderableObjectTechnique *m_PoolRenderableObjectTechnique;

public:
	CRenderableObjectTechniquesSceneRendererCommand();
	CRenderableObjectTechniquesSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
