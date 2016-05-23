#ifndef INC_IDLESTATE_H_
#define INC_IDLESTATE_H_

#include <vector>
#include "Vector3.h"
#include "State.h"

class CIdleState:public CState {
public:
	CIdleState()
	{
	};
	
	~CIdleState(){};

	void Create();
	void Update(float ElapsedTime);
	void Init();
	void Reset();

private:
	bool isFaced();
	bool isHeared();
	bool isSeen();

};

#endif