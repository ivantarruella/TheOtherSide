#include "TaskManager.h"
#include "Task.h"
#include "Player.h"
#include "RootTask.h"

#include "Base.h"

CTaskManager::~CTaskManager()
{
	//m_Root->destroy();
	CHECKED_DELETE(m_Root);
}

void CTaskManager::update(float ElapsedTime)
{
	m_Root->doAction(ElapsedTime);
}

void CTaskManager::Reset()
{
	m_bAlarm = false;
	m_Root->Reset();
}

void CTaskManager::SetPlayer(CPlayer * player)
{
	m_Root->SetPlayer(player);
}

void CTaskManager::GetAlarm()
{
	((CRootTask*)m_Root)->GetAlarm();
}

void CTaskManager::GetHurt()
{
	((CRootTask*)m_Root)->GetSon(0)->GetHurt();
}

void CTaskManager::SetDead()
{
	((CRootTask*)m_Root)->SetDead();
}