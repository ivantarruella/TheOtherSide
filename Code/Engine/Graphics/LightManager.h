#pragma once
#ifndef __CLASS_LIGHTMANAGER_H__
#define __CLASS_LIGHTMANAGER_H__

#include "Light.h"
#include "Utils/VectorMapManager.h"
#include "xml/XMLTreeNode.h"
#include <string>

class CRenderManager;

class CLightManager : public CVectorMapManager<CLight>
{
private:
	std::string m_Path;
	std::string m_ShadowsType;
	Vect3f m_AmbientLight;

public:
	CLightManager();
	~CLightManager();

	void Load(const std::string &FileName, const std::string& shadows_type);
	void Reload();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RenderManager);
	void GenerateShadowMaps(CRenderManager *RM);
	void BlurShadowMapsH(CRenderManager *RM);
	void BlurShadowMapsV(CRenderManager *RM);
	const TVectorResources& GetLights() { return m_ResourcesVector; }
	size_t getNumLights() {	return m_ResourcesVector.size(); }
	CLight* GetLightByName(std::string Name);
	void SetLightEnabled(std::string Name, bool enabled);
	void SetLightActive(std::string Name, bool active);

	void SetAmbientLight(const Vect3f& ambient) { m_AmbientLight = ambient; }
	const Vect3f& GetAmbientLight() { return m_AmbientLight; }

	void ChangeWorldLighting(CRenderManager *RM, int world, bool enabled);
	
	void SetLightMoving(std::string Name, bool move);
};

#endif