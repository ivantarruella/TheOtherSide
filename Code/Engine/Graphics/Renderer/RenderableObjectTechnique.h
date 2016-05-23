#pragma once
#ifndef __CLASS_RENDERABLE_OBJECT_TECHNIQUE_H__
#define __CLASS_RENDERABLE_OBJECT_TECHNIQUE_H__

#include "Named.h"
#include <string>

class CEffectTechnique;

class CRenderableObjectTechnique : public CNamed
{
private:
	CEffectTechnique *m_EffectTechnique;
public:
	CRenderableObjectTechnique():m_EffectTechnique(NULL){}
	CRenderableObjectTechnique(const std::string &Name, CEffectTechnique *EffectTechnique);

	void SetEffectTechnique(CEffectTechnique *EffectTechnique);
	CEffectTechnique * GetEffectTechnique() const;
};

#endif