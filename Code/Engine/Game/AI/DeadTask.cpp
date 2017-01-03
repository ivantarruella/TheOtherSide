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
		return true;

	return false;
}

void CDeadTask::doAction(float ElapsedTime)
{
	bool bIsHeadShoot = m_Owner->GetHeadShoot();

	// physX
	((CSoldier*)m_Owner)->RemoveCapsuleForBullets();
		
	// animations
	m_Owner->ClearAllAnims();
	m_Owner->ExecuteAction((bIsHeadShoot ? RECARGA_ARMA_ANIM : SHOOT_ANIM), 0.f, 0.f);
	m_Owner->ClearCycle(WAIT_ANIM, 0.f);
	m_Owner->ChangeCharacterAnimation(HIT_ANIM, 0.f);
		
	// sound
	float dist = CORE->GetCamera()->GetEye().Distance(m_Owner->GetPosition());
	std::stringstream action;
	action.clear();
	float vol = 1.0f - dist/25.0f;
		
	if (!bIsHeadShoot)
		action << "sound_soldier_die(" << vol << ")";
	else 
		action << "sound_soldier_die_headshot(" << vol << ")";
	
	CORE->GetScriptManager()->RunCode(action.str());
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
}
