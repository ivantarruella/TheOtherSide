#pragma once
#ifndef __CLASS_ENABLE_ZTEST_SCENE_RENDERER_COMMAND_H__
#define __CLASS_ENABLE_ZTEST_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CEnableZTestSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEnableZTestSceneRendererCommand();
	CEnableZTestSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};
#endif