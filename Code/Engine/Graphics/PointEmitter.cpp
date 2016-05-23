#include "PointEmitter.h"
#include "XML\XMLTreeNode.h"
#include "TextureManager.h"
#include "Core.h"

#include "Base.h"

void CPointEmitter::GenerateParticlePosition(CParticle * particle)
{
	particle->SetPos(m_vPos);
	particle->SetFirstPos(m_vPos);
}

CPointEmitter::CPointEmitter(CXMLTreeNode &parser)
{	
	CParticleEmitter::Load(parser);
}

CPointEmitter::CPointEmitter(CParticleEmitter& other)
{
	CParticleEmitter::Copy(other);
}

CPointEmitter::~CPointEmitter()
{
	m_vAcelerations.clear();

	m_vColors.clear();

	m_vDirections.clear();

	m_vSizes.clear();
}