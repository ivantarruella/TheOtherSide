#include "CircleEmitter.h"
#include "Random.h"
#include "XML\XMLTreeNode.h"

CCircleEmitter::CCircleEmitter(CXMLTreeNode &parser)
{
	CParticleEmitter::Load(parser);
	m_Radius = parser.GetFloatProperty("radius", 0.0f);
}

CCircleEmitter::CCircleEmitter(CParticleEmitter& other)
{
	CParticleEmitter::Copy(other);
}

CCircleEmitter::~CCircleEmitter()
{
	m_vAcelerations.clear();

	m_vColors.clear();

	m_vDirections.clear();

	m_vSizes.clear();
}

void CCircleEmitter::GenerateParticlePosition(CParticle * particle)
{
	Vect3f l_aux;
	float l_radius, l_angle;
	l_aux;

	CRandom * l_Random = CORE->GetRandom();
	//Buscamos un flotante entre 0.0f y 2Pi
	l_angle = l_Random->getRandFloat(0.0f, e2PIf);
	//Nos devuelve las coordenadas de X y Z según el ángulo (X y Z entre -1 y 1)
	l_aux = xzFromAngle(l_angle);
	
	//Buscamos un radio entre 0 y radius
	l_radius = l_Random->getRandFloat(0.0f, m_Radius);
	//Lo multiplicamos por la posición antes conseguida y la sumamos a la posición del emisor
	l_aux *= l_radius;
	l_aux += m_vPos;
	particle->SetPos(l_aux);
	particle->SetFirstPos(l_aux);
}

Vect3f CCircleEmitter::xzFromAngle( float radians ) 
{
	Vect3f l_aux;
	l_aux.x = sin( radians );
	l_aux.y = float( 0.0f );
	l_aux.z = -cos( radians );
	return l_aux;
}