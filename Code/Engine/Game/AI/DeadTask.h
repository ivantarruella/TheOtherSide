#ifndef INC_DEADTASK_H_
#define INC_DEADTASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CDeadTask:public CTask {
public:
	CDeadTask()
		:m_bDying(false),m_bDead(false)
	{
	};
	
	~CDeadTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();

private:
	bool m_bDying;
	bool m_bDead;
};

#endif