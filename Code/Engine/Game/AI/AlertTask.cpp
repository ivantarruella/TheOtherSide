#include "AlertTask.h"
#include "Task.h"
#include "Character.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "EnemyManager.h"
#include "Core.h"

#include "Base.h"

CAlertTask::~CAlertTask()
{
	destroy();
}

bool CAlertTask::checkCondition(float ElapsedTime)
{
	if(!m_Alarm)
		return true;
	else
		return false;
}

void CAlertTask::doAction(float ElapsedTime)
{
	m_Time = m_Time + ElapsedTime;
	const float rotationSpeed = mathUtils::Deg2Rad( 90.0f );
	
	m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);			
	CORE->GetEnemyManager()->AlertCloseSoldiers((CSoldier*)m_Owner);
	//m_Alarm =true;
	((CSoldier*)m_Owner)->GetAlarm();
}

void CAlertTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}

void CAlertTask::Reset()
{
	m_Time = 0.0f;
	m_Alarm = false;
}
