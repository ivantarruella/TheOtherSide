#ifndef INC_CHASESTATE_H_
#define INC_CHASESTATE_H_

#include <vector>
#include "Vector3.h"
#include "State.h"

class CNode;

class CChaseState:public CState {
public:
	CChaseState()
		:m_Lost(false), m_LPos(false), m_fLostTime(0.0f)
	{
	};
	
	~CChaseState();

	void Create();
	void Update(float ElapsedTime);
	void Init();
	void Reset();

private:
	bool isFaced();
	bool isFaced(Vect3f pos);
	bool isHeared();
	bool isSeen();
	bool isReachable();
	bool isAtLeft();
	bool isAtLeft(Vect3f pos);
	bool isBetterNext();

	std::vector<CNode*> m_Path;
	bool m_Lost;
	bool m_LPos;
	Vect3f m_LastPos;
	float m_fLostTime;
};

#endif