#pragma once
#ifndef __CLASS_DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H__
#define __CLASS_DEFERRED_SHADING_SCENE_RENDERER_COMMAND_H__

#include "StagedTexturedRendererCommand.h"


class CRenderableObjectTechnique;
class CRenderManager;
class CLight;

class CDeferredShadingSceneRendererCommand : public CStagedTexturedRendererCommand
{
private:
	CRenderableObjectTechnique *m_RenderableObjectTechnique;
	RECT m_Rect;

	RECT DetermineClipRect(CLight* Light);

public:
	CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts);
	
	void Execute(CRenderManager &RM);
};

#endif