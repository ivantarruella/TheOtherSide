#pragma once
#ifndef INC_CIRCLE_EMITER_H_
#define INC_CIRCLE_EMITER_H_

//#define NUMPARTICLES 100

#include "Math\Vector3.h"
#include "Math\Color.h"
#include <string>
#include "Particle.h"
#include "ParticleEmitter.h"
#include "utils\RecyclingArray.h"

class CCamera;
class CTexture;
class CXMLTreeNode;

class CCircleEmitter: public CParticleEmitter
{
public:
	CCircleEmitter(CXMLTreeNode &parser);
	CCircleEmitter(CParticleEmitter& other);
	~CCircleEmitter();
	void GenerateParticlePosition(CParticle * particle);
	void SetRadius(float radius){m_Radius=radius;}
private:
	Vect3f xzFromAngle( float radians );
	float m_Radius;
};

#endif