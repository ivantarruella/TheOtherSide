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

#define	SOLDIER_BBOX_HEIGHT		0.9f
#define	SOLDIER_BBOX_SIZE		0.35f
#define MAX_LIFE_SOLDIER		4.0f

CSoldier::CSoldier()
	:m_Alarm(false), m_bFrozen(false), m_TimeDead(0.0f), m_Cover(-1)
{


}

CSoldier::CSoldier(const Vect3f &pos, const std::string &CoreName, const std::string &Name):CCharacter(pos, CoreName, Name, SOLDIER_BBOX_SIZE,SOLDIER_BBOX_HEIGHT)
{	
	m_Cover = -1;
	m_TimeDead = 0.0f;
	m_Alarm = false;
	m_bFrozen = false;
	m_TaskManager = new CTaskManager();
	CRootTask* l_Root = new CRootTask();
	CIdleTask* l_Idle = new CIdleTask();
	CPatrolTask* l_Patrol = new CPatrolTask();
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
	l_Root->AddSon(l_Patrol);
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
	l_Patrol->SetOwner(this);
	m_TaskManager->SetRoot(l_Root);

	SetLife(MAX_LIFE_SOLDIER);
	m_initPosition = GetPosition();
	m_initYaw = GetYaw();
	CreateCapsuleForBullets();
	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
}

CSoldier::CSoldier(const CXMLTreeNode& XmlData):CCharacter(XmlData, SOLDIER_BBOX_SIZE,SOLDIER_BBOX_HEIGHT)
{	
	m_Cover = -1;
	m_TimeDead = 0.0f;
	m_Alarm = false;
	m_bFrozen = false;
	m_TaskManager = new CTaskManager();
	CRootTask* l_Root = new CRootTask();
	CIdleTask* l_Idle = new CIdleTask();
	CPatrolTask* l_Patrol = new CPatrolTask();
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
	l_Root->AddSon(l_Patrol);
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
	l_Patrol->SetOwner(this);
	m_TaskManager->SetRoot(l_Root);

	SetLife(MAX_LIFE_SOLDIER);
	m_initPosition = GetPosition();
	m_initYaw = GetYaw();
	CreateCapsuleForBullets();
	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
}

CSoldier::~CSoldier()
{
	CHECKED_DELETE(m_TaskManager);
	CHECKED_DELETE(m_PhysicElement);
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


void CSoldier::RemoveCapsuleForBullets()
{
	if(GetCreatePhysics())
		CORE->GetPhysicsManager()->ReleaseActorCapsule(GetPhysicActor());
}

void CSoldier::Update(float ElapsedTime)
{
	if (m_Player==NULL)
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
	if(GetPhysicActor())
		GetPhysicActor()->SetGlobalPosition(Position+Vect3f(0.f,SOLDIER_CAPSULE_HEIGHT,0.f));
}

void CSoldier::SetPosition(const Vect3f & Direction, float ElapsedTime)
{
	CCharacter::SetPosition(Direction,  ElapsedTime);
	if(GetPhysicActor() )
		GetPhysicActor()->SetGlobalPosition(GetPosition()+Vect3f(0.f,SOLDIER_CAPSULE_HEIGHT,0.f));
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

#if 1
void CSoldier::CreateCapsuleForBullets()
{
	if(GetCreatePhysics())
	{
		GetPhysicActor()->AddCapsuleShape(SOLDIER_BBOX_SIZE,SOLDIER_BBOX_HEIGHT, m_Position+Vect3f(0.f,SOLDIER_BBOX_HEIGHT,0.f),v3fZERO,0,ECG_ENEMICS);
		CORE->GetPhysicsManager()->AddPhysicActor(GetPhysicActor());
	}
}
#else
void CSoldier::CreateCapsuleForBullets()
{
	if(GetCreatePhysics())
	{
		Mat44f l_TransfMat;
		l_TransfMat.SetIdentity();
		Vect3f l_Pos;
		CalSkeleton* l_pSkeleton = this->GetCalModel()->getSkeleton();
		int l_BoneIdx = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("esbirro Head");
		if (l_BoneIdx != -1)
		{
			CalCoreBone* l_pBone = l_pSkeleton->getCoreSkeleton()->getCoreBone(l_BoneIdx);
			CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
			CalQuaternion l_vRotation = l_pBone->getRotationAbsolute();
			l_TransfMat = this->GetTransform();
			Vect3f l_Pos = l_TransfMat * /** Vect3f(l_vRotation.x,l_vRotation.y,l_vRotation.z)* */Vect3f(l_vTranslation.x, l_vTranslation.y+0.15f, l_vTranslation.z) ;
			GetPhysicActor()->AddSphereShape(0.15f,l_Pos,v3fZERO,0,ECG_ENEMICS);
		}

		l_TransfMat.SetIdentity();
		l_BoneIdx = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("esbirro Pelvis");
		if (l_BoneIdx != -1)
		{
			CalCoreBone* l_pBone = l_pSkeleton->getCoreSkeleton()->getCoreBone(l_BoneIdx);
			CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
			//CalQuaternion l_vRotation = l_pBone->getRotationAbsolute();
			l_TransfMat = this->GetTransform();
			Vect3f l_Pos = l_TransfMat * Vect3f(l_vTranslation.x, l_vTranslation.y, l_vTranslation.z) ;
			GetPhysicActor()->AddCapsuleShape(0.3f,1.8f,l_Pos,v3fZERO,0,ECG_ENEMICS);
		}

		l_TransfMat.SetIdentity();
		l_BoneIdx = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("esbirro R Forearm");
		if (l_BoneIdx != -1)
		{
			CalCoreBone* l_pBone = l_pSkeleton->getCoreSkeleton()->getCoreBone(l_BoneIdx);
			CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
			CalQuaternion l_vRotation = l_pBone->getRotationAbsolute();
			l_TransfMat = this->GetTransform();
			Vect3f l_Pos = l_TransfMat * Vect3f(l_vRotation.x,l_vRotation.y,l_vRotation.z)* Vect3f(l_vTranslation.x, l_vTranslation.y, l_vTranslation.z) ;
			GetPhysicActor()->AddCapsuleShape(0.15f,0.9f,l_Pos,v3fZERO,0,ECG_ENEMICS);
		}

		l_TransfMat.SetIdentity();
		l_BoneIdx = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("esbirro L Forearm");
		if (l_BoneIdx != -1)
		{
			CalCoreBone* l_pBone = l_pSkeleton->getCoreSkeleton()->getCoreBone(l_BoneIdx);
			CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
			CalQuaternion l_vRotation = l_pBone->getRotationAbsolute();
			l_TransfMat = this->GetTransform();
			Vect3f l_Pos = l_TransfMat * Vect3f(l_vRotation.x,l_vRotation.y,l_vRotation.z)* Vect3f(l_vTranslation.x, l_vTranslation.y, l_vTranslation.z) ;
			GetPhysicActor()->AddCapsuleShape(0.15f,0.9f,l_Pos,v3fZERO,0,ECG_ENEMICS);
		}

		GetPhysicActor()->AddSphereShape(0.15f, m_Position+Vect3f(0.f,1.85f-GetHeight(),0.f),v3fZERO,0,ECG_ENEMICS);
		GetPhysicActor()->AddCapsuleShape(0.4f,1.8f, m_Position+Vect3f(0.f,1.17f-SOLDIER_BBOX_HEIGHT,0.f),v3fZERO,0,ECG_ENEMICS);

		//CreatePhysics(m_Position+Vect3f(0.f,1.17f-SOLDIER_BBOX_HEIGHT,0.f),0.4f,1.8f,ECG_ENEMICS);
		CORE->GetPhysicsManager()->AddPhysicActor(GetPhysicActor());
	}
}
#endif