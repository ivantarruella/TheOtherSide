#pragma once
#ifndef INC_LINE_EMITER_H_
#define INC_LINE_EMITER_H_

//#define NUMPARTICLES 1000

#include "Math\Vector3.h"
#include "Math\Color.h"
#include <string>
#include "Particle.h"
#include "ParticleEmitter.h"
#include "utils\RecyclingArray.h"

class CCamera;
class CTexture;
class CXMLTreeNode;

class CLineEmitter: public CParticleEmitter
{
public:	
	CLineEmitter(CXMLTreeNode &parser);
	CLineEmitter(CParticleEmitter& other);
	~CLineEmitter();

	void GenerateParticlePosition(CParticle * particle);
	void SetEnd(Vect3f end) {m_End = end;};
private:
	Vect3f m_End;
};

#endif