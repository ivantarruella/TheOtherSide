#pragma once
#ifndef INC_TASKMANAGER_H_
#define INC_TASKMANAGER_H_

#include <string>

class CTask;
class CPlayer;

class CTaskManager
{
protected:
	CTask * m_Root; 
	bool m_bAlarm;

public:
	CTaskManager()
		:m_Root(NULL), m_bAlarm(false)
	{};

	~CTaskManager();

	void update(float ElapsedTime);
	void Reset();

	void SetRoot(CTask* root){m_Root = root;};
	CTask* GetRoot(){return m_Root;};
	void SetPlayer(CPlayer * player);
	void GetAlarm();
	void GetHurt();
	void SetDead();
};

#endif INC_TASKMANAGER_H_