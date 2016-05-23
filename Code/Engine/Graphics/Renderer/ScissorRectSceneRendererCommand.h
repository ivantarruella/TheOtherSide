#pragma once
#ifndef __CLASS_SCISSOR_RECT_SCENE_RENDERER_COMMAND_H__
#define __CLASS_SCISSOR_RECT_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"
#include "d3d9.h"

class CRenderManager;

class CScissorRectSceneRendererCommand : public CSceneRendererCommand
{
protected:
	RECT m_Rect;

public:
	CScissorRectSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif