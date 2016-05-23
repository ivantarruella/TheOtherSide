#pragma once
#ifndef INC_SPHERE_EMITER_H_
#define INC_SPHERE_EMITER_H_

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

class CSphereEmitter: public CParticleEmitter
{
public:
	CSphereEmitter(CXMLTreeNode &parser);
	CSphereEmitter(CParticleEmitter& other);
	~CSphereEmitter();
	void GenerateParticlePosition(CParticle * particle);
	void SetRadius(float radius){m_Radius=radius;}
private:
	float m_Radius;
};

#endif