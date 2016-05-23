#pragma once
#ifndef INC_LUASTATE_H_
#define INC_LUASTATE_H_

#include "State.h"

class CLUAState : public CState
{
public:
	CLUAState(){};
	~CLUAState(){};

	void Create();
	void Update(float ElapsedTime);
	void Init();

private:

};

#endif
