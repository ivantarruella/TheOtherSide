#include "SetRenderTargetSceneRendererCommand.h"
#include "SceneRendererCommand.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "Logger.h"
#include "Exception.h"
#include "Base.h"

CSetRenderTargetSceneRendererCommand::CSetRenderTargetSceneRendererCommand(CXMLTreeNode &atts)
	: CStagedTexturedRendererCommand(atts)
{
	int l_count_targets = atts.GetNumChildren();
	m_ViewportStencil = atts.GetBoolProperty("use_view_port_depth_stencil_buffer", false);
	for (int j=0; j<l_count_targets; ++j)
	{
		int l_StageId = atts(j).GetIntProperty("stage_id", 0);
		std::string l_Name = atts(j).GetPszProperty("name","");
		bool l_TextureWidhtAsFrameBuffer = atts(j).GetBoolProperty("texture_width_as_frame_buffer", false);

		if(m_ViewportStencil && !l_TextureWidhtAsFrameBuffer)
		{
			std::string msg_error = "CSetRenderTargetSceneRendererCommand::FormatType->Error, viewport stencil true y texture width false: "+l_Name;
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			throw CException(__FILE__, __LINE__, msg_error);
		}
		std::string l_FormatType = atts(j).GetPszProperty("format_type", "");

		uint32 w,h;
		if (l_TextureWidhtAsFrameBuffer)
		{
			CORE->GetRenderManager()->GetWidthAndHeight(w, h);
		}
		else
		{
			w = atts(j).GetIntProperty("width", 0);
			h = atts(j).GetIntProperty("height", 0);
		}

		uint32 w1, h1;
		CORE->GetRenderManager()->GetWidthAndHeight(w1, h1);
		assert(w <= w1 && h <= h1); 

		CTexture* l_Texture = CORE->GetTextureManager()->GetTexture(l_Name);
		bool l_bError = l_Texture->Create(l_Name, w, h, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::GetFormatTypeFromString(l_FormatType));
		if (!l_bError && l_Texture != NULL)
		{
			m_StageTextures.push_back(CStageTexture(l_StageId, l_Texture));
		}
		CORE->GetTextureManager()->RemoveResource(l_Name);
		CORE->GetTextureManager()->AddResource(l_Name, l_Texture);
	}
}

void CSetRenderTargetSceneRendererCommand::Execute(CRenderManager &RM)
{
	for(unsigned int i = 0; i<m_StageTextures.size(); ++i)
	{
		m_StageTextures[i].m_Texture->SetAsRenderTarget(m_StageTextures[i].m_StageId, m_ViewportStencil);
	}
}

void CSetRenderTargetSceneRendererCommand::UnsetRenderTarget()
{
	for(unsigned int i = 0; i<m_StageTextures.size(); ++i)
	{
		m_StageTextures[i].m_Texture->UnsetAsRenderTarget(m_StageTextures[i].m_StageId);
	}
}