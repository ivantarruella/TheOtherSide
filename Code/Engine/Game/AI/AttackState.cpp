#include "AttackState.h"
#include "State.h"
#include "StateMachine.h"
#include "Character.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Scripting\ScriptManager.h"
#include "Characters\Monster.h"
#include "Base.h"

#define ATTACK_DIST		4.0f
#define ATTACK_TIME		1.2f
#define ATTACK_DAMAGE	0.25f

void CAttackState::Update(float ElapsedTime)
{	
	const float rotationSpeed = mathUtils::Deg2Rad( 135.0f );
	m_Time = m_Time + ElapsedTime;
	
	float l_sqdistance = m_Owner->GetPosition().SqDistance(m_Player->GetPosition());
	if(l_sqdistance <= ATTACK_DIST)
	{
		if(m_fHitTime > 0.0f)
		{
			m_fHitTime +=ElapsedTime;
			if(m_fHitTime > 0.8f)
			{
				Vect3f l_pla = m_Player->GetPosition();
				l_pla.y = 1.0f;
				Vect3f l_mon = m_Owner->GetPosition();
				l_mon.y = 1.0f;
				if(l_pla.Distance(l_mon) < 1.5f)
				{
					m_Player->TakeDamage(ATTACK_DAMAGE);
				}
				m_fHitTime=0.0f;
			}
		}

		if(m_Time >= ATTACK_TIME)
		{
			m_Time-=ATTACK_TIME;
			m_fHitTime += ElapsedTime;
			//m_Owner->ChangeCharacterAnimation(RUN_ANIM, 0.3f);
			m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
			CORE->GetScriptManager()->RunCode("sound_monster_hit()");
			m_Owner->ExecuteAction(RUN_ANIM, 0.3f, 0.3f);
			return;
		}
		else
		{	
			if(isFaced())
			{
				Vect3f l_dir = (m_Player->GetPosition() - m_Owner->GetPosition()).Normalize();
				m_Owner->SetYaw(atan2(-l_dir.x, l_dir.z));
				Vect3f l_pos = m_Player->GetPosition() - l_dir;
				
				Vect3f l_new = m_Owner->GetPosition() + l_dir * ElapsedTime*m_WalkSpeed;
				Vect3f l_old = (l_pos - l_new).Normalize();

				if((l_dir * l_old) > 0.0f){							
					m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);
					m_Owner->SetPosition(l_new);
					m_Owner->SetPosition(m_Owner->GetFront()*m_WalkSpeed, ElapsedTime);
				}
				else{		
					m_Owner->ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
					l_dir = l_pos - m_Owner->GetPosition();
					l_dir.y=0.0f;
					m_Owner->SetPosition(l_dir, ElapsedTime);
					//m_Owner->SetPosition(l_pos);
				}

				return;
			}
			else{			
				m_Owner->ChangeCharacterAnimation(WALK_ANIM, 0.3f);			
				if( isAtLeft())
				{
					m_Owner->SetYaw(m_Owner->GetYaw() + rotationSpeed*ElapsedTime);
				} 
				else{ 				
					m_Owner->SetYaw(m_Owner->GetYaw() - rotationSpeed*ElapsedTime);
				}
				return;
			}
		}
	}
	else{
		if(isHeared() || isSeen())
		{	
			m_StateMachine->ChangeState("CHASE");
			return;
		}
		/*else{			
			m_StateMachine->ChangeState("WANDER");
			return;
		}*/
	}
}

void CAttackState::Create()
{
	m_Time = ATTACK_TIME;
	m_Name = "ATTACK";
}

void CAttackState::Init()
{
	m_Time = ATTACK_TIME;
}

void CAttackState::Reset()
{
	m_Time = ATTACK_TIME;
	m_fHitTime = 0.0f;
	m_Name = "ATTACK";
}

bool CAttackState::isFaced()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition()).Normalize();
	
	float l_vSuma = m_Owner->GetFront() * l_aux;
	if(l_vSuma < 0.95f)
		return false;
	return true;
}

bool CAttackState::isHeared()
{
	Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());		
	if(l_aux.Length() > m_HearDistance)
		return false;
	return true;
}

bool CAttackState::isSeen()
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

bool CAttackState::isAtLeft()
{
	Vect3f l_up(0.0f, 1.0f, 0.0f);
	Vect3f l_right = l_up ^ m_Owner->GetFront();
	l_right.Normalize();
	float l_d = -(l_right * m_Owner->GetPosition());

	float v( (l_right * m_Player->GetPosition()) + l_d );

	return v <= 0.0f;
}
