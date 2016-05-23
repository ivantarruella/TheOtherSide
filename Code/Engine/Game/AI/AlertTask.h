#ifndef INC_ALERTTASK_H_
#define INC_ALERTTASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CAlertTask:public CTask {
public:
	CAlertTask()
		:m_Alarm(false)
	{
	};
	
	~CAlertTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();
	void GetAlarm(){m_Alarm=true;};

private:
	bool m_Alarm;
};

#endif