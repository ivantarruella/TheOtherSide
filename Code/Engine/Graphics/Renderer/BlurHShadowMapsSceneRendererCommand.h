#pragma once
#ifndef __CLASS_BLUR_H_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_BLUR_H_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CBlurHShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
public:
	CBlurHShadowMapsSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
