#include "Bullet.h"
#include "Camera\Camera.h"
#include "LightManager.h"
#include "RenderableObject.h"
#include "BillboardManager.h"
#include "TextureManager.h"
#include "EnemyManager.h"
#include "Soldier.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Player.h"
#include "AnimatedModelsManager.h"
#include <iostream>
#include "Random.h"
#include "Base.h"


CBullet::CBullet(void): CBillboard(), m_bIsCollided(false), m_fSpeed(0.0f), m_pShotLight(NULL),m_bIsEnded(false)
	,m_fTimer(0.0f),m_bBulletMoving(false),m_vCollisionPoint(NULL),m_fDamage(BULLET_DAMAGE),m_vObjAnimatedPosition(NULL),
	m_fMaxHeight(0.f),m_fMaxWidth(0.f),m_fStepHeight(0.f),m_fStepWidth(0.f),m_vDirection(NULL),m_pCollidedObjectUserData(NULL),m_szCollidedObjectName("")
{
}

CBullet::CBullet(const CBillboard & other): CBillboard(other),m_bIsCollided(false), m_fSpeed(0.0f), m_pShotLight(NULL)
	,m_bIsEnded(false),m_fTimer(0.0f),m_bBulletMoving(false),m_vCollisionPoint(NULL),m_fDamage(BULLET_DAMAGE),m_vObjAnimatedPosition(NULL),
	m_fMaxHeight(0.f),m_fMaxWidth(0.f),m_fStepHeight(0.f),m_fStepWidth(0.f),m_vDirection(NULL),m_pCollidedObjectUserData(NULL),m_szCollidedObjectName("")
{	
}

CBullet::~CBullet(void)
{
}

void CBullet::Update (float _DeltaTime)
{
	if(m_bIsEnded)
		return;

	m_fTimer += _DeltaTime;

	if(m_pShotLight)
		m_pShotLight->SetPosition(CBillboard::GetPos());

	//Halo
	if(!m_bBulletMoving)
	{
		StartEffect(_DeltaTime);
		//ScaleBillboard();
	}
	//Beam
	else if(!m_bIsCollided)
	{
		RayEffect(_DeltaTime);
	}
	else //IsCollided - Impact billboard
	{

		EndEffect();
		//ScaleBillboard();
	}
}

void CBullet::ChangeBillboard(const std::string& _BillboardName)
{
	CBillboard* l_pBillboard = CORE->GetBillboardManager()->GetBillboardCore(_BillboardName);
	if (l_pBillboard == NULL)
		return;

	SetTexture(l_pBillboard->GetTexture());
	SetWidth(l_pBillboard->GetWidth());
	SetHeight(l_pBillboard->GetHeight());

	if(m_bBulletMoving && !m_bIsCollided)
	{
		CBillboard::UpdateAxisBillboard(m_vDirection);
		if(m_pShotLight)
			m_pShotLight->SetEndRangeAttenuation(2.f);
	}
	else if(m_bIsCollided)
	{
		//PrepareBillboardToScale(END_EFFECT_TIME);
		//int blood_frame = 1;

		CSoldier* l_pSoldier =  GetSoldier(m_pCollidedObjectUserData);
		//bool isHeadShoot = CheckHeadShoot(l_pSoldier);
		//if (isHeadShoot)
		//	blood_frame = 0;

		//Ground or ceiling
		if(GetPos().y <GROUND_HEIGHT || GetPos().y >CEILING_HEIGHT)
		{
			//if(m_szCollidedObjectName.compare(0,7,"Soldier")==0)
			if (l_pSoldier!=NULL)
			{
				//std::stringstream bloodBB;
				//bloodBB.clear();
				//bloodBB << "bBlood" << (blood_frame+1);

				CBillboard* pBillboard = CORE->GetBillboardManager()->GetBillboardCore("bBlood");
				if (pBillboard != NULL) {
					SetTexture(pBillboard->GetTexture());
					SetWidth(pBillboard->GetWidth());
					SetHeight(pBillboard->GetHeight());
				}
				SetPos(m_vCollisionPoint);
				CBillboard::Update();
			}
			else {
				SetPos(m_vCollisionPoint);
				CBillboard::UpdateImpactBillboard();
			}
		} 
		else 
		{
			float l_fSurface = 0.15f;
			if (l_pSoldier!=NULL)
			//if(m_szCollidedObjectName.compare(0,7,"Soldier")==0)
			{
				l_fSurface = 0.3f;

				//std::stringstream bloodBB;
				//bloodBB.clear();
				//bloodBB << "bBlood" << (blood_frame+1);

				CBillboard* pBillboard = CORE->GetBillboardManager()->GetBillboardCore("bBlood");
				if (pBillboard != NULL) {
					SetTexture(pBillboard->GetTexture());
					SetWidth(pBillboard->GetWidth());
					SetHeight(pBillboard->GetHeight());
				}
			}

			SetPos(m_vCollisionPoint-m_vDirection*l_fSurface);
			CBillboard::Update();
		}
	}
}

void CBullet::StartEffect(float _DeltaTime)
{
	if(m_pShotLight)
		m_pShotLight->SetEnabled(false);

	CBillboard::Update();

	if(m_fTimer  > START_EFFECT_TIME )
	{
		m_bBulletMoving = true;
		//Check collision before moving
		CheckCollision();
		float l_fDistance = m_vCollisionPoint.Distance(GetPos());

		if(l_fDistance < DIST_TO_WALL)
		{
			SetCollision();
			return;
		}
		else // billboard position is in the middle of the image -> translation to put it at the end of the weapon
		{
			SetPos(GetPos()+m_vDirection*1.6f);
		}

		ChangeBillboard("bLaserBeam");

		if(m_pShotLight)
			m_pShotLight->SetEnabled(true);
	}
}

void CBullet::SetCollision()
{
	if (m_pCollidedObjectUserData==NULL)
		return;

	m_bIsCollided = true;
	m_fTimer = 0.0f;
	m_szCollidedObjectName = static_cast<const std::string&>(m_pCollidedObjectUserData->GetName());
	ChangeBillboard("bReboteDisparo");
}

void CBullet::RayEffect(float _DeltaTime)
{
	//last collision check
	float l_fOldDistance=MAX_DISTANCE;
	Vect3f l_vOldCollisionPoint=NULL;
	CPhysicUserData* l_pOldCollisionObjUserData=NULL;
	Vect3f l_vOldObjAnimatedPosition=NULL;
	if(m_vCollisionPoint!=NULL)
	{
		l_fOldDistance=m_vCollisionPoint.Distance(GetPos());
		l_vOldCollisionPoint = m_vCollisionPoint;
		l_pOldCollisionObjUserData = m_pCollidedObjectUserData;
		if(m_vObjAnimatedPosition!=NULL)
			l_vOldObjAnimatedPosition = m_vObjAnimatedPosition;
	}
	//update position bullet
	SetPos(GetPos()+m_fSpeed*m_vDirection*_DeltaTime);
	CheckCollision();

	//no collision
	if(m_pCollidedObjectUserData==NULL)
	{
		if(l_pOldCollisionObjUserData!=NULL)//no collision after having collided
		{
			m_vCollisionPoint=l_vOldCollisionPoint;
			m_pCollidedObjectUserData=l_pOldCollisionObjUserData;
			SetCollision();
		}

	}
	else{ //collision

		float l_fDistance = m_vCollisionPoint.Distance(GetPos());

		if(l_fDistance < SCALE_DISTANCE)
		{
			SetHeight(GetHeight()/2.f);
			SetWidth(GetWidth()/2.f);
		}
		CBillboard::UpdateAxisBillboard(m_vDirection);


		//collision with another object different from the last collision check
		if(l_pOldCollisionObjUserData != m_pCollidedObjectUserData )
		{
			if(!IsAnimatedObject(l_pOldCollisionObjUserData))
			{
				m_vCollisionPoint=l_vOldCollisionPoint;
				SetCollision();
			}
			else  // collision with a static object after a collision with an animated object (animat.object can move)
			{
				GetCollidedCharacterPosition(l_pOldCollisionObjUserData);
				float distance = m_vObjAnimatedPosition.Distance(l_vOldObjAnimatedPosition);
				if(distance < MOVEMENT_DISTANCE)
				{ 
					m_vCollisionPoint=l_vOldCollisionPoint;
					m_pCollidedObjectUserData=l_pOldCollisionObjUserData;
					SetCollision();
				}
				else 
					if (l_fDistance < DIST_TO_WALL) 
						SetCollision();
			}
		}
		else // collision with the same object as the last collision check
		{
			if(l_fDistance < DIST_TO_WALL +0.5f)
				SetCollision();
		}
	}
}

void CBullet::ScaleBillboard() 
{
	if(GetHeight()> m_fMaxHeight)
	{
		m_fStepHeight = -m_fStepHeight;
		m_fStepWidth  = -m_fStepWidth;
	}

	if((GetHeight() + m_fStepHeight)>0.f && (GetWidth() + m_fStepWidth)>0)

	{
		SetHeight(GetHeight() + m_fStepHeight);
		SetWidth(GetWidth() + m_fStepWidth);
	}
}

void CBullet::EndEffect()
{

	if(m_szCollidedObjectName != "")
	{
		CheckDamageCaused();
		m_szCollidedObjectName = "";
	}

	if(GetPos().y <GROUND_HEIGHT || GetPos().y >CEILING_HEIGHT){
		CBillboard::UpdateImpactBillboard();
	} 
	else 
	{
		CBillboard::Update();
	}

	//bReboteLineaDisparo , bReboteDisparo
	if(m_fTimer > END_EFFECT_TIME)
	{
		m_bIsEnded=true;
		if(m_pShotLight)
		{
			m_pShotLight->SetEnabled(false);
			m_pShotLight->SetActive(false);
		}
	}
}

void CBullet::Init(CCharacter* player)
{

	if(m_pShotLight!=NULL){
		m_pShotLight->SetPosition(CBillboard::GetPos());
	}

	m_fSpeed = BULLET_SPEED;

	//PrepareBillboardToScale(START_EFFECT_TIME);

	m_Player = player;
}

void CBullet::PrepareBillboardToScale(float _Time){

	m_fMaxHeight=GetHeight();
	m_fMaxWidth=GetWidth();

	SetHeight(0.0f);
	SetWidth(0.0f);

	m_fStepHeight = m_fMaxHeight / (_Time/2.f)/30.f;
	m_fStepWidth = m_fMaxWidth / (_Time/2.f)/30.f;
}

void CBullet::CheckCollision(){

	//Collision Detection
	SCollisionInfo l_sCollision;
	uint32 l_uimpactMask = 5<<0; // BULLETS MASK

	CPhysicUserData * l_pUserData=CORE->GetPhysicsManager()->RaycastClosestActorShoot (GetPos(),m_vDirection , l_uimpactMask, l_sCollision, m_fSpeed);

	if(l_pUserData==NULL ) 
	{
		m_szCollidedObjectName="";
		m_vCollisionPoint=NULL;
		m_pCollidedObjectUserData = NULL;
		return;
	}
	else 
	{
		m_pCollidedObjectUserData = l_pUserData;
		m_vCollisionPoint = l_sCollision.m_CollisionPoint;
		if(IsAnimatedObject(m_pCollidedObjectUserData))
		{
			GetCollidedCharacterPosition(m_pCollidedObjectUserData);
		}
		else
			m_vObjAnimatedPosition = NULL;

	}
}

bool CBullet::CheckHeadShoot(CSoldier* pSoldier)
{
	if(pSoldier != NULL)
	{
		float fDamage = m_fDamage;
		if (pSoldier->GetPosition().y <= 1.0f)
			if (m_vCollisionPoint.y > 1.6f && m_vCollisionPoint.y <= 1.85f)
				return true;

		if (pSoldier->GetPosition().y >= 3.0f)
			if (m_vCollisionPoint.y > 4.6f && m_vCollisionPoint.y <= 4.85f)
				return true;
	}

	return false;
}

void CBullet::GetCollidedCharacterPosition(CPhysicUserData *_PhysicUserData) {


	if(_PhysicUserData==NULL) 
		return;

	CCharacter* l_pCharacter; 
	m_vObjAnimatedPosition = NULL;

	l_pCharacter = m_Player;
	if(l_pCharacter!=NULL && _PhysicUserData == l_pCharacter->GetPhysicUserData())
	{
		m_vObjAnimatedPosition = l_pCharacter->GetPosition();
	}
	else //enemies
	{
		l_pCharacter =  GetSoldier(_PhysicUserData);
		if(l_pCharacter!=NULL && _PhysicUserData == l_pCharacter->GetPhysicUserData()) 
			m_vObjAnimatedPosition = l_pCharacter->GetPosition();
	}

}

void CBullet::CheckDamageCaused()
{

	// player
	CCharacter * l_pPlayer = m_Player;
	if(l_pPlayer != NULL && m_pCollidedObjectUserData == l_pPlayer->GetPhysicUserData())
	{
		l_pPlayer->TakeDamage(m_fDamage);
	}
	else //enemies
	{
		CSoldier* l_pSoldier =  GetSoldier(m_pCollidedObjectUserData);
		if(l_pSoldier != NULL)
		{
			float fDamage = m_fDamage;
			bool isHeadShoot = CheckHeadShoot(l_pSoldier);
			if (isHeadShoot)
				fDamage *= 10.0f;

			l_pSoldier->TakeDamage(fDamage);
			l_pSoldier->GetHurt();
		}
	}
}

bool CBullet::IsAnimatedObject(CPhysicUserData *_PhysicUserData)
{
	if(_PhysicUserData==NULL) 
		return false;

	CCharacter *l_pCharacter;
	l_pCharacter = m_Player;
	if(l_pCharacter!=NULL && _PhysicUserData == l_pCharacter->GetPhysicUserData())
	{
		return true;
	}
	else //enemies
	{
		l_pCharacter = GetSoldier(_PhysicUserData);
		if(l_pCharacter!=NULL && _PhysicUserData == l_pCharacter->GetPhysicUserData())
			return true;
	}

	return false;
}

inline CSoldier* CBullet::GetSoldier(CPhysicUserData *_PhysicUserData)
{

	return  (CSoldier*)CORE->GetEnemyManager()->GetSoldier(_PhysicUserData);
}


