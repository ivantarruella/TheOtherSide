#include "AimTask.h"
#include "Task.h"
#include "Character.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"

#include "Base.h"

CAimTask::~CAimTask()
{
	destroy();
}

bool CAimTask::checkCondition(float ElapsedTime)
{
	if(m_Player==NULL)
	{
		m_Player=(CCharacter*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
	}

	//float l_dist = m_Player->GetPosition().SqDistance(m_Owner->GetPosition());

	if(isFaced() || !isNotBlockedEyeLine() /*|| l_dist < 10.0f*/)
		return false;
	else
		return true;
	//TODO Está a tiro
}

void CAimTask::doAction(float ElapsedTime)
{
	/*m_Time = m_Time + ElapsedTime;
	const float rotationSpeed = mathUtils::Deg2Rad( 180.0f );
	
	m_Owner->ChangeCharacterAnimation(RUN_ANIM, 0.3f);			
	if( isAtLeft())
	{
		m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
	} 
	else{ 				
		m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);
	}
	return;*/
}

void CAimTask::Reset()
{
	m_Time = 0.0f;
}

void CAimTask::destroy()
{
	for(unsigned int i=0; i < m_Sons.size(); i++)
	{
		CHECKED_DELETE(m_Sons[i]);
	}
	m_Sons.clear();
}
