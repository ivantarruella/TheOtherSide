#include "SphereEmitter.h"
#include "Random.h"
#include "XML\XMLTreeNode.h"

CSphereEmitter::CSphereEmitter(CXMLTreeNode &parser)
{
	CParticleEmitter::Load(parser);
	m_Radius = parser.GetFloatProperty("radius", 0.0f);
}

CSphereEmitter::CSphereEmitter(CParticleEmitter& other)
{
	CParticleEmitter::Copy(other);
}

CSphereEmitter::~CSphereEmitter()
{
	m_vAcelerations.clear();

	m_vColors.clear();

	m_vDirections.clear();

	m_vSizes.clear();
}

void CSphereEmitter::GenerateParticlePosition(CParticle * particle)
{
	float l_x, l_y, l_z;
	float l_radius;

	CRandom * l_Random = CORE->GetRandom();
	//Buscamos un flotante entre -1.0f y 1.0f por cada cordenada
	l_x = l_Random->getRandFloat(-1.0f, 1.0f);
	l_y = l_Random->getRandFloat(-1.0f, 1.0f);
	l_z = l_Random->getRandFloat(-1.0f, 1.0f);
	
	Vect3f l_aux(l_x, l_y, l_z);
	l_aux.Normalize();
	//Buscamos un radio entre 0 y radius
	l_radius = l_Random->getRandFloat(0.0f, m_Radius);
	//Lo multiplicamos por la posición antes conseguida y la sumamos a la posición del emisor
	l_aux *= l_radius;
	l_aux += m_vPos;
	particle->SetPos(l_aux);
	particle->SetFirstPos(l_aux);
}