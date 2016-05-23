#ifndef INC_BATTLETASK_H_
#define INC_BATTLETASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CBattleTask:public CTask {
public:
	CBattleTask()
		:m_Alarm(false)
	{
	};
	
	~CBattleTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();
	void GetAlarm();
	void SetPlayer(CCharacter* player);
private:
	bool m_Alarm;
};

#endif