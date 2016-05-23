#pragma once
#ifndef INC_BOX_EMITER_H_
#define INC_BOX_EMITER_H_

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

class CBoxEmitter: public CParticleEmitter
{
public:
	CBoxEmitter(CXMLTreeNode &parser);
	CBoxEmitter(CParticleEmitter& other);
	~CBoxEmitter();
	void GenerateParticlePosition(CParticle * particle);
	void SetSize(float size){m_Size=size; }
private:
	float m_Size;
};

#endif