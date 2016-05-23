#include "LineEmitter.h"
#include "Random.h"
#include "XML\XMLTreeNode.h"
#include "TextureManager.h"
#include "Core.h"

#include "Base.h"

void CLineEmitter::GenerateParticlePosition(CParticle * particle)
{
	Vect3f l_aux;
	CRandom * l_Random = CORE->GetRandom();
	l_aux = l_Random->getRandVect3f(m_vPos, m_End);
	particle->SetPos(l_aux);
	particle->SetFirstPos(l_aux);
}

CLineEmitter::CLineEmitter(CXMLTreeNode &parser)
{	
	CParticleEmitter::Load(parser);
}

CLineEmitter::CLineEmitter(CParticleEmitter& other)
{
	CParticleEmitter::Copy(other);
}

CLineEmitter::~CLineEmitter()
{
	m_vAcelerations.clear();

	m_vColors.clear();

	m_vDirections.clear();

	m_vSizes.clear();
}