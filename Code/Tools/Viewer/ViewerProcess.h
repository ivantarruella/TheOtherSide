#ifndef __CLASS_VIEWERPROCESS_H__
#define __CLASS_VIEWERPROCESS_H__

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

#define GUI_LIVE_WIDTH			24.0f
#define GUI_ENERGY_WIDTH		24.0f

#define MAX_DAMAGE_TIME			3.0f

#define MOUSE_SENSITIVITY		0.010f		// velocidad actualización ratón por defecto
#define MOUSE_SENSITIVITY_MIN	0.001f
#define MOUSE_SENSITIVITY_MAX	0.02f

#define CAMERA_INI_POS			(Vect3f(22.f, 0.7f, -2.f))


class CViewerProcess :	public CProcess
{
public:
	CViewerProcess();
	~CViewerProcess();

	// Process
	void PreInit();
	bool Init();
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
	bool ButtonUsePressed();
	void SetPlayerCanUse(bool use);

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
	void ChangeCamera();
	void Release();
	void UpdateInputActions();
	void UpdateGUI(float ElapsedTime);
	void SetGamma(CRenderManager &RM, float GammaFactor);

private:
	CPlayer* m_Player;
	CCamera * m_ProvisionalCamera;
	float m_fPlayerLife;
	float m_fPlayerEnergy;
	float m_fTimeHit;
	WORD ramp[256*3];		// gamma correction
	
	std::vector<std::string> m_sMirrorPieces;
	std::vector<std::string> m_sBloodText;

	bool m_bReplay;
};

#endif
