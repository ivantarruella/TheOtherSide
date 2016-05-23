#pragma once
#ifndef __CLASS_DRAW_QUAD_RENDERER_COMMAND_H__
#define __CLASS_DRAW_QUAD_RENDERER_COMMAND_H__

#include "StagedTexturedRendererCommand.h"
#include "Color.h"

class CRenderManager;
class CEffectTechnique;

class CDrawQuadRendererCommand : public CStagedTexturedRendererCommand
{
protected:
	CColor m_Color;
	RECT m_Rect;

public:
	CDrawQuadRendererCommand();
	CDrawQuadRendererCommand(CXMLTreeNode &atts);
	void Execute(CRenderManager &RM);
};

void DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CRenderManager *RM, CEffectTechnique *EffectTechnique, RECT Rect, CColor Color, CTexture *Texture,
	float U0=0.0f, float V0=0.0f, float U1=1.0f, float V1=1.0f);

#endif
