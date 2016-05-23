#pragma once
#ifndef __CLASS_DIRECTIONALLIGHT_H__
#define __CLASS_DIRECTIONALLIGHT_H__

#include "Light.h"
#include "Vector3.h"
#include "Vector2.h"

typedef Vect3f CPoint3D;

class CRenderManager;

class CDirectionalLight : public CLight
{
protected:
	CPoint3D m_Direction;
	Vect2f m_OrthoShadowMapSize;

public:
	CDirectionalLight() 
		: CLight(), m_Direction(Vect3f(0.0f, 0.0f, 0.0f)),m_OrthoShadowMapSize(Vect2f()) 
	{
		m_Type = DIRECTIONAL;
	}
	
	virtual void Update(float ElapsedTime);

	void SetDirection(const CPoint3D &Direction);
	CPoint3D GetDirection() const;
	void RenderDebug(CRenderManager *RM);
	void SetOrthoShadowMapSize(float w,float h);
	void SetShadowMap(CRenderManager *RM);
	bool isVisible(CRenderManager &RM, const CFrustum* Frustum);
	float GetRadius();
};

#endif