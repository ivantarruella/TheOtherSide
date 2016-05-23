#pragma once
#ifndef INC_PLANE_EMITER_H_
#define INC_PLANE_EMITER_H_

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

class CPlaneEmitter: public CParticleEmitter
{
public:
	CPlaneEmitter(CXMLTreeNode &parser);
	CPlaneEmitter(CParticleEmitter& other);
	~CPlaneEmitter();
	void GenerateParticlePosition(CParticle * particle);
	void SetSize(float size){m_Size=size; }
	float GetSize(){return m_Size;};
private:
	float m_Size;
};

#endif