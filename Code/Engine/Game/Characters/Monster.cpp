#include "Monster.h"
#include "Soldier.h"
#include "EnemyManager.h"
#include "Player.h"
#include "StateMachine.h"
#include "IdleState.h"
#include "PatrolState.h"
#include "AttackState.h"
#include "ChaseState.h"
#include "WanderState.h"
#include "EscapeState.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Player.h"
#include "SpotLight.h"
#include "LightManager.h"
#include "PhysicController.h"
#include "PhysicsManager.h"
#include "Base.h"


#define MONSTER_BBOX_SIZE	0.33f
#define MONSTER_BBOX_HEIGHT	0.70f

CMonster::CMonster()
{

}

CMonster::CMonster(const CXMLTreeNode& XmlData):
	CCharacter(XmlData, MONSTER_BBOX_SIZE,MONSTER_BBOX_HEIGHT)
{	
	m_bFrozen = false;
	m_TimeDead = 0.0f;	
	
	m_StateMachine = new CStateMachine();
	//CPatrolState* l_Patrol = new CPatrolState();
	CIdleState* l_Idle = new CIdleState();
	CAttackState* l_Attack = new CAttackState();
	CChaseState* l_Chase = new CChaseState();
	CWanderState* l_Wander = new CWanderState();
	CEscapeState* l_Escape = new CEscapeState();
	//l_Patrol->Create();
	l_Idle->Create();
	l_Attack->Create();
	l_Chase->Create();
	l_Wander->Create();
	l_Escape->Create();
	//m_StateMachine->AddState(l_Patrol);
	m_StateMachine->AddState(l_Idle);
	m_StateMachine->AddState(l_Attack);
	m_StateMachine->AddState(l_Chase);
	m_StateMachine->AddState(l_Wander);
	m_StateMachine->AddState(l_Escape);
	m_StateMachine->SetInitState("IDLE");

	/*Vect3f l_node0(2.4f, 0.0f, -5.7f);
	Vect3f l_node1(2.4f, 0.0f, -13.10f);
	Vect3f l_node2(8.74f, 0.0f, -13.10f);
	Vect3f l_node3(8.74f, 0.0f, -5.7f);
	l_Patrol->addPatrolPoint(8);
	l_Patrol->addPatrolPoint(9);
	l_Patrol->addPatrolPoint(10);*/

	//l_Patrol->SetOwner(this);
	l_Idle->SetOwner(this);
	l_Attack->SetOwner(this);
	l_Chase->SetOwner(this);
	l_Wander->SetOwner(this);
	l_Escape->SetOwner(this);
	m_initPosition = GetPosition();
	m_initYaw = GetYaw();
	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
}

CMonster::CMonster(const Vect3f &pos, const std::string &CoreName, const std::string &Name):
	CCharacter(pos, CoreName, Name, MONSTER_BBOX_SIZE, MONSTER_BBOX_HEIGHT)
{	
	m_bFrozen = false;	
	m_TimeDead = 0.0f;
	
	m_StateMachine = new CStateMachine();
	//CPatrolState* l_Patrol = new CPatrolState();
	CIdleState* l_Idle = new CIdleState();
	CAttackState* l_Attack = new CAttackState();
	CChaseState* l_Chase = new CChaseState();
	CWanderState* l_Wander = new CWanderState();
	CEscapeState* l_Escape = new CEscapeState();
	//l_Patrol->Create();
	l_Idle->Create();
	l_Attack->Create();
	l_Chase->Create();
	l_Wander->Create();
	l_Escape->Create();
	//m_StateMachine->AddState(l_Patrol);
	m_StateMachine->AddState(l_Idle);
	m_StateMachine->AddState(l_Attack);
	m_StateMachine->AddState(l_Chase);
	m_StateMachine->AddState(l_Wander);
	m_StateMachine->AddState(l_Escape);
	m_StateMachine->SetInitState("IDLE");

	//l_Patrol->SetOwner(this);
	l_Idle->SetOwner(this);
	l_Attack->SetOwner(this);
	l_Chase->SetOwner(this);
	l_Wander->SetOwner(this);
	l_Escape->SetOwner(this);
	m_initPosition = GetPosition();
	m_initYaw = GetYaw();
	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
}

CMonster::~CMonster()
{
	CHECKED_DELETE(m_StateMachine);
}

void CMonster::Reload()
{
	CCharacter::Reload();
}

void CMonster::Reset()
{
	m_StateMachine->Reset();
	SetPosition(m_initPosition);
	SetYaw(m_initYaw);
}

void CMonster::Update(float ElapsedTime)
{
	if (m_Player==NULL || !GetEnabled())
		return;

	Vect3f l_pla=m_Player->GetPosition();
	Vect3f l_mon=GetPosition();

	if(!IsDead()) {
		if(!m_bFrozen && abs(l_pla.y - l_mon.y)<2.0f)
		{
			if(m_TimeDead == 0.0f)
			{
				if(m_Player->GetLife() > 0.0f)
				{
					m_StateMachine->Update(ElapsedTime);
				}
				else{
					ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
				}
				
			}
		}
		else{
			if(abs(l_pla.y - l_mon.y)>=2.0f)
				ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
		}
	}

	CCharacter::Update(ElapsedTime);
}

void CMonster::SetPosition(const Vect3f &Position){
	CCharacter::SetPosition(Position);

}
void CMonster::SetPosition(const Vect3f & Direction, float ElapsedTime){
	CCharacter::SetPosition(Direction, ElapsedTime);
}

void CMonster::SetPlayer(CPlayer * player)
{
	m_StateMachine->SetPlayer(player);
}

bool CMonster::isLighted(const Vect3f& pos, Vect3f dir, float distance)
{
	Vect3f l_pos = CCharacter::GetPosition();
	l_pos.y = pos.y;
	
	//Check distance
	float l_distance = pos.Distance(l_pos);
	if(l_distance > distance)
		return false;
		
	//Check angle
	Vect3f l_aux = (l_pos - pos);	
	l_aux.Normalize();
	float l_vSuma = dir * l_aux;
	float l_angle = ((CSpotLight*)CORE->GetLightManager()->GetResource("LASER"))->GetAngle();
	
	if(l_vSuma < degToDot(l_angle/2.0f))
		return false;

	return true;
}

void CMonster::Freeze()
{
	m_bFrozen =true;
}

void CMonster::Unfreeze()
{
	m_bFrozen =false;
}