#ifndef __CLASS_FANOBJECT_H__
#define __CLASS_FANOBJECT_H__
#pragma once

#include "Vector3.h"
#include "LogicObject.h"
#include "Utils\LerpAnimator1D.h"


#include <vector>

class CInstanceMesh;


class CFanObject : public CLogicObject
{
public:
	CFanObject();
	CFanObject(CXMLTreeNode &atts);
	~CFanObject();

	void Update(float ElapsedTime);
	
private:
	void RotatePhysXMesh(CInstanceMesh* mesh, float angle);
	void Restart(){};

private:
	CInstanceMesh* m_FanMesh1;

	float m_fSpinVel;
	float m_fTime;
	float m_fAngle;
};

#endif
