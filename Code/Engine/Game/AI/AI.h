#pragma once
#ifndef INC_AI_H_
#define INC_AI_H_

#include <string>

//ESTADOS DE LA IA
#define IDLE 0
#define CHASE 1
#define ATTACK 2
#define ESCAPE 3
#define DEAD 4
#define PATROL 5

class CAI
{
protected:
	int m_State;

public:
	CAI(){};
	
	virtual ~CAI(){};

	virtual void Create()=0;
	virtual void Update(float ElapsedTime)=0;
	virtual void Init()=0;

	
	//Funciones propias de la IA
	void Reset(){m_State = IDLE;}

};

#endif
