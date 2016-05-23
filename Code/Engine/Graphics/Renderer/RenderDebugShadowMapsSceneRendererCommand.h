#pragma once
#ifndef __CLASS_RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__
#define __CLASS_RENDER_DEBUG_SHADOW_MAPS_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"
#include "Vector2.h"

class CRenderManager;

class CRenderDebugShadowMapsSceneRendererCommand : public CSceneRendererCommand
{
protected:
	float m_ScreenWidth, m_ScreenHeight;
	void AdvanceTexturePosition(Vect2f &Position);

public:
	CRenderDebugShadowMapsSceneRendererCommand();
	CRenderDebugShadowMapsSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
