#ifndef INC_MOVETASK_H_
#define INC_MOVETASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CNode;

class CMoveTask:public CTask {
public:
	CMoveTask()
		:m_bChasing(false), m_bCoverReached(false), m_bCovered(false), m_ApropiateCover(-2), m_TimeUncover(1.0f)
		,m_bReady(true), m_TimeWait(0.0f)
	{
	};
	
	~CMoveTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();
	void SetPlayer(CCharacter* player);
	int MostApropiateCover();
	bool isReady(){return m_bReady;};

private:
	bool m_bChasing;	
	bool m_bCoverReached;
	bool m_bCovered;
	bool m_bReady;
	int m_ApropiateCover;
	float m_TimeUncover;
	float m_TimeWait;

	std::vector<CNode*> m_Path;
	bool Chase(float ElapsedTime);
	bool reachCover(int numCover, float ElapsedTime);
	void Covered(int numCover, float ElapsedTime);
	bool isReachable(Vect3f pos);
	bool isFacedRun();
	bool isFacedRun(Vect3f pos);
};

#endif