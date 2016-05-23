#ifndef __CLASS_SPAWNPOINTOBJECT_H__
#define __CLASS_SPAWNPOINTOBJECT_H__
#pragma once

#include "Vector3.h"
#include "XML\XMLTreeNode.h"
#include "LogicObject.h"

class CSpawnPointObject : public CLogicObject
{
public:
	CSpawnPointObject();
	CSpawnPointObject(CXMLTreeNode &atts);
	~CSpawnPointObject();

	void Update(float ElapsedTime);
	
	Vect3f GetPosition() { return m_Position; }
	void SetPosition(const Vect3f& pos) { m_Position = pos; }

private:
	void Restart(){};

private:
	Vect3f m_Position;
};

#endif