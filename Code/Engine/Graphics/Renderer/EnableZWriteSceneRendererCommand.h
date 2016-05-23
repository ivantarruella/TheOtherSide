#pragma once
#ifndef __CLASS_ENABLE_ZWRITE_SCENE_RENDERER_COMMAND_H__
#define __CLASS_ENABLE_ZWRITE_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CEnableZWriteSceneRendererCommand : public CSceneRendererCommand
{
public:
	CEnableZWriteSceneRendererCommand();
	CEnableZWriteSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif