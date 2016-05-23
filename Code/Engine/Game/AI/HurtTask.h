#ifndef INC_HURTTASK_H_
#define INC_HURTTASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CHurtTask:public CTask {
public:
	CHurtTask()
		:m_bHurt(false)
	{
	};
	
	~CHurtTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void GetHurt();
	void Reset();

private:
	bool m_bHurt;
};

#endif