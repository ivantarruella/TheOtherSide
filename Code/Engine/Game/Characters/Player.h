#ifndef __CLASS_PLAYER_H__
#define __CLASS_PLAYER_H__

#pragma once

#include "Character.h"
#include "World.h"
#include "Weapon\Weapon.h"


class CRenderManager;
class CCamera;
class CCharacter;
class CXMLTreeNode;

class CPlayer : public CCharacter
{
public:
	CPlayer();
	CPlayer(CXMLTreeNode XMLData);
	virtual ~CPlayer();

	void Update(float ElapsedTime);
	void Load();
	void Reload();
	
	void SetPlayerSpeed(float Speed) { m_fSpeed = Speed; }
	float GetPlayerSpeed() { return m_fSpeed; }
	
	bool GetPlayerAimming() { return m_bIsAiming; }
	bool GetPlayerWasAimming() { return m_bWasAiming; }
	bool GetPlayerRunning() { return m_bIsRunning; }
	bool GetPlayerShoot() { return m_bShoot; }
	bool GetPlayerLinterna() { return (m_bIsAiming && m_World.GetWorld()==CWorld::MIRROR_WORLD); }
	
	float GetPlayerLifeGUI();
	float GetPlayerMunitionGUI();

	void ChangeWorld(bool change, const Vect3f& SpawnPos);
	bool isInRealWorld(){return m_World.GetWorld()==CWorld::REAL_WORLD;};
	void SetIniPos(const Vect3f& pos) { m_vPosIni = pos; }
	
	void SetCanUseItem(bool use) { m_bCanUse = use; }
	bool GetUseItem() { return m_bUseItem; }
	bool GetUseButtonPressed() { return m_bUsePressed; }
	void SetUseItemAnim(tAnimationStates anim) { m_tUseAnim = anim; }

	void SetMouseSpeed(float speed) { m_fMouseSpeed = speed; }
	float GetMouseSpeed() { return m_fMouseSpeed; }

	void IncMirrorPiece() { m_iNumMirrorPieces++; }
	int GetMirrorPieces() { return m_iNumMirrorPieces; }

	float GetHitTime() { return m_fHitTime; }
	float GetHitTotalTime();

	float GetMaxPlayerLife();
//Lua
	void RegisterPlayer();

//Overrided functions
	void SetPosition(const Vect3f &Position);
	void SetPosition(const Vect3f & Direction, float ElapsedTime);
	void SetYaw(float Yaw);
	void SetPitch(float Pitch);
	void SetRoll(float Roll);

	void RecoverLife();
	void PartiallyRecoverLife(float Quantity = 0.1f);

	float GetLanternDistance();
	void UpdateShoot();

	CWeapon& GetWeapon() { return m_oWeapon; }
//	void DeleteWeapon() { m_oWeapon = NULL; }
	float GetShootKeyTime(){	return m_fShootKeyPressedTime;}

private:
	CPlayer* getPlayer() {return this;}
	void UpdateInputActions();
	void UpdateCamera(float ElapsedTime);
	void UpdatePlayerPosition(float ElapsedTime);
	void UpdatePlayerSpeed();
	void UpdatePlayerOrientation();
	
	void UpdatePlayerAnimations(float ElapsedTime);
	void UpdatePlayerDeadAnim(float ElapsedTime);
	void UpdatePlayerMoveAnim(float ElapsedTime);
	void UpdatePlayerShootAnim(float ElapsedTime);
	void UpdatePlayerUseAnim(float ElapsedTime);
	void UpdatePlayerWeapon(float ElapsedTime);
	void UpdatePlayerHit(float ElpasedTime);

private:
	CWorld m_World;
	CCamera* m_Camera;
	float m_fDeltaPitch, m_fDeltaYaw;
	float m_fUpDown, m_fLeftRight;
	float m_fSpeed;
	tPlayerStates m_PlayerMovement;
	tAnimationStates m_lastAnim;
	bool m_bIsRunning;
	bool m_bIsAiming;
	bool m_bWasAiming;
	bool m_bShoot;
	float m_fZoom;		
	float m_fDistance;
	float m_fDistanceFromCenter;
	bool m_bCanUse, m_bUseItem, m_bUseAnim, m_bUsePressed;
	float m_fUseAnimTime;
	CWeapon m_oWeapon;
	Vect3f m_vPosIni;
	float m_fMouseSpeed;
	float m_fShootKeyPressedTime;
	int m_iNumMirrorPieces;
	bool m_bPlayerDying, m_bIsDead;
	float m_fTime, m_fUpdateCam;
	tAnimationStates m_tUseAnim;
	float m_fHitTime, m_fStepTime, m_fShotTime;
	bool m_bBlood;
};

#endif	// __CLASS_PLAYER_H__

