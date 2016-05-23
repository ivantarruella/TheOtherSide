#ifndef INC_SHOOTTASK_H_
#define INC_SHOOTTASK_H_

#include <vector>
#include "Vector3.h"
#include "Task.h"

class CShootTask:public CTask {
public:
	CShootTask();
	~CShootTask();

	bool checkCondition(float ElapsedTime);
	void doAction(float ElapsedTime);
	void destroy();
	void Reset();

private:
	bool isShooting;
	float m_TimeShoot;
	bool m_shoot;
};

#endif