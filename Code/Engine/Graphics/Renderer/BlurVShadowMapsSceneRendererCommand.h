#pragma once
#ifndef __CLASS_BLUR_V_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_BLUR_V_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CBlurVShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CBlurVShadowMapsSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
