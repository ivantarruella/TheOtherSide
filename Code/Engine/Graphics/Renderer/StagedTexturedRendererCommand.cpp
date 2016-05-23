#include "StagedTexturedRendererCommand.h"

CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode &atts)
	: CSceneRendererCommand(atts)
{
}

CStagedTexturedRendererCommand::~CStagedTexturedRendererCommand()
{
	m_StageTextures.clear();
}

void CStagedTexturedRendererCommand::ActivateTextures()
{
	for (size_t t=0; t<m_StageTextures.size(); ++t)
	{
		m_StageTextures[t].Activate();
	}
}

void CStagedTexturedRendererCommand::AddStageTexture(int StageId, CTexture *Texture)
{
	m_StageTextures.push_back(CStageTexture(StageId, Texture));
}
