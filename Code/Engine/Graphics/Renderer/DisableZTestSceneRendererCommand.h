#pragma once
#ifndef __CLASS_DISABLE_ZTEST_SCENE_RENDERER_COMMAND_H__
#define __CLASS_DISABLE_ZTEST_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CDisableZTestSceneRendererCommand : public CSceneRendererCommand
{
public:
	CDisableZTestSceneRendererCommand();
	CDisableZTestSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif
