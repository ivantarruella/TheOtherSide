#ifndef INC_PATROLSTATE_H_
#define INC_PATROLSTATE_H_

#include <vector>
#include "Vector3.h"
#include "State.h"

class CNode;

class CPatrolState:public CState {
public:
	CPatrolState()
		:m_numPatrol(0), m_bFaced(false), m_ascent(true)
	{
	};
	
	~CPatrolState();

	void Create();
	void Update(float ElapsedTime);
	void Init();
	void Reset();

	//void addPatrolPoint(const Vect3f &point) {m_PatrolWay.push_back(point);};
	void addPatrolPoint(int point) {m_PatrolWay.push_back(point);};

private:
	bool isFaced();
	bool isFaced(Vect3f pos);
	bool isHeared();
	bool isSeen();
	bool isAtLeft();
	bool isAtLeft(Vect3f pos);
	bool isReachable(Vect3f pos);

	//std::vector<Vect3f> m_PatrolWay;
	
	std::vector<CNode*> m_Path;
	std::vector<int> m_PatrolWay;
	bool m_ascent;
	int m_numPatrol;
	bool m_bFaced;

};

#endif