#ifndef INC_AIMTASK_H_
#define INC_AIMTASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CAimTask:public CTask {
public:
	CAimTask()
	{
	};
	
	~CAimTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();

};

#endif