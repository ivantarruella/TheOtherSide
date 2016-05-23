#ifndef INC_IDLETASK_H_
#define INC_IDLETASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CIdleTask:public CTask {
public:
	CIdleTask()
	{
	};
	
	~CIdleTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();

};

#endif