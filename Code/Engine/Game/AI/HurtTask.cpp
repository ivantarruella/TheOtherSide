#include "HurtTask.h"
#include "Task.h"
#include "Character.h"
#include "Core.h"
#include "EnemyManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "Camera\Camera.h"
#include "Base.h"

CHurtTask::~CHurtTask()
{
	destroy();
}

bool CHurtTask::checkCondition(float ElapsedTime)
{
	if(m_Owner->GetLife() > 0.0f)
		if(m_bHurt)
			return true;

	return false;
	//TODO si se ve o escucha al player
}

void CHurtTask::doAction(float ElapsedTime)
{
	m_Time +=ElapsedTime;
	//m_Owner->ChangeCharacterAnimation(USE_ANIM, 0.3f);
	//m_Owner->ExecuteAction(USE_ANIM, 0.3f, 0.3f);   //ANIMACION DE ESTAR HERIDO
	if(m_Time > 0.5f)
	{
		m_bHurt = false;
		m_Time = 0.0f;
	}
	return;
}

void CHurtTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CHurtTask::Reset()
{
	m_Time = 0.0f;
	m_bHurt = false;
}

void CHurtTask::GetHurt()
{
	float dist = CORE->GetCamera()->GetEye().Distance(m_Owner->GetPosition());
	std::stringstream action;
	action.clear();
	float vol = 1.0f - dist/25.0f;
	action << "sound_soldier_hit(" << vol << ")";
	CORE->GetScriptManager()->RunCode(action.str());

	m_bHurt=true;
	if(!((CSoldier*)m_Owner)->isAlarm())
	{
		((CSoldier*)m_Owner)->GetAlarm();
		CORE->GetEnemyManager()->AlertCloseSoldiers((CSoldier*)m_Owner);
	}
	//m_Owner->ClearAllAnims();
	//m_Owner->ChangeCharacterAnimation(USE_ANIM, 0.3f, 0.3f);   //ANIMACION DE ESTAR HERIDO
	m_Time = 0.0f;
	//Reducir vida
}
