#pragma once

#include "MeshInstance.h"


class CSpotLight;
class CPlayer;
class CXMLTreeNode;
class CPhysicController;


enum TWeaponMode	{ 
	GUN,
	LANTERN
};

class CWeapon //: public CInstanceMesh
{
public:
	/*CWeapon(const CXMLTreeNode& XmlData);
	CWeapon(const std::string &Name, const std::string &CoreName);*/

	CWeapon();
	~CWeapon(void);
	void Init();
	void ReloadWeapon();
	void SetLanternLight (CSpotLight* Light);
	void SetDamage(float Damage)
	{
		m_fDamage = Damage;
	}
	float GetDamage()
	{
		return m_fDamage;
	}
	void ChangeWeaponMode()
	{
		m_tMode= (m_tMode == GUN)? LANTERN:GUN;
	}
	TWeaponMode GetWeaponMode()
	{
		return m_tMode;
	}
	void SetWeaponMode(TWeaponMode mode)
	{
		m_tMode = mode;
	}
	void SetForwardDirection();
	void SetForwardDirection(Vect3f Forward)
	{
		m_vForward = Forward.Normalize();
	}
	Vect3f& GetForwardDirection()
	{
		return m_vForward;
	}
	void SetPlayer(CPlayer * Player)
	{
		m_oPlayer=Player;
	}
	CPlayer * GetPlayer() 
	{
		return m_oPlayer;
	}

	Vect3f& GetShotOriginPosition() { return m_vLightPosition;}
	bool IsAiming();
	bool IsLanternAttackMode();
	float GetAvailableMunition();
	void SetAvailableMunition(float bullets);
	void Shoot(CPlayer* player);
	void Light(float dt);
	void SetLightPosition();
	void Update(float dt, CPlayer* player);
	void RechargeWeapon();
	void PartiallyRechargeWeapon();
	float GetLanternDistance();
	bool IsMunitionFull();
	bool IsMunitionEmpty();
	bool CheckShotDuration();
	bool WeaponCollision(float Yaw=0.f);

	bool AimingWeaponCollision();
	bool WasAimingWeaponCollision();

	int GetMaxBullets();
	float GetLanternMaxPowerTime();

private:
	//vida que llevara cada bala
	float m_fDamage;
	//direccio arma
	Vect3f m_vForward;

	//Energia de la llanterna i bales (proporcional)
	float m_fLanternTimer;
	int m_iAvailableBullets;
	float m_fShotTimer;
	//tipus arma
	TWeaponMode m_tMode;

	CPlayer * m_oPlayer;
	//Llum llanterna
	CSpotLight *m_LanternLight,*m_Laser;
	Vect3f		m_vLightPosition;

	Vect3f m_vWeaponBonePosition;

};

