#include "IdleTask.h"
#include "Task.h"
#include "Character.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"

#include "Base.h"

CIdleTask::~CIdleTask()
{
	destroy();
}

bool CIdleTask::checkCondition(float ElapsedTime)
{
	return true;
}

void CIdleTask::doAction(float ElapsedTime)
{
	if(m_Player==NULL)
	{
		m_Player=(CCharacter*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
	}
	if(m_Player==NULL)
		return;

	m_Time = m_Time + ElapsedTime;
	m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
}

void CIdleTask::Reset()
{
	m_Time = 0.0f;
}

void CIdleTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}
