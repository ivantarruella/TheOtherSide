//----------------------------------------------------------------------------------------------------------------
// Clase de números Aleatorios
//----------------------------------------------------------------------------------------------------------------
#define __DONT_INCLUDE_MEM_LEAKS__
#include "Random.h"
#include "Base.h"


CRandom CRandom::s_Random;

// --------------------------------------------------------
//  Uso:            
//  Retorno:        
//  Parámetros:     
// --------------------------------------------------------
unsigned CRandom::getRand () const
{
	m_uSeed = m_uSeed* 214013L + 2531011L;
	return ((m_uSeed>> 16) & Rand_MAX);
}


// Obtención de números pseudo-aleatorios
signed CRandom::getRandBits(unsigned uBits) const 
{
	return static_cast<int>(getRand () & ((1<<uBits)-1));   
}

unsigned CRandom::getRandUnsigned (unsigned uMin, unsigned uMax) const { 
	const unsigned uDiv = (uMax - uMin + 1); 
	assert(uDiv > 0); 
	return uMin +  getRand() % (uDiv == 0 ? 1 : uDiv); 
}

signed CRandom::getRandSigned (signed uMin, signed uMax) const { 
	const unsigned uDiv = (static_cast<unsigned>(uMax - uMin + 1));
	assert(uDiv > 0); 
	return (signed) (uMin + (getRand() % (uDiv == 0 ? 1 : uDiv)));  
}

float CRandom::getRandFloat (float fMin, float fMax) const 
{ 
	return fMin + (fMax - fMin) * static_cast<float>(getRand()) * getInvRandMax (); 
}

bool CRandom::getRandBool () const 
{
	return getRandUnsigned (0, 1) == 1;           
}

Vect3f CRandom::getRandVect3f(Vect3f vMin,Vect3f vMax) const
{
	Vect3f l_Vect=Vect3f();
	l_Vect.x=getRandFloat(vMin.x,vMax.x);
	l_Vect.y=getRandFloat(vMin.y,vMax.y);
	l_Vect.z=getRandFloat(vMin.z,vMax.z);

	return l_Vect;
}

CColor CRandom::getRandColor(CColor vMin,CColor vMax) const
{
	CColor l_Vect=CColor();
	l_Vect.SetRed(getRandFloat(vMin.GetRed(),vMax.GetRed()));
	l_Vect.SetGreen(getRandFloat(vMin.GetGreen(),vMax.GetGreen()));
	l_Vect.SetBlue(getRandFloat(vMin.GetBlue(),vMax.GetBlue()));
	l_Vect.SetAlpha(getRandFloat(vMin.GetAlpha(),vMax.GetAlpha()));

	return l_Vect;
}