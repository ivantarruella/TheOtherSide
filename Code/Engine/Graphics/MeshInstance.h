#ifndef __CLASS_MESH_INSTANCE_H__
#define __CLASS_MESH_INSTANCE_H__
#pragma once

#include "RenderableObject.h"
#include "StaticMesh.h"
#include "xml/XMLTreeNode.h"

class CRenderManager;
class CLight;
class CFrustum;

class CInstanceMesh : public CRenderableObject
{
public:
	CInstanceMesh(const std::string &Name, const std::string &CoreName);
	CInstanceMesh(const CXMLTreeNode& XmlData);
	~CInstanceMesh();
	
	void Render(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender);
	void RenderDebug(CRenderManager *RM);
	void RenderShadow(CRenderManager *RM, CLight* Light);
	void RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum);

	CStaticMesh* GetStaticMesh() { return m_StaticMesh; }

private:
	void PositionMesh(CRenderManager *RM);
	void DrawMesh(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender);
	void DrawMeshShadow(CRenderManager *RM, const CFrustum* Frustum, CLight* Light);

private:
	CStaticMesh *m_StaticMesh;
};

#endif