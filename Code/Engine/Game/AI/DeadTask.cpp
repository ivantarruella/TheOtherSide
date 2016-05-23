#include "DeadTask.h"
#include "Task.h"
#include "Character.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "Camera\Camera.h"
#include "Base.h"

CDeadTask::~CDeadTask()
{
	destroy();
}

bool CDeadTask::checkCondition(float ElapsedTime)
{
	if(m_Owner->GetLife() <= 0.0f && !m_bDead)  //Vida menor que 0
	{
		return true;
	}
	m_Time=0.0f;
	return false;
	//TODO si se ve o escucha al player
}

void CDeadTask::doAction(float ElapsedTime)
{
	if (!m_bDying)
	{
		((CSoldier*)m_Owner)->RemoveCapsuleForBullets();
		m_Owner->ClearAllAnims();
		m_Owner->ExecuteAction(SHOOT_ANIM, 0.f, 0.f);
		m_Owner->ClearCycle(WAIT_ANIM, 0.f);
		m_Owner->ChangeCharacterAnimation(HIT_ANIM, 0.f);
		m_bDying = true;

		float dist = CORE->GetCamera()->GetEye().Distance(m_Owner->GetPosition());
		std::stringstream action;
		action.clear();
		float vol = 1.0f - dist/25.0f;
		action << "sound_soldier_die(" << vol << ")";
		CORE->GetScriptManager()->RunCode(action.str());

		m_Time = m_Owner->GetAnimationDuration() * 2.f;
	}
	else
	{
		m_Time -= ElapsedTime;
		
		if (m_Time <= 0.0f)
		{
			m_Owner->RemoveAction(SHOOT_ANIM);
			m_Owner->SetIsDead(true);
		}
	}
}

void CDeadTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CDeadTask::Reset()
{
	m_Time = 0.0f;
}
