#include "Weapon.h"
#include "include\PhysicsManager.h"
#include "SpotLight.h"
#include "LightManager.h"
#include "Camera\Camera.h"
#include "AnimatedModelsManager.h"
#include "Player.h"
#include "Soldier.h"
#include "Monster.h"
#include "EnemyManager.h"
#include "SoundManager.h"
#include "BillboardManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "BulletManager.h"
#include "Camera\ThirdPersonCamera.h"

#include "Base.h"

#define	LANTERN_MAXPOWERTIME	60.0f	//tiempo de potencia de maxima luz (s)

#define POWERGUN				5.0f			//velocidad de tiro del arma (balas x seg)
#define MAXBULLETS				90.0f			//numero maximo de balas

#define DAMAGE					0.5f			//vida que quita cada impacto
#define SECPERBULLET			(LANTERN_MAXPOWERTIME/MAXBULLETS)
#define BULLETPERSEC			(MAXBULLETS/LANTERN_MAXPOWERTIME)

#define	SHOT_KEY_PRESSED		0.1f
#define SHOT_STEP_DURATION		0.2f

#define BOUNDING_SPHERE			0.25f
#define OVERLAP_SPHERE_SIZE		0.15f
#define OVERLAP_SPHERE_SIZE_BIG	0.50f

class CThirdPersonCamera;

CWeapon::CWeapon():m_LanternLight(NULL),m_vForward (NULL),m_oPlayer(NULL),m_vLightPosition(NULL),m_Laser(NULL),m_fShotTimer(0.0f),m_vWeaponBonePosition(NULL)
{
	Init();	

}

CWeapon::~CWeapon(void)

{

}


void CWeapon::Init()
{
	m_fDamage = DAMAGE;
	m_fLanternTimer = LANTERN_MAXPOWERTIME;
	m_tMode = GUN;
	m_iAvailableBullets = (int)MAXBULLETS;
	//Para que empiece disparando
	m_fShotTimer=SHOT_STEP_DURATION;
}

bool CWeapon::IsLanternAttackMode(){

	return GetWeaponMode() == LANTERN && m_oPlayer->GetPlayerAimming() && m_oPlayer->GetPlayerShoot();

}

bool CWeapon::IsAiming()
{
	return GetWeaponMode() == LANTERN && m_oPlayer->GetPlayerAimming() && !m_oPlayer->GetPlayerShoot();
}

void CWeapon::ReloadWeapon()
{
	m_LanternLight = (CSpotLight*)CORE->GetLightManager()->GetResource("LINTERNA");
	m_Laser = (CSpotLight*)CORE->GetLightManager()->GetResource("LASER");

}

void CWeapon::SetLanternLight (CSpotLight* Light)
{
	m_LanternLight = Light;
}

void CWeapon::Light(float dt)
{
	if(CORE->GetCamera()->GetTypeCamera()!=TC_FPS)
	{
		if(m_fLanternTimer <= 0.f )
		{
			m_LanternLight->SetVisible(false);
			m_Laser->SetVisible(false);
		}
		else
		if(IsLanternAttackMode() && m_Laser != NULL) 
		{
			m_LanternLight->SetVisible(false);
			m_Laser->SetVisible(true);

			m_fLanternTimer -= 2.f*dt;
			m_iAvailableBullets = (int)(BULLETPERSEC*m_fLanternTimer);
			//Check lighted enemies
			CORE->GetEnemyManager()->CheckLightedMonsters(m_LanternLight);

		}
		else if(m_LanternLight != NULL){
			bool bAiming = IsAiming();
			m_LanternLight->SetVisible(bAiming);
			m_Laser->SetVisible(false);

			if (bAiming) {
				m_fLanternTimer -= 0.1f*dt;
				m_iAvailableBullets = (int)(BULLETPERSEC*m_fLanternTimer);
			}
		}
	}
}

void CWeapon::Shoot(CPlayer* player){

	SCollisionInfo l_Collision;
	//uint32 impactMask = 1<<0;

	m_fShotTimer = 0.0f;

	m_fLanternTimer -= SECPERBULLET;
	m_iAvailableBullets--;

	CORE->GetBulletManager()->AddBullet(m_vLightPosition,m_vForward, player);
}

float CWeapon::GetAvailableMunition()
{
	return (GetWeaponMode() == LANTERN)? m_fLanternTimer:(float)m_iAvailableBullets;
}

void CWeapon::SetAvailableMunition(float Bullets)
{
	m_iAvailableBullets = (int)Bullets;
}

bool CWeapon::IsMunitionFull()
{
	if (GetWeaponMode() == LANTERN && m_fLanternTimer >= LANTERN_MAXPOWERTIME)
		return true;

	if (GetWeaponMode() == GUN && m_iAvailableBullets >= MAXBULLETS)
		return true;

	return false;
}

bool CWeapon::IsMunitionEmpty()
{
	if (GetWeaponMode() == LANTERN && m_fLanternTimer <= 0.f)
		return true;

	if (GetWeaponMode() == GUN && m_iAvailableBullets <= 0)
		return true;

	return false;
}

void CWeapon::SetForwardDirection()
{
	CCamera * l_Camera = CORE->GetCamera();

	if( GetWeaponMode()==GUN &&((CThirdPersonCamera*)CORE->GetCamera())->GetCollisionPoint() !=NULL )
		m_vForward = ((CThirdPersonCamera*)CORE->GetCamera())->GetCollisionPoint() - m_vLightPosition;
	else{

		float l_fYaw, l_fPitch, l_fRoll;

		if(m_oPlayer->GetPlayerAimming())
		{
			l_fYaw = l_Camera->GetPlayerYaw();
			l_fPitch = l_Camera->GetPlayerPitch();
			l_fRoll = l_Camera->GetPlayerRoll();
		}
		else
		{
			l_fYaw = m_oPlayer->GetYaw()+FLOAT_PI_VALUE/2.f-0.3f;
			l_fPitch = m_oPlayer->GetPitch()-FLOAT_PI_VALUE/6.f;
			l_fRoll = m_oPlayer->GetRoll();
		}
		m_vForward = Vect3f(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch), sin(l_fYaw) * cos(l_fPitch));
	}


	m_vForward.Normalize();

	if(m_LanternLight!=NULL)
		m_LanternLight->SetDirection(m_vForward);
	if(m_Laser!=NULL)
		m_Laser->SetDirection(m_vForward);
}


void CWeapon::Update(float dt, CPlayer* player)
{
	m_fShotTimer += dt;

	if(m_LanternLight==NULL)
		m_LanternLight = (CSpotLight*)CORE->GetLightManager()->GetResource("LINTERNA");
	if(m_Laser==NULL)
		m_Laser = (CSpotLight*)CORE->GetLightManager()->GetResource("LASER");

	if(CORE->GetCamera()!=NULL){
		SetLightPosition();
		SetForwardDirection();
		
		if(GetWeaponMode() == LANTERN)
		{

			Light(dt);
		}
		else 
		{
			if (m_LanternLight!=NULL)
				m_LanternLight->SetVisible(false);
			if (m_Laser!=NULL)
				m_Laser->SetVisible(false);
			if(m_oPlayer->GetPlayerAimming())
			{
				if(m_oPlayer->GetPlayerShoot())
					if(m_iAvailableBullets > 0 && CheckShotDuration())
						Shoot(player);
			}
		}
	}


}

bool CWeapon::CheckShotDuration()
{
	if(m_oPlayer->GetShootKeyTime()>SHOT_KEY_PRESSED)
	{
		if(m_fShotTimer > m_oPlayer->GetAnimationDuration()/POWERGUN)
		{
			return true;
		}
	}
	else if(m_oPlayer->GetShootKeyTime() <=SHOT_KEY_PRESSED && m_fShotTimer > SHOT_STEP_DURATION)
	{

		return true;
	}

	return false;
}

//posicion desde donde sale la luz para linterna o las particulas para arma
void CWeapon::SetLightPosition()
{
	if (CORE->GetCamera()==NULL)
		return;

	Mat44f l_TransfMat;
	l_TransfMat.SetIdentity();

	// calculo de las coordenadas del hueso donde esta la pistola para hacer salir la luz desde esta posicion
	CalSkeleton* l_pSkeleton = m_oPlayer->GetCalModel()->getSkeleton();
	int l_BoneIdx = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Dummy_Arma");
	if (l_BoneIdx != -1)
	{
		CalBone* l_pBone = l_pSkeleton->getBone(l_BoneIdx);
		CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
		l_TransfMat = m_oPlayer->GetTransform();
		m_vWeaponBonePosition = l_TransfMat * Vect3f(l_vTranslation.x, l_vTranslation.y, l_vTranslation.z+0.1f) ;
		bool bIsAiming = (m_oPlayer!=NULL && m_oPlayer->GetPlayerAimming());
		if(bIsAiming)
		{
			float pitch = CORE->GetCamera()->GetPlayerPitch();
			float dy = pitch/10.f;
			if ( dy > 0.f)
				dy *= 2.f;			
			m_vLightPosition = l_TransfMat * Vect3f(l_vTranslation.x, l_vTranslation.y+dy, l_vTranslation.z+0.185f) ;
		}
		else
			m_vLightPosition = l_TransfMat * Vect3f(l_vTranslation.x+0.03f, l_vTranslation.y-0.115f, l_vTranslation.z+0.185f) ;

		if(m_LanternLight!=NULL)
			m_LanternLight->SetPosition(m_vLightPosition);
		if(m_Laser!=NULL)
			m_Laser->SetPosition(m_vLightPosition);
	}
}


void CWeapon::RechargeWeapon()
{
	m_fLanternTimer = LANTERN_MAXPOWERTIME;
	m_iAvailableBullets = (int)MAXBULLETS;
}

void CWeapon::PartiallyRechargeWeapon()
{
	m_fLanternTimer += LANTERN_MAXPOWERTIME/MAXBULLETS;
	m_iAvailableBullets++;
}

float CWeapon::GetLanternDistance() 
{ 
	if(m_Laser!=NULL)
		return m_Laser->GetEndRangeAttenuation();
	return 0.0f;
}

bool CWeapon::WeaponCollision(float Yaw)
{
	bool colision = CORE->GetPhysicsManager()->CheckOverlapSphere(m_vWeaponBonePosition,OVERLAP_SPHERE_SIZE);

	return colision;
}


bool CWeapon::AimingWeaponCollision()
{
	Vect3f l_Pos (m_vWeaponBonePosition+m_oPlayer->GetFront()*0.3f + Vect3f(0.f,0.35f,0.f));
	bool colision_forward = CORE->GetPhysicsManager()->CheckOverlapSphere(l_Pos, OVERLAP_SPHERE_SIZE);

	Vect3f l_Pos2 (m_vWeaponBonePosition-m_oPlayer->GetFront()*0.3f + Vect3f(0.f,0.35f,0.f));
	bool colision_backward = CORE->GetPhysicsManager()->CheckOverlapSphere(l_Pos2, OVERLAP_SPHERE_SIZE);

	return (colision_forward || colision_backward);
}

bool CWeapon::WasAimingWeaponCollision()
{
	Vect3f l_Pos (m_vWeaponBonePosition+m_oPlayer->GetFront()*0.3f + Vect3f(0.f,0.35f,0.f));
	bool colision = CORE->GetPhysicsManager()->CheckOverlapSphere(l_Pos, OVERLAP_SPHERE_SIZE_BIG);

	return colision;
}

int CWeapon::GetMaxBullets()
{
	return (int)MAXBULLETS;
}

float CWeapon::GetLanternMaxPowerTime()
{
	return LANTERN_MAXPOWERTIME;
}