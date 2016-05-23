#include "PlaneEmitter.h"
#include "Random.h"
#include "XML\XMLTreeNode.h"

CPlaneEmitter::CPlaneEmitter(CXMLTreeNode &parser)
{
	CParticleEmitter::Load(parser);
	m_Size = parser.GetFloatProperty("size", 0.0f);
}

CPlaneEmitter::CPlaneEmitter(CParticleEmitter& other)
{
	CParticleEmitter::Copy(other);
}

CPlaneEmitter::~CPlaneEmitter()
{
	m_vAcelerations.clear();

	m_vColors.clear();

	m_vDirections.clear();

	m_vSizes.clear();
}


void CPlaneEmitter::GenerateParticlePosition(CParticle * particle)
{
	Vect3f l_aux;
	float l_z, l_x;
	l_aux = m_vPos;

	CRandom * l_Random = CORE->GetRandom();
	l_z = l_Random->getRandFloat(- m_Size, m_Size);
	l_aux.z += l_z;
	l_x = l_Random->getRandFloat(- m_Size, m_Size);
	l_aux.x += l_x;
	particle->SetPos(l_aux);
	particle->SetFirstPos(l_aux);
}