#pragma once
#ifndef __CLASS_CLEAR_SCENE_RENDERER_COMMAND_H__
#define __CLASS_CLEAR_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"
#include "d3d9.h"

class CRenderManager;

class CClearSceneRendererCommand : public CSceneRendererCommand
{
protected:
	bool m_Color;
	bool m_Depth;
	bool m_Stencil;
	DWORD m_ClearMask;

public:
	CClearSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif