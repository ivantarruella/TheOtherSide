#include "DrawQuadRendererCommand.h"
#include "RenderManager.h"
#include "EffectTechnique.h"
#include "TextureManager.h"
#include "RenderableObjectsTechniqueManager.h"
#include "VertexType.h"
#include "Base.h"

CDrawQuadRendererCommand::CDrawQuadRendererCommand(CXMLTreeNode &atts)
	: CStagedTexturedRendererCommand(atts)
{
	int l_count_textures = atts.GetNumChildren();
	for (int i=0; i<l_count_textures; ++i)
	{
		int l_StageId = atts(i).GetIntProperty("stage_id", 0);
		std::string l_File = atts(i).GetPszProperty("file","");

		CTexture* l_Texture = CORE->GetTextureManager()->GetTexture(l_File);
		if (l_Texture != NULL)
		{
			CStagedTexturedRendererCommand::AddStageTexture(l_StageId, l_Texture);
		}
	}
	
	bool l_TextureWidhtAsFrameBuffer = atts.GetBoolProperty("texture_width_as_frame_buffer", true);
	uint32 w,h;
	if (l_TextureWidhtAsFrameBuffer)
	{
		CORE->GetRenderManager()->GetWidthAndHeight(w, h);
	}
	else
	{
		w = atts.GetIntProperty("width", 0);
		h = atts.GetIntProperty("height", 0);
	}

	m_Rect.top=m_Rect.left=(long)0.0;
	m_Rect.right=w-1;
	m_Rect.bottom=h-1;
}

void CDrawQuadRendererCommand::Execute(CRenderManager &RM)
{
	ActivateTextures();

	CRenderableObjectTechnique *l_Rot= CORE->GetRenderableObjectsTechniqueManager()->GetResource(CORE->GetRenderableObjectsTechniqueManager()->GetRenderableObjectTechniqueNameByVertexType(SCREEN_TEXTURE_COLORED_VERTEX::GetVertexType()),"");
	if (l_Rot != NULL)
		DrawColoredQuad2DTexturedInPixelsByEffectTechnique(&RM, l_Rot->GetEffectTechnique(), m_Rect, colWHITE, NULL);
}

void DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CRenderManager *RM,
	CEffectTechnique *EffectTechnique, RECT Rect, CColor Color, CTexture *Texture,
	float U0, float V0, float U1, float V1)
{
	if (EffectTechnique!=NULL)
	{
		EffectTechnique->BeginRender();
		CEffect* l_Effect = EffectTechnique->GetEffect();
		if (l_Effect != NULL)
		{
			LPD3DXEFFECT l_D3DEffect=l_Effect->GetD3DEffect();
			if(l_D3DEffect!=NULL)
			{
				l_D3DEffect->SetTechnique(EffectTechnique->GetD3DTechnique());
				UINT l_NumPasses;
				l_D3DEffect->Begin(&l_NumPasses, 0);
				for (UINT iPass = 0; iPass < l_NumPasses; iPass++)
				{
					l_D3DEffect->BeginPass(iPass);
					RM->DrawColoredQuad2DTexturedInPixels(Rect,Color,Texture, U0, V0, U1, V1);
					l_D3DEffect->EndPass();
				}
				l_D3DEffect->End();
			}
		}
	}
}