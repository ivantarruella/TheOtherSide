#ifndef __CLASS_RENDERABLEOBJECTSMANAGER_H__
#define __CLASS_RENDERABLEOBJECTSMANAGER_H__

#pragma once

#include "RenderableObject.h"
#include "xml/XMLTreeNode.h"
#include "Utils/MapManager.h"
#include "Camera\Frustum.h"

#include <vector>

class CRenderableObjectsManager : public CMapManager<CRenderableObject>
{
private:
	std::vector<CRenderableObject *> m_RenderableObjects;
	bool m_bLayerEnabled;

public:
	CRenderableObjectsManager();
	~CRenderableObjectsManager();
	
	void Update(float ElapsedTime);
	void Render(CRenderManager *RM, bool forwardRender);
	void RenderDebug(CRenderManager *RM);
	void RenderShadow(CRenderManager *RM, CLight* Light);
	void RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum);

	void Load(const std::string &FileName);

	void AddMeshInstance(const CXMLTreeNode& xml_data);
	void AddAnimatedModelInstance(const CXMLTreeNode& xml_data);
	void AddAnimatedModelInstance(const std::string &CoreName ,const std::string &Name, const Vect3f &pos);
	bool AddResource(const std::string &Name, CRenderableObject *RenderableObject);
	bool RemoveResource(const std::string &Name);
	//bool isAResource(CPhysicUserData * userData);
	CRenderableObject * GetInstance(const std::string &Name) const;

	void SetEnabled(bool enabled) { m_bLayerEnabled = enabled; }
	bool GetEnabled() { return m_bLayerEnabled; }

	void EnablePhysicLayer(bool enable);

private:
	void CleanUp();
};

#endif