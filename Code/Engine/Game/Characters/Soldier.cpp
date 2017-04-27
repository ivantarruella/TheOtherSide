#include "Soldier.h"
#include "Monster.h"
#include "TaskManager.h"
#include "IdleTask.h"
#include "PatrolTask.h"
#include "RootTask.h"
#include "BattleTask.h"
#include "ShootTask.h"
#include "MoveTask.h"
#include "AimTask.h"
#include "AlertTask.h"
#include "DeadTask.h"
#include "HurtTask.h"
#include "Player.h"
#include "Core.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "PhysicController.h"
#include "PhysicsManager.h"
#include "Base.h"

#define	SOLDIER_BBOX_HEIGHT		0.90f
#define	SOLDIER_BBOX_SIZE		0.25f
#define SOLDIER_COLLISION_BOX_HEIGHT	0.485f
#define SOLDIER_COLLISION_BOX_SIZE		0.225f

#define MAX_LIFE_SOLDIER		2.5f

CSoldier::CSoldier(const Vect3f &pos, const std::string &CoreName, const std::string &Name):CCharacter(pos, CoreName, Name, SOLDIER_BBOX_SIZE,SOLDIER_BBOX_HEIGHT)
{	
	CreateSoldier();
}

CSoldier::CSoldier(const CXMLTreeNode& XmlData):CCharacter(XmlData, SOLDIER_BBOX_SIZE,SOLDIER_BBOX_HEIGHT)
{	
	CreateSoldier();
}

CSoldier::~CSoldier()
{
	CHECKED_DELETE(m_TaskManager);
	CHECKED_DELETE(m_PhysicElement);
}

void CSoldier::CreateSoldier()
{
	m_TaskManager = new CTaskManager();
	CRootTask* l_Root = new CRootTask();
	CIdleTask* l_Idle = new CIdleTask();
	//CPatrolTask* l_Patrol = new CPatrolTask();
	CBattleTask* l_Battle = new CBattleTask();
	CAimTask* l_Aim = new CAimTask();
	CAlertTask* l_Alert = new CAlertTask();
	CMoveTask* l_Move = new CMoveTask();
	CShootTask* l_Shoot = new CShootTask();
	CHurtTask* l_Hurt = new CHurtTask();
	CDeadTask* l_Dead = new CDeadTask();
	
	l_Root->AddSon(l_Hurt);
	l_Root->AddSon(l_Dead);
	l_Root->AddSon(l_Battle);
	//l_Root->AddSon(l_Patrol);
	l_Root->AddSon(l_Idle);

	l_Battle->AddSon(l_Alert);
	l_Battle->AddSon(l_Aim);
	l_Battle->AddSon(l_Move);
	l_Battle->AddSon(l_Shoot);

	l_Root->SetOwner(this);
	l_Hurt->SetOwner(this);
	l_Dead->SetOwner(this);
	l_Idle->SetOwner(this);
	l_Battle->SetOwner(this);
	l_Aim->SetOwner(this);
	l_Alert->SetOwner(this);
	l_Move->SetOwner(this);
	l_Shoot->SetOwner(this);
	//l_Patrol->SetOwner(this);
	
	m_TaskManager->SetRoot(l_Root);

	m_Cover = -1;
	m_TimeDead = 0.0f;
	m_Alarm = false;
	m_bFrozen = false;
	m_HeadShoot = false;
	SetLife(MAX_LIFE_SOLDIER);
	m_initPosition = GetPosition();
	m_initYaw = GetYaw();

	CreateCapsuleForBullets();

	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
}

void CSoldier::Reload()
{
	CCharacter::Reload();
}

void CSoldier::Reset()
{
	m_Alarm = false;
	m_bFrozen = false;
	CCharacter::SetIsDead(false);
	CCharacter::SetIsHit(false);
	SetLife(MAX_LIFE_SOLDIER);
	m_TaskManager->Reset();	
	SetPosition(m_initPosition);
	SetYaw(m_initYaw);
}


void CSoldier::Update(float ElapsedTime)
{
	if (m_Player==NULL || !GetEnabled())
		return;

	Vect3f l_pla=m_Player->GetPosition();
	Vect3f l_sol=GetPosition();

	if(!m_bFrozen && abs(l_pla.y - l_sol.y)<2.0f)
	{
		if(IsDead())
		{
			//m_TimeDead +=ElapsedTime;
			CORE->GetEnemyManager()->ComunicateTimeDead(this, m_TimeDead);
			CMonster* monster = CORE->GetEnemyManager()->GetMonsterConnected(this);
			if (monster) {
				monster->SetPosition(Vect3f(-GetPosition().x, GetPosition().y, GetPosition().z));				
				//monster->SetYaw(this->GetYaw());
				CORE->GetEnemyManager()->RemoveEnemies(this);
				this->SetCreatePhysics(false);
			}
		}
		else {
			if(m_Player->GetLife() > 0.0f)
			{
				m_TaskManager->update(ElapsedTime);
			}
			else{
				ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
			}
		}
		
	}
	else{
		if(!IsDead()) {
			if(abs(l_pla.y - l_sol.y)>2.0f)
			{
				if(GetLife() > 0.0f)
				{
					ChangeCharacterAnimation(WAIT_ANIM, 0.3f);
				}
				else
				{
					m_TaskManager->update(ElapsedTime);
				}
			}
		}
	}

	CCharacter::Update(ElapsedTime);
}

void CSoldier::SetPosition(const Vect3f &Position)
{
	CCharacter::SetPosition(Position);
}

void CSoldier::SetPosition(const Vect3f & Direction, float ElapsedTime)
{
	CCharacter::SetPosition(Direction, ElapsedTime);
	GetPhysicActor()->SetGlobalPosition(CCharacter::GetPosition() + Vect3f(0.f,SOLDIER_COLLISION_BOX_HEIGHT + SOLDIER_BBOX_HEIGHT,0.f));
}

void CSoldier::SetPlayer(CPlayer * player)
{
	m_TaskManager->SetPlayer(player);
}

void CSoldier::GetAlarm()
{
	m_Alarm = true;
	m_TaskManager->GetAlarm();
}

void CSoldier::Freeze()
{
	m_bFrozen =true;
}

void CSoldier::Unfreeze()
{
	m_bFrozen =false;
}

void CSoldier::GetHurt()
{
	m_TaskManager->GetRoot()->GetSon(0)->GetHurt();
	ExecuteAction(USE_ANIM, 0.3f, 0.3f);		// soldier hurt anim
}

void CSoldier::RemoveCapsuleForBullets()
{
	if(GetCreatePhysics())
		CORE->GetPhysicsManager()->ReleaseActorCapsule(GetPhysicActor());
}

void CSoldier::CreateCapsuleForBullets()
{
	if(GetCreatePhysics())
	{
		GetPhysicActor()->AddCapsuleShape(SOLDIER_COLLISION_BOX_SIZE,SOLDIER_COLLISION_BOX_HEIGHT, CCharacter::GetPosition() + Vect3f(0.f,SOLDIER_COLLISION_BOX_HEIGHT+SOLDIER_BBOX_HEIGHT,0.f),v3fZERO,0,ECG_ESCENARI);
		CORE->GetPhysicsManager()->AddPhysicActor(GetPhysicActor());
	}
}
