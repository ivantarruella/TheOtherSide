#ifndef __CLASS_RENDERABLE_OBJECT_H__
#define __CLASS_RENDERABLE_OBJECT_H__
#pragma once

#include "Object3D.h"
#include "Named.h"
#include "Camera\Frustum.h"

class CRenderManager;
class CLight;
class CFrustum;

class CRenderableObject : public CObject3D, public CNamed
{
public:
	CRenderableObject();
	virtual ~CRenderableObject() {}
	virtual void Update(float ElapsedTime) {}
	virtual void Render(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender) = 0;
	virtual void RenderDebug(CRenderManager *RM) = 0;
	virtual void RenderShadow(CRenderManager *RM, CLight* Light) = 0;
	virtual void RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum) = 0;
};

#endif