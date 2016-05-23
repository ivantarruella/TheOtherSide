#pragma once
#ifndef __CLASS_UNSET_SCISSOR_RECT_SCENE_RENDERER_COMMAND_H__
#define __CLASS_UNSET_SCISSOR_RECT_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"
#include "d3d9.h"

class CRenderManager;

class CUnsetScissorRectSceneRendererCommand : public CSceneRendererCommand
{

public:
	CUnsetScissorRectSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif