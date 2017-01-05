#ifndef __CLASS_GAMEPROCESS_H__
#define __CLASS_GAMEPROCESS_H__

#pragma once

#include "Process.h"
#include "Matrix44.h"

#include <vector>
#include <string>

class CObject3D;
class CPlayer;
class CMonster;
class CSpotLight;
class CInstanceMesh;	
class CWeapon;


#define CAMERA_ZNEAR			0.01f
#define CAMERA_ZFAR				55.0f
#define CAMERA_FOV				(55.0f * FLOAT_PI_VALUE/180.0f)

#define GUI_LIVE_WIDTH			24.0f
#define GUI_ENERGY_WIDTH		24.0f

#define MAX_DAMAGE_TIME			3.0f

#define MOUSE_SENSITIVITY		0.010f		// velocidad actualización ratón por defecto
#define MOUSE_SENSITIVITY_MIN	0.001f
#define MOUSE_SENSITIVITY_MAX	0.02f

#define CAMERA_INI_POS			(Vect3f(23.f, 0.7f, -3.f))


class CGameProcess :	public CProcess
{
public:
	CGameProcess::CGameProcess()
		: m_Player(NULL), 
		  #ifdef _DEBUG
		  m_ProvisionalCamera(NULL), 
		  #endif
		  m_fPlayerLife(0.0f), 
		  m_fPlayerEnergy(0.0f), 
		  m_fTimeHit(0.0f), 
		  m_bReplay(false),
		  m_bFinalEnemies(false),
		  m_bPlayingActionMusic(false)
	{
	}
	~CGameProcess() {}

	// Process
	void PreInit();
	bool Init();
	void Release();
	void Update(float ElapsedTime);
	void RenderScene(CRenderManager* rendermanager);
	void RenderDebug(CRenderManager* rendermanager);

	void Load();
	void Reload();
	void ReloadPlayerWeapon();
	void RegisterLUA();

	// Player
	void SetPlayer();
	CPlayer* GetPlayer() { return m_Player; }
	void ChangePlayerAnim(int anim, float delay);
	bool UseItem();
	void SetPlayerCanUse(bool use);
	int GetNumPiecesFound();
	bool IsInRealWorld();

	// Setup
	void SetMouseSensitivity(float sensitivity);
	float GetMouseSensitivity(); 
	void SetGammaFactor(float gamma);

	// Triggers
	void MirrorTrigger(const std::string& action, const std::string& mirrorName, const std::string& targetMirror, const std::string& rotatedMirror);
	void TutorialTrigger(const std::string& action, const std::string& Name);
	void DoorTrigger(const std::string& action, const std::string& Name);
	void ElevatorTrigger(const std::string& action, const std::string& Name);
	void LasersTrigger(const std::string& action, const std::string& Name);
	void LifeTrigger(const std::string& action, const std::string& Name);
	void EnergyTrigger(const std::string& action, const std::string& Name);
	void MirrorPieceTrigger(const std::string& action, const std::string& Name);
	void BrokenMirrorTrigger(const std::string& action, const std::string& Name);

private:	
	CCamera* GetCamera();
	
	void UpdateInputActions();
	void UpdateGameActions();
	void InitGUIData();
	void UpdateGUI(float ElapsedTime);
	void SetGamma(CRenderManager &RM, float GammaFactor);

#ifdef _DEBUG	
	void ChangeCamera();
#endif

private:
	CPlayer* m_Player;
	float m_fPlayerLife;
	float m_fPlayerEnergy;
	float m_fTimeHit;
	WORD ramp[256*3];		// gamma correction
	
	std::vector<std::string> m_sMirrorPieces;
	std::vector<std::string> m_sBloodText;

	bool m_bReplay, m_bFinalEnemies, m_bPlayingActionMusic;

#ifdef _DEBUG	
	CCamera * m_ProvisionalCamera;
#endif

};

#endif
