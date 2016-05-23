#include "LogicObject.h"
#include "GUIManager.h"
#include "Base.h"

CLogicObject::CLogicObject()
	: m_Type(NONE), m_bEnabled(true)
{
}

CLogicObject::CLogicObject(CXMLTreeNode &atts)
	: m_Type(NONE), m_bEnabled(true)
{
	// Obtenemos los datos de logic object
	SetName(atts.GetPszProperty("name",""));
}

CLogicObject::~CLogicObject()
{
}

