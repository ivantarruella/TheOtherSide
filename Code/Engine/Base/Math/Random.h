//-----------------------------------------------------------------------
// CRandom class
// Clase de números Aleatorios
//-----------------------------------------------------------------------
#ifndef RANDOM_H
#define RANDOM_H

#include <numeric>
#include "Vector3.h"
#include "Color.h"
#include <assert.h>

class CRandom
{
	enum { Rand_MAX = 0x7FFF };
public:
	CRandom (unsigned seed = 0x12345678): m_uSeed(seed) { }

	// Mecánica de la clase
	bool init (unsigned seed = 0x12345678)  { m_uSeed = seed; return true; }

	// Obtención de números pseudo-aleatorios
	signed    getRandBits     (unsigned uBits)               const;
	unsigned  getRandUnsigned (unsigned uMin, unsigned uMax) const;
	signed    getRandSigned   (signed   uMin, signed   uMax) const;
	float     getRandFloat    (float    fMin, float    fMax) const;
	bool      getRandBool     ()                             const;
	Vect3f	  getRandVect3f	  (Vect3f vMin,Vect3f vMax)		 const;
	CColor	  getRandColor	  (CColor vMin,CColor vMax)		 const;
	// Información
	unsigned  getSeed () const { return m_uSeed; }
	static const CRandom& GetGlobalRandom() { return s_Random; }

private:

	static CRandom  s_Random;
	unsigned getRand () const;
	static unsigned getRandMax ()    { return Rand_MAX; }
	static float    getInvRandMax () { return 1.f / Rand_MAX; }
	mutable unsigned m_uSeed;
};

#endif