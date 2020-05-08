#include "TorusEmitter.h"
#include "Random.h"
#include "XML\XMLTreeNode.h"


CTorusEmitter::CTorusEmitter(CXMLTreeNode &parser)
{
	CParticleEmitter::Load(parser);
	m_Radius = parser.GetFloatProperty("radius", 0.0f);
}

CTorusEmitter::CTorusEmitter(CParticleEmitter& other)
{
	CParticleEmitter::Copy(other);
}

CTorusEmitter::~CTorusEmitter()
{
	m_vAcelerations.clear();

	m_vColors.clear();

	m_vDirections.clear();

	m_vSizes.clear();
}

void CTorusEmitter::GenerateParticlePosition(CParticle * particle)
{
	Vect3f l_aux;
	float l_angle;
	l_aux;

	CRandom * l_Random = CORE->GetRandom();
	//Buscamos un flotante entre 0.0f y 2Pi
	l_angle = l_Random->getRandFloat(0.0f, e2PIf);
	//Nos devuelve las coordenadas de X y Z según el ángulo (X y Z entre -1 y 1)
	l_aux = xzFromAngle(l_angle);
	
	//Lo multiplicamos por el radio y lo sumamos a la posición del emisor
	l_aux *= m_Radius;
	l_aux += m_vPos;
	particle->SetPos(l_aux);
	particle->SetFirstPos(l_aux);
}

Vect3f CTorusEmitter::xzFromAngle( float radians ) 
{
	Vect3f l_aux;
	l_aux.x = (float)sin( radians );
	l_aux.y = float( 0.0f );
	l_aux.z = (float)(-cos( radians ));
	return l_aux;
}