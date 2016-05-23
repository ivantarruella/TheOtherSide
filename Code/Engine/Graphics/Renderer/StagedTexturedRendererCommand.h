#pragma once
#ifndef __CLASS_STAGED_TEXTURED_RENDERER_COMMAND_H__
#define __CLASS_STAGED_TEXTURED_RENDERER_COMMAND_H__

#include "SceneRendererCommand.h"
#include "Texture.h"
#include <vector>

class CTexture;
class CRenderManager;

class CStagedTexturedRendererCommand : public CSceneRendererCommand
{
protected:
	class CStageTexture
	{
	public:
		int m_StageId;
		CTexture *m_Texture;
		CStageTexture(int StageId, CTexture *Texture)
		{
			m_StageId=StageId;
			m_Texture=Texture;
		}

		void Activate()
		{
			m_Texture->Activate(m_StageId);
		}
	};

	std::vector<CStageTexture> m_StageTextures;

public:
	CStagedTexturedRendererCommand(CXMLTreeNode &atts);
	virtual ~CStagedTexturedRendererCommand();


	void ActivateTextures();
	void AddStageTexture(int StageId, CTexture *Texture);
	virtual void Execute(CRenderManager &RM) = 0;
};
#endif
