#pragma once
#include "Camera\FPSCamera.h"
#include "Player.h"

#define SLOW_FREE_CAMERA 3
#define	FAST_FREE_CAMERA 6



class CFreeCamera :
	public CFPSCamera
{
public:
	CFreeCamera(void);
	CFreeCamera(float zn, float zf, float fov, float aspect, Vect3f& Position);
	~CFreeCamera(void);


	void Update(float ElapsedTime);


private:
	float m_fDeltaPitch, m_fDeltaYaw;
	float m_fSpeed;
	tPlayerStates m_CameraMovement;
	bool m_bIsRunning;
	

	void UpdateInputActions();
	void UpdateCamera(float ElapsedTime);
	void UpdatePlayerPosition(float ElapsedTime);
	void UpdateCameraSpeed();
	

};

