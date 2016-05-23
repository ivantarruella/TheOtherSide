#ifndef INC_PATROLTASK_H_
#define INC_PATROLTASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CNode;

class CPatrolTask:public CTask {
public:
	CPatrolTask()
		:m_numPatrol(-1), m_ascent(false), m_bFaced(false), m_spinTime(0.0f), m_bSpinning(false), m_Count(5), m_Prev(-1)
	{
		/*addPatrolPoint(1);
		addPatrolPoint(2);
		addPatrolPoint(3);
		addPatrolPoint(4);*/
		m_numPatrol = -1;
		m_Time = 0.0f;
	};
	
	~CPatrolTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();
	void addPatrolPoint(int point) {m_PatrolWay.push_back(point);};

private:
	bool isReachable(Vect3f pos);

	std::vector<CNode*> m_Path;
	std::vector<int> m_PatrolWay;
	bool m_ascent;
	int m_numPatrol;
	float m_spinTime;
	bool m_bSpinning;
	bool m_bFaced;
	int m_Count;
	int m_Prev;

};

#endif