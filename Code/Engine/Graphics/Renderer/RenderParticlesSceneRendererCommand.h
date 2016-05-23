#pragma once
#ifndef __CLASS_RENDER_PARTICLES_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_PARTICLES_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CRenderParticlesSceneRendererCommand : public CSceneRendererCommand
{

public:
	CRenderParticlesSceneRendererCommand();
	CRenderParticlesSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};
#endif
