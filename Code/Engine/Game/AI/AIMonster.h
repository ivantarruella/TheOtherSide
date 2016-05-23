#pragma once
#ifndef INC_AIMONSTER_H_
#define INC_AIMONSTER_H_

#include "AI.h"
#include <string>

class CAIMonster: public CAI 
{
private:

public:
	CAIMonster(){};
	
	~CAIMonster(){};

	void Create();
	void Update(float ElapsedTime);
	void Init();

	

};

#endif