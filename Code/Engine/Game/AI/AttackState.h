#ifndef INC_ATTACKSTATE_H_
#define INC_ATTACKSTATE_H_

#include <vector>
#include "Vector3.h"
#include "State.h"

class CAttackState:public CState {
public:
	CAttackState()
		:m_fHitTime(0.0f)
	{
	};
	
	~CAttackState(){};

	void Create();
	void Update(float ElapsedTime);
	void Init();
	void Reset();

private:
	bool isFaced();
	bool isHeared();
	bool isSeen();
	bool isAtLeft();
	float m_fHitTime;

};

#endif