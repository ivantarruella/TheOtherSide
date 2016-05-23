#pragma once
#ifndef __CLASS_POOL_RENDERABLE_OBJECT_TECHNIQUE_H__
#define __CLASS_POOL_RENDERABLE_OBJECT_TECHNIQUE_H__

#include "RenderableObjectTechnique.h"
#include "XML\XMLTreeNode.h"
#include <vector>
#include <string>

class CEffectTechnique;

class CPoolRenderableObjectTechnique : public CNamed
{
private:

	class CPoolRenderableObjectTechniqueElement
	{
	public:
		CRenderableObjectTechnique m_RenderableObjectTechnique;
		CRenderableObjectTechnique *m_OnRenderableObjectTechniqueManager;

		CPoolRenderableObjectTechniqueElement(const std::string &Name, CEffectTechnique *EffectTechnique, CRenderableObjectTechnique *OnRenderableObjectTechniqueManager)
		{
			m_RenderableObjectTechnique.SetName(Name);
			m_RenderableObjectTechnique.SetEffectTechnique(EffectTechnique);
			m_OnRenderableObjectTechniqueManager = OnRenderableObjectTechniqueManager;
		}
	};

	std::vector<CPoolRenderableObjectTechniqueElement *> m_RenderableObjectTechniqueElements;

public:
	CPoolRenderableObjectTechnique();
	CPoolRenderableObjectTechnique(CXMLTreeNode &TreeNode);
	virtual ~CPoolRenderableObjectTechnique();

	void Destroy();
	void AddElement(const std::string &Name, const std::string &TechniqueName, CRenderableObjectTechnique *ROTOnRenderableObjectTechniqueManager);
	void Apply();
};
#endif