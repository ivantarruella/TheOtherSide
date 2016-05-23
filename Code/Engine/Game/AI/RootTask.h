#ifndef INC_ROOTTASK_H_
#define INC_ROOTTASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CRootTask:public CTask {
public:
	CRootTask()
		:m_bFrozen(false), m_bDead(false)
	{
	};
	
	~CRootTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();
	void SetPlayer(CCharacter* player);
	void GetAlarm();
	void SetDead();
private:
	bool m_bFrozen;
	bool m_bDead;

};

#endif