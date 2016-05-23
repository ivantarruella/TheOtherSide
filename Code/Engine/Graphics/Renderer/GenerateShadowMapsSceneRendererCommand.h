#pragma once
#ifndef __CLASS_GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_GENERATE_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CGenerateShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CGenerateShadowMapsSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
