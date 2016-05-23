#ifndef __CLASS_LOGICOBJECT_H__
#define __CLASS_LOGICOBJECT_H__
#pragma once

#include "Named.h"
#include "XML\XMLTreeNode.h"

class CRenderableObject;

class CLogicObject : public CNamed
{
public:
	enum TLogicType	{ 
		NONE = 0,
		MIRROR,
		SPAWN_POINT,
		TUTORIAL,
		DOOR,
		ELEVATOR,
		FAN,
		LASERS,
		LIFE_RECOVER,
		ENERGY_RECOVER,
		MIRROR_PIECE,
		BROKEN_MIRROR
	};

public:
	CLogicObject();
	CLogicObject(CXMLTreeNode &atts);
	virtual ~CLogicObject();
	
	virtual void Update(float ElapsedTime) = 0;
	virtual void Restart() = 0;

	void SetType(const TLogicType Type) { m_Type = Type; }
	TLogicType GetType() const { return m_Type; }

	bool GetEnabled() { return m_bEnabled; }
	void SetEnabled(bool val) { m_bEnabled = val; }

protected:
	TLogicType m_Type;
	bool m_bEnabled;
};

#endif
