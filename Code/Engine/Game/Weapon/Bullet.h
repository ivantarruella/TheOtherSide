#ifndef BULLET_CLASS
#define BULLET_CLASS

#include "BillboardAnimation.h"
#include "OmniLight.h"
#include "Character.h"
#include "include\PhysicsManager.h"
#include <string>

#define BULLET_SPEED 30.f
#define START_EFFECT_TIME 0.1f
#define END_EFFECT_TIME	0.15f
#define BULLET_DAMAGE	0.5f
#define DIST_TO_WALL	0.75f
#define MAX_DISTANCE	100.f
#define SCALE_DISTANCE	2.0f
#define MOVEMENT_DISTANCE 0.15f

class CRenderableObject;
class CPhysicUserData;
class CPlayer;
class CSoldier;

class CBullet: public CBillboard, public CNamed
{
public:
	CBullet(void);
	CBullet(const CBillboard &);
	~CBullet(void);

	void Init(CCharacter* player);
	bool IsCollided() { return m_bIsCollided; }
	bool IsEnded() { return m_bIsEnded; }

	void SetDirection(Vect3f &_Direction)
	{
		m_vDirection = _Direction;
	}
	Vect3f GetDirection()	{ return m_vDirection; }

	void SetBulletSpeed(float _Speed)
	{
		m_fSpeed = _Speed;
	}
	float GetBulletSpeed()	{ return m_fSpeed; }

	float GetDamage()	{ return m_fDamage;	}
	void SetDamage(float _Damage)
	{
		m_fDamage = _Damage;
	}

	void SetLight(COmniLight* _Light) { 
		m_pShotLight=_Light;
	}

	void StartEffect(float);
	void RayEffect(float);
	void EndEffect();

	void ChangeBillboard(const std::string&);
	void CheckDamageCaused();
	void CheckCollision();
	bool CheckHeadShoot(CSoldier* pSoldier);
	void SetCollision();
	bool IsAnimatedObject(CPhysicUserData *);
	void ScaleBillboard();
	void PrepareBillboardToScale(float);
	void GetCollidedCharacterPosition(CPhysicUserData *) ;
	void Update(float);

private:


	Vect3f m_vDirection, m_vCollisionPoint, m_vObjAnimatedPosition;
	COmniLight *m_pShotLight;
	CPhysicUserData *m_pCollidedObjectUserData;
	std::string m_szCollidedObjectName;

	bool m_bIsCollided,m_bIsEnded,m_bBulletMoving;

	float m_fSpeed, m_fTimer;
	float m_fMaxHeight,m_fMaxWidth;
	float m_fStepHeight,m_fStepWidth;
	float m_fDamage;

	CCharacter* m_Player;

	CSoldier* GetSoldier(CPhysicUserData *);

};

#endif