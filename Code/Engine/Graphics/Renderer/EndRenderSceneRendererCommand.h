#pragma once
#ifndef __CLASS_END_RENDER_SCENE_RENDERER_COMMAND_H__
#define __CLASS_END_RENDER_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CEndRenderSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEndRenderSceneRendererCommand ();
	CEndRenderSceneRendererCommand (CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};
#endif



