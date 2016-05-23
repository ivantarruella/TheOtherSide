#pragma once
#ifndef __CLASS_DISABLE_ZWRITE_SCENE_RENDERER_COMMAND_H__
#define __CLASS_DISABLE_ZWRITE_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CRenderManager;

class CDisableZWriteSceneRendererCommand : public CSceneRendererCommand
{
public:
	CDisableZWriteSceneRendererCommand();
	CDisableZWriteSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif

