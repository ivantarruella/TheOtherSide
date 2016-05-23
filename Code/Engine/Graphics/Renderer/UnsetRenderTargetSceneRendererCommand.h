#pragma once
#ifndef __CLASS_UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H__
#define __CLASS_UNSET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"

class CSetRenderTargetSceneRendererCommand;
class CRenderManager;

class CUnsetRenderTargetSceneRendererCommand : public CSceneRendererCommand
{
private:
	CSetRenderTargetSceneRendererCommand *m_SetRenderTargetRendererCommand;

public:
	CUnsetRenderTargetSceneRendererCommand(CSetRenderTargetSceneRendererCommand *SetRenderTargetRendererCommand, CXMLTreeNode &atts);
	void Execute(CRenderManager &RM);
};
#endif