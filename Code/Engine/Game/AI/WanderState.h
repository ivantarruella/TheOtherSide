#ifndef INC_WANDERSTATE_H_
#define INC_WANDERSTATE_H_

#include <vector>
#include "Vector3.h"
#include "State.h"

class CWanderState:public CState {
public:
	CWanderState()
		: m_fLostTime(0.0f)
	{
	};
	
	~CWanderState(){};

	void Create();
	void Update(float ElapsedTime);
	void Init();
	void Reset();

private:
	bool isFaced();
	bool isFaced(Vect3f position);
	bool isHeared();
	bool isSeen();
	bool isAtLeft(Vect3f position);
	void recalculateWander();

	Vect3f m_PosWander;
	Vect3f m_Pivote;
	float m_fLostTime;

};

#endif