#include "CaptureFrameBufferSceneRendererCommand.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "Base.h"

CCaptureFrameBufferSceneRendererCommand::CCaptureFrameBufferSceneRendererCommand(CXMLTreeNode &atts)
	: CStagedTexturedRendererCommand(atts)
{
	int l_count_targets = atts.GetNumChildren();
	for (int j=0; j<l_count_targets; ++j)
	{
		int l_StageId = atts(j).GetIntProperty("stage_id", 0);
		std::string l_Name = atts(j).GetPszProperty("name","");
		bool l_TextureWidhtAsFrameBuffer = atts(j).GetBoolProperty("texture_width_as_frame_buffer", false);
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

CCaptureFrameBufferSceneRendererCommand::~CCaptureFrameBufferSceneRendererCommand()
{
}

void CCaptureFrameBufferSceneRendererCommand::Execute(CRenderManager &RM)
{
	for (size_t t=0;t<m_StageTextures.size();++t)
	{
		m_StageTextures[t].m_Texture->CaptureFrameBuffer(m_StageTextures[t].m_StageId);
	}
}