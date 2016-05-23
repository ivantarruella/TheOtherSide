#include "SpawnPointObject.h"


CSpawnPointObject::CSpawnPointObject()
	: m_Position(Vect3f(0.f,0.f,0.f))
{
	m_Type = SPAWN_POINT;
}

CSpawnPointObject::CSpawnPointObject(CXMLTreeNode &atts)
	: CLogicObject(atts)
{
	m_Type = SPAWN_POINT;
	m_Position = atts.GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f),true);
}

CSpawnPointObject::~CSpawnPointObject()
{
}

void CSpawnPointObject::Update(float ElapsedTime)
{
}