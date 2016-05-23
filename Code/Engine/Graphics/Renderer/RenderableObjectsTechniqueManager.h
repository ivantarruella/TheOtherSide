#pragma once
#ifndef __CLASS_RENDERABLE_OBJECTS_TECHNIQUE_MANAGER_H__
#define __CLASS_RENDERABLE_OBJECTS_TECHNIQUE_MANAGER_H__

#include "RenderableObjectTechnique.h"
#include "PoolRenderableObjectTechnique.h"
#include "EffectTechnique.h"
#include "EffectManager.h"
#include "Utils\MapManager.h"
#include "Utils\VectorMapManager.h"

#include <string>


class CRenderableObjectsTechniqueManager : public CMapManager<CRenderableObjectTechnique>
{
private:
	CVectorMapManager<CPoolRenderableObjectTechnique>	m_PoolRenderableObjectTechniques;
	void InsertRenderableObjectTechnique(const std::string &ROTName, const std::string &TechniqueName);
	std::string m_FileName;

public:
	CRenderableObjectsTechniqueManager(){}
	virtual ~CRenderableObjectsTechniqueManager();

	void Destroy();
	void Load(const std::string &FileName);
	void Reload();
	std::string GetRenderableObjectTechniqueNameByVertexType(unsigned int VertexType);
	CVectorMapManager<CPoolRenderableObjectTechnique>	& GetPoolRenderableObjectTechniques();

	CRenderableObjectTechnique * GetResource(const std::string &Name, const std::string &TechniqueName)
	{
		CRenderableObjectTechnique *l_ROT = CMapManager::GetResource(Name);
		if(l_ROT == NULL && TechniqueName!="")
		{
			InsertRenderableObjectTechnique(Name, TechniqueName);
			l_ROT = CMapManager::GetResource(Name);
		}
		
		return l_ROT;
	}
};

#endif



 