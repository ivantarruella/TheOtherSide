#pragma once
#ifndef INC_POINT_EMITER_H_
#define INC_POINT_EMITER_H_

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

class CPointEmitter: public CParticleEmitter
{
public:
	CPointEmitter(CXMLTreeNode &parser);	
	CPointEmitter(CParticleEmitter& other);
	~CPointEmitter();

	void GenerateParticlePosition(CParticle * particle);
private:

};

#endif
