#include "RootTask.h"
#include "Task.h"
#include "BattleTask.h"
#include "Soldier.h"
#include "Character.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"

#include "Base.h"

CRootTask::~CRootTask()
{
	destroy();
}

bool CRootTask::checkCondition(float ElapsedTime)
{
	return true;
}

void CRootTask::doAction(float ElapsedTime)
{
	if(!m_bFrozen)
	{
		if(m_Owner!=NULL && m_Player!=NULL)
		{
			if(m_Sons[0]->checkCondition(ElapsedTime))
			{
				m_Sons[0]->doAction(ElapsedTime);
			}
			if(m_Sons[1]->checkCondition(ElapsedTime))
			{
				m_Sons[1]->doAction(ElapsedTime);
				return;
			}
			
			for(unsigned int i=2; i<m_Sons.size(); i++)
			{
				if(m_Sons[i]->checkCondition(ElapsedTime))
				{
					m_Sons[i]->doAction(ElapsedTime);
					i=m_Sons.size();
				}
			}
			return;
		}
		else{		
			if(m_Player==NULL && m_Owner!=NULL)
			{
				m_Player=(CCharacter*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
				((CSoldier*)m_Owner)->SetPlayer((CPlayer*)m_Player);
			}
		}
	}
	else{
		//m_Owner->ChangeCharacterAnimation(AIM_ANIM, 0.3f);
	}
}

void CRootTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CRootTask::Reset()
{
	m_Time = 0.0f;
	m_bFrozen =false;
	m_bDead =false;
	for(unsigned int i=0; i<m_Sons.size(); i++)
	{
		m_Sons[i]->Reset();
	}
}

void CRootTask::SetPlayer(CCharacter* player)
{
	m_Player = player;
	for(unsigned int i=0; i<m_Sons.size(); i++)
	{
		m_Sons[i]->SetPlayer(player);
	}
}

void CRootTask::GetAlarm()
{
	((CBattleTask*)m_Sons[2])->GetAlarm();
}

void CRootTask::SetDead()
{
	m_bDead=true;
}