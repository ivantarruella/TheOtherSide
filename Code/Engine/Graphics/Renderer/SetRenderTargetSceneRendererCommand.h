#pragma once
#ifndef __CLASS_SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H__
#define __CLASS_SET_RENDER_TARGET_SCENE_RENDERER_COMMAND_H__

#include "StagedTexturedRendererCommand.h"

class CRenderManager;

class CSetRenderTargetSceneRendererCommand : public CStagedTexturedRendererCommand
{
protected:
	bool						m_ViewportStencil;
public:
	CSetRenderTargetSceneRendererCommand(CXMLTreeNode &atts);

	void Execute(CRenderManager &RM);
	void UnsetRenderTarget();
};

#endif
