#include "IdleState.h"
#include "State.h"
#include "StateMachine.h"
#include "Character.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Characters\Monster.h"
#include "Base.h"

void CIdleState::Update(float ElapsedTime)
{
	if (m_Player==NULL)
		return;

	//const float rotationSpeed = degToRad( 45.0f );

	m_Time = m_Time + ElapsedTime;

	float l_sqdistance = m_Owner->GetPosition().SqDistance(m_Player->GetPosition());
	
	m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
	if(isFaced() && l_sqdistance<4.0f)
	{
		m_StateMachine->ChangeState("ATTACK");
		return;
	}

	if(isHeared() || isSeen())
	{
		m_StateMachine->ChangeState("CHASE");
		return;
	}

	/*if(m_Time > 10.0f)
	{
		m_Time = 0.0f;
		m_StateMachine->ChangeState("PATROL");
		return;
	}*/
}

void CIdleState::Create()
{
	m_Time = 0.0f;
	m_Name = "IDLE";
}

void CIdleState::Init()
{
	m_Time = 0.0f;
}

void CIdleState::Reset()
{	
	m_Time = 0.0f;
	m_Name = "IDLE";
}

bool CIdleState::isFaced()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition()).Normalize();
	
	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < 0.98f)
		return false;
	return true;
}

bool CIdleState::isHeared()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());		

	if(l_aux.Length() > m_HearDistance)
		return false;
	return true;
}

bool CIdleState::isSeen()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());		
	if(l_aux.Length() > m_VisionDistance)
		return false;
	l_aux.Normalize();
	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < degToDot(m_VisionAngle/2.0f))
		return false;
	return true;
}
