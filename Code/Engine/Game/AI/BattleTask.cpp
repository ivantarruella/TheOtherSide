#include "BattleTask.h"
#include "Task.h"
#include "Character.h"
#include "Core.h"
#include "AlertTask.h"
#include "MoveTask.h"
#include "Renderer\RenderableObjectsLayersManager.h"

#include "Base.h"

CBattleTask::~CBattleTask()
{
	destroy();
}

bool CBattleTask::checkCondition(float ElapsedTime)
{
	if(m_Alarm || (isHeared() && isNotBlockedEyeLine()) || (isSeen() && isNotBlockedEyeLine()))
		return true;

	return false;
	//TODO si se ve o escucha al player
}

void CBattleTask::doAction(float ElapsedTime)
{	
	m_Alarm=true;
	for(unsigned int i=0; i<m_Sons.size(); i++)
	{
		if(m_Sons[i]->checkCondition(ElapsedTime))
		{
			if(i<3)
				m_Sons[i]->doAction(ElapsedTime);
				//i=m_Sons.size();
			else{
				if(((CMoveTask*)m_Sons[2])->isReady())
				{
					m_Sons[i]->doAction(ElapsedTime);
				}
				else{
					m_Sons[i]->Reset();
				}
			}
		}
	}
	return;
}

void CBattleTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CBattleTask::Reset()
{
	m_Time = 0.0f;
	m_Alarm = false;
	for(unsigned int i=0; i<m_Sons.size(); i++)
	{
		m_Sons[i]->Reset();
	}
}

void CBattleTask::GetAlarm()
{
	m_Alarm=true;
	((CAlertTask*)m_Sons[0])->GetAlarm();
}

void CBattleTask::SetPlayer(CCharacter* player)
{
	m_Player = player;
	for(unsigned int i=0; i<m_Sons.size(); i++)
	{
		m_Sons[i]->SetPlayer(player);
	}
}
