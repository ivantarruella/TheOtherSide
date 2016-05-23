#include "FreeCamera.h"
#include "ActionToInput.h"
#include "Character.h"
#include "Base.h"

#define FREE_CAM_SPEED	0.010f


CFreeCamera::CFreeCamera(void)
{
}

CFreeCamera::CFreeCamera(float zn, float zf, float fov, float aspect, Vect3f& Position):
CFPSCamera (zn, zf,fov, aspect,new CObject3D(Position, Vect3f(1.0f),0.0f,0.0f)),	
	m_fDeltaPitch(0.0f), m_fDeltaYaw(0.0f),	m_fSpeed(SLOW_FREE_CAMERA),m_CameraMovement(IDDLE),m_bIsRunning(false)
{
	

}

CFreeCamera::~CFreeCamera(void)
{
	CHECKED_DELETE(m_pObject3D);
}


void CFreeCamera::Update(float ElapsedTime){
	
	UpdateInputActions();

	UpdateCamera(ElapsedTime);
	UpdateCameraSpeed();
	UpdatePlayerPosition(ElapsedTime);

}



void CFreeCamera::UpdateInputActions()
{
	CActionToInput* actionToInput = CORE->GetActionToInput();

	// Mouse
	actionToInput->DoAction(DOACTION_PLAYERLOOKLEFT, m_fDeltaYaw); 
	actionToInput->DoAction(DOACTION_PLAYERLOOKRIGHT, m_fDeltaYaw);
	actionToInput->DoAction(DOACTION_PLAYERLOOKUP, m_fDeltaPitch); 
	actionToInput->DoAction(DOACTION_PLAYERLOOKDOWN, m_fDeltaPitch);

	

	// Keyboard
	m_CameraMovement = IDDLE;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVEUP) || actionToInput->DoAction(DOACTION_PLAYERMOVEUP_PAD))
		m_CameraMovement = WALK_FORWARD;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN) || actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN_PAD))
		m_CameraMovement = WALK_BACKWARDS;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVELEFT) || actionToInput->DoAction(DOACTION_PLAYERMOVELEFT_PAD))
		m_CameraMovement = WALK_LEFT;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT) || actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT_PAD))
		m_CameraMovement = WALK_RIGHT;

	if (actionToInput->DoAction(DOACTION_PLAYERMOVEUP) && actionToInput->DoAction(DOACTION_PLAYERMOVELEFT))
		m_CameraMovement = WALK_FORWARD_LEFT;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVEUP) && actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT))
		m_CameraMovement = WALK_FORWARD_RIGHT;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN) && actionToInput->DoAction(DOACTION_PLAYERMOVELEFT))
		m_CameraMovement = WALK_BACKWARDS_LEFT;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN) && actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT))
		m_CameraMovement = WALK_BACKWARDS_RIGHT;

	m_bIsRunning = (actionToInput->DoAction(DOACTION_PLAYERRUN) || actionToInput->DoAction(DOACTION_PLAYERRUN_PAD)); 
}

void CFreeCamera::UpdateCamera(float ElapsedTime)
{
	
	// Update mouse camera
	if (m_fDeltaYaw)
	{

		float l_fYaw = m_pObject3D->GetYaw() - (m_fDeltaYaw*FREE_CAM_SPEED);
		m_pObject3D->SetYaw(l_fYaw);

	}

	if (m_fDeltaPitch)
	{
		float l_fPitch =  m_pObject3D->GetPitch() - (m_fDeltaPitch*FREE_CAM_SPEED);
		if(l_fPitch < -FLOAT_PI_VALUE/3) l_fPitch = -FLOAT_PI_VALUE/3;			// ponemos tope a la camara
		else if(l_fPitch > FLOAT_PI_VALUE/3) l_fPitch = FLOAT_PI_VALUE/3;

		m_pObject3D->SetPitch(l_fPitch);
	}

}

void CFreeCamera::UpdateCameraSpeed()
{
	if (m_bIsRunning)
		m_fSpeed=FAST_FREE_CAMERA;
	else
		m_fSpeed=SLOW_FREE_CAMERA;

}

void CFreeCamera::UpdatePlayerPosition(float ElapsedTime)
{
	float l_fYaw =m_pObject3D->GetYaw();
	float l_fPitch =  m_pObject3D->GetPitch();
	Vect3f l_Position = m_pObject3D->GetPosition();
	Vect3f l_vDir(0.0f);
	Vect3f l_vUpDown, l_vLeftRight, l_vDir1, l_vDir2;
	
	l_vUpDown(cos(l_fYaw)*cos(l_fPitch), sin(l_fPitch), sin(l_fYaw)*cos(l_fPitch) );	// up & down
	l_vLeftRight(cos(l_fYaw+ePIf/2)*cos(l_fPitch), 0.0f, sin(l_fYaw+ePIf/2)*cos(l_fPitch));	// left & right
	l_vDir1(cos(l_fYaw-ePIf/4)*cos(l_fPitch), sin(l_fPitch), sin(l_fYaw-ePIf/4)*cos(l_fPitch));		// forward right & backward left
	l_vDir2(cos(l_fYaw+ePIf/4)*cos(l_fPitch), sin(l_fPitch), sin(l_fYaw+ePIf/4)*cos(l_fPitch));		// backward right & forward left


	switch (m_CameraMovement) {
	case IDDLE:
		break;

	case WALK_FORWARD:
		l_vDir=l_vUpDown;
		break;

	case WALK_BACKWARDS:
		l_vDir=-l_vUpDown;
		break;

	case WALK_LEFT:
		l_vDir=l_vLeftRight;
		break;

	case WALK_RIGHT:
		l_vDir=-l_vLeftRight;
		break;

	case WALK_FORWARD_LEFT:
		l_vDir=l_vDir2;
		break;

	case WALK_FORWARD_RIGHT:
		l_vDir=l_vDir1;
		break;

	case WALK_BACKWARDS_LEFT:
		l_vDir=-l_vDir1;
		break;

	case WALK_BACKWARDS_RIGHT:
		l_vDir=-l_vDir2;
		break;
	}

	//Yaw del objeto al que sigue la camara
	m_pObject3D->SetYaw(l_fYaw);
	
	//Position
	l_Position = l_Position + l_vDir * ElapsedTime * m_fSpeed;
	m_pObject3D->SetPosition(l_Position);
}


