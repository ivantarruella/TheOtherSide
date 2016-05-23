#pragma once
#ifndef __CLASS_PRESENT_SCENE_RENDERER_COMMAND_H__
#define __CLASS_PRESENT_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CPresentSceneRendererCommand : public CSceneRendererCommand
{
public:
	CPresentSceneRendererCommand();
	CPresentSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif