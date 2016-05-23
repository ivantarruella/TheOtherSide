#pragma once
#ifndef __CLASS_RENDERABLE_OBJECTS_LAYER_MANAGER_H__
#define __CLASS_RENDERABLE_OBJECTS_LAYER_MANAGER_H__

#include "RenderableObjectsManager.h"
#include "Utils\VectorMapManager.h"
#include "Camera\Frustum.h"
#include <string>

class CRenderManager;

class CRenderableObjectsLayersManager : public CVectorMapManager<CRenderableObjectsManager>
{
private:
	std::string m_FileName;
	CRenderableObjectsManager *m_DefaultRenderableObjectManager;
	CRenderableObjectsManager * GetRenderableObjectManager(CXMLTreeNode &Node);

public:
	CRenderableObjectsLayersManager();
	~CRenderableObjectsLayersManager();

	void Destroy();
	void Load(const std::string &FileName);
	void Reload();
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM, bool forwardRender);
	void Render(CRenderManager *RM, const std::string &LayerName, bool forwardRender);
	void RenderReflected(CRenderManager *RM, const std::string &LayerName);
	void RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum);
	void RenderReflected(CRenderManager *RM, const std::string &LayerName, CRenderableObject* Mesh, const CFrustum* Frustum);

	void SetLayerEnabled(const std::string& Name, bool enabled);
	bool GetLayerEnabled(const std::string& Name);

	void EnablePhysicLayer(const std::string& LayerName, bool enabled);

	CRenderableObjectsManager * GetRenderableObjectManager(const std::string &layer);
};

#endif