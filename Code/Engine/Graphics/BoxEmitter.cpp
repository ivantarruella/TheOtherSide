#include "BoxEmitter.h"
#include "Random.h"
#include "XML\XMLTreeNode.h"

CBoxEmitter::CBoxEmitter(CXMLTreeNode &parser)
{
	CParticleEmitter::Load(parser);
	m_Size = parser.GetFloatProperty("size", 0.0f);
}

CBoxEmitter::CBoxEmitter(CParticleEmitter& other)
{
	CParticleEmitter::Copy(other);
}

CBoxEmitter::~CBoxEmitter()
{
	m_vAcelerations.clear();

	m_vColors.clear();

	m_vDirections.clear();

	m_vSizes.clear();
}

void CBoxEmitter::GenerateParticlePosition(CParticle * particle)
{
	Vect3f l_aux;
	float l_z, l_y, l_x;
	l_aux = m_vPos;

	CRandom * l_Random = CORE->GetRandom();
	l_z = l_Random->getRandFloat(- m_Size/2, m_Size/2);
	l_aux.z += l_z;
	l_y = l_Random->getRandFloat(- m_Size/2, m_Size/2);
	l_aux.y += l_y;
	l_x = l_Random->getRandFloat(- m_Size/2, m_Size/2);
	l_aux.x += l_x;
	particle->SetPos(l_aux);
	particle->SetFirstPos(l_aux);
}