#pragma once
#ifndef __CLASS_SETUP_MATRICES_SCENE_RENDERER_COMMAND_H__
#define __CLASS_SETUP_MATRICES_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"


class CRenderManager;

class CSetupMatricesSceneRendererCommand : public CSceneRendererCommand
{
public:
	CSetupMatricesSceneRendererCommand();
	CSetupMatricesSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
};

#endif