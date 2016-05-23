#ifndef __CLASS_VIEWERPROCESS_H__
#define __CLASS_VIEWERPROCESS_H__

#pragma once

#include "Process.h"
//#include "Vector2.h"

class CObject3D;
class CPlayer;
class CSpotLight;

enum {
	ALL_WORLDS,
	REAL_WORLD,
	MIRROR_WORLD
};

#define TMIN_WORLD_CHANGE		2.0f

class CTestPhysXProcess :	public CProcess
{
public:
	CTestPhysXProcess(void)
		: m_Player(NULL),m_Linterna(NULL),m_Laser(NULL), m_bWorld(ALL_WORLDS),m_fChangeTime(0.f)
	{
	}
	~CTestPhysXProcess(void) { Release(); }

	bool Init();
	void Update(float ElapsedTime);
	void RenderScene(CRenderManager* rendermanager);
	void RenderDebug(CRenderManager* rendermanager);
	void SetCamera();
	void Reload();
	void RegisterLUA();

	void SetPlayer();
	CObject3D* GetPlayer();

	void UpdatePlayerWeapon();
	void ChangeWorldActions();
	void ChangeWorld(const std::string& pointName);

private:	
	CCamera* GetCamera();
	void Release();
	void UpdateInputActions();

private:
	CPlayer* m_Player;
	CSpotLight* m_Linterna;		// prueba de linterna
	CSpotLight* m_Laser;

	float m_fChangeTime;
	bool m_bWorld;
};

#endif
