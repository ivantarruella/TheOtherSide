#include "Player.h"
#include "Scripting\ScriptManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "GamePad.h"
#include "LightManager.h"
#include "RenderManager.h"
#include "AnimatedModelsManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Camera\Camera.h"
#include "Camera\ThirdPersonCamera.h"
#include "Camera\ThPSCamera.h"
#include "include\PhysicController.h"
#include "LevelManager.h"
#include "LogicObjectsManager.h"
#include "EnemyManager.h"
#include "Character.h"
#include "luabind\luabind.hpp"
#include "Weapon.h"
#include "Base.h"


using namespace luabind;

#define MOUSE_SPEED				0.010f		// velocidad actualización ratón por defecto inicial 
#define MOUSE_SPEED_AIM			0.006f		// velocidad actualización ratón apuntando por defecto inicial 

#define GAMEPAD_SPEED			0.00033f	// velocidad actualización gamepad
#define GAMEPAD_SPEED_AIM		0.00022f	// velocidad actualización gamepad apuntando

#define PLAYER_SPEED_WALK		0.0275f		// velocidad movimiento personaje andando
#define PLAYER_SPEED_WALK_BCK	0.025f		// velocidad movimiento personaje andando atrás
#define PLAYER_SPEED_WALK_AIM	0.0275f		// velocidad movimiento personaje andando y apuntando
#define PLAYER_SPEED_WALK_AIM_BCK	0.025f	// velocidad movimiento personaje andando hacia atrás y apuntando
#define PLAYER_SPEED_RUN		0.055f		// velocidad movimiento personaje corriendo

#define PLAYER_UPDATE_CAM_TIME	0.01f		// velocidad update camara al apuntar
#define CAM_ZOOM_AIM_STEP		0.05f

#define ANIMS_DELAY				0.3f			// Blending delay entre animaciones

#define MAX_LIFE_PLAYER			2.50f		// Vida inicial
#define LIFE_THRESHOLD			0.02f		

#define CAM_PITCH_DOWN			(FLOAT_PI_VALUE/9.0f)
#define CAM_PITCH_UP			(FLOAT_PI_VALUE/9.0f)

#define PLAYER_HIT_TIME			0.2f
#define SHOOT_SOUND_TIME		0.25f	// = POWERGUN

#define PLAYER_BBOX_HEIGHT		0.65f
#define PLAYER_BBOX_SIZE		0.25f

CPlayer::CPlayer()
	: m_Camera(NULL), m_fDeltaPitch(0.f), m_fDeltaYaw(0.f), m_fSpeed(0.f), 
	m_PlayerMovement(IDDLE), m_lastAnim(WAIT_ANIM), m_bIsAiming(false), m_bWasAiming(false), m_bIsRunning(false), m_bShoot(false), m_bUseAnim(false), m_bCanUse(false), m_fUseAnimTime(0.0f),
	m_bUsePressed(false), m_fZoom(CAM_ZOOM_WALK), m_fDistance(CAM_DIST), m_fDistanceFromCenter(PLAYER_DISTANCE_FROM_CENTER), m_bUseItem(false), CCharacter(), m_vPosIni(Vect3f(0.0f, 0.0f, 0.0f)), m_fMouseSpeed(MOUSE_SPEED),
	m_iNumMirrorPieces(0), m_bPlayerDying(false), m_bIsDead(false), m_fTime(0.0f), m_fUpdateCam(0.0f), m_tUseAnim(USE_ANIM), m_fHitTime(0.0f), m_bBlood(false), m_fStepTime(0.0f),
	m_fShotTime(SHOOT_SOUND_TIME), mf_Yaw(0.0f)
{
	InitPlayer();
}

CPlayer::CPlayer(CXMLTreeNode XMLData) : m_Camera(NULL), m_fDeltaPitch(0.f), m_fDeltaYaw(0.f), m_fSpeed(0.f),  
	m_PlayerMovement(IDDLE), m_lastAnim(WAIT_ANIM), m_bIsAiming(false), m_bWasAiming(false), m_bIsRunning(false), m_bShoot(false), m_bUseAnim(false), m_bCanUse(false), m_fUseAnimTime(0.0f),
	m_bUsePressed(false), m_fZoom(CAM_ZOOM_WALK), m_fDistance(CAM_DIST), m_fDistanceFromCenter(PLAYER_DISTANCE_FROM_CENTER), m_bUseItem(false), CCharacter(XMLData, PLAYER_BBOX_SIZE,PLAYER_BBOX_HEIGHT), m_vPosIni(Vect3f(0.0f, 0.0f, 0.0f)), m_fMouseSpeed(MOUSE_SPEED),
	m_iNumMirrorPieces(0), m_bPlayerDying(false), m_bIsDead(false), m_fTime(0.0f), m_fUpdateCam(0.0f), m_tUseAnim(USE_ANIM), m_fHitTime(0.0f), m_bBlood(false), m_fStepTime(0.0f),
	m_fShotTime(SHOOT_SOUND_TIME), mf_Yaw(0.0f)
{
	InitPlayer();
}

CPlayer::~CPlayer()
{
	if(GetCreatePhysics())
	{
		CORE->GetPhysicsManager()->ReleasePhysicActor(GetPhysicActor());
		CORE->GetPhysicsManager()->ReleasePhysicController(GetPhysicController());
	}
}

void CPlayer::InitPlayer()
{
	SetLife(MAX_LIFE_PLAYER);
	SetIniPos(GetPosition());
	m_oWeapon = CWeapon();
}

void CPlayer::Update(float ElapsedTime)
{
	m_Camera = CORE->GetCamera();
	if (m_Camera == NULL || m_Camera->GetTypeCamera()==TC_FPS)
		return;
	if (!CORE->GetLevelManager()->IsOk())
		return;
	if (CORE->GetLogicObjectsManager()->GetTutorialActive())
	{
		ChangeCharacterAnimation(WAIT_ANIM, ANIMS_DELAY);
		CCharacter::Update(ElapsedTime/1000.f);
		return;
	}

	UpdateInputActions();
	UpdateCamera(ElapsedTime);
	UpdatePlayerWeapon(ElapsedTime);
	UpdatePlayerOrientation();
	UpdatePlayerSpeed();
	UpdatePlayerPosition(ElapsedTime);
	UpdatePlayerAnimations(ElapsedTime); 
	UpdatePlayerHit(ElapsedTime);
}



void CPlayer::UpdateInputActions()
{
	CActionToInput* actionToInput = CORE->GetActionToInput();

	actionToInput->DoAction(DOACTION_PLAYERMOVEUPDOWN_DPAD, m_fUpDown);
	actionToInput->DoAction(DOACTION_PLAYERMOVELEFTRIGHT_DPAD, m_fLeftRight);

	// Mouse camera
	actionToInput->DoAction(DOACTION_PLAYERLOOKLEFT, m_fDeltaYaw); 
	actionToInput->DoAction(DOACTION_PLAYERLOOKRIGHT, m_fDeltaYaw);
	actionToInput->DoAction(DOACTION_PLAYERLOOKUP, m_fDeltaPitch); 
	actionToInput->DoAction(DOACTION_PLAYERLOOKDOWN, m_fDeltaPitch);

	// GamePad camera
	bool bGamePad = CORE->GetInputManager()->HasGamePad();
	if (bGamePad) {
		float fDeltaYaw = 0.f, fDeltaPitch = 0.f;
		actionToInput->DoAction(DOACTION_PLAYERLOOKLEFT_PAD, fDeltaYaw); 
		actionToInput->DoAction(DOACTION_PLAYERLOOKRIGHT_PAD, fDeltaYaw);
		actionToInput->DoAction(DOACTION_PLAYERLOOKUP_PAD, fDeltaPitch); 
		actionToInput->DoAction(DOACTION_PLAYERLOOKDOWN_PAD, fDeltaPitch);
		float delta = (m_bIsAiming ? GAMEPAD_SPEED_AIM : GAMEPAD_SPEED);
		m_fDeltaYaw += (fDeltaYaw * delta);
		m_fDeltaPitch -= (fDeltaPitch * delta);
	}

	// Player movement (Keyboard & Pad)
	m_PlayerMovement = IDDLE;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVEUP) || (bGamePad&&(actionToInput->DoAction(DOACTION_PLAYERMOVEUP_PAD) || m_fUpDown > INPUT_DEADZONE)))
		m_PlayerMovement = WALK_FORWARD;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN) || (bGamePad&&(actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN_PAD) || m_fUpDown < -INPUT_DEADZONE)))
		m_PlayerMovement = WALK_BACKWARDS;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVELEFT) || (bGamePad&&(actionToInput->DoAction(DOACTION_PLAYERMOVELEFT_PAD) || m_fLeftRight < -INPUT_DEADZONE)))
		m_PlayerMovement = WALK_LEFT;
	if (actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT) || (bGamePad&&(actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT_PAD) || m_fLeftRight > INPUT_DEADZONE)))
		m_PlayerMovement = WALK_RIGHT;
	if ((actionToInput->DoAction(DOACTION_PLAYERMOVEUP) && actionToInput->DoAction(DOACTION_PLAYERMOVELEFT)) || 
		(bGamePad&&((actionToInput->DoAction(DOACTION_PLAYERMOVEUP_PAD) && actionToInput->DoAction(DOACTION_PLAYERMOVELEFT_PAD))
		|| (m_fUpDown > INPUT_DEADZONE && m_fLeftRight < -INPUT_DEADZONE))))
		m_PlayerMovement = WALK_FORWARD_LEFT;
	if ((actionToInput->DoAction(DOACTION_PLAYERMOVEUP) && actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT)) ||
		(bGamePad&&((actionToInput->DoAction(DOACTION_PLAYERMOVEUP_PAD) && actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT_PAD))
		|| (m_fUpDown > INPUT_DEADZONE && m_fLeftRight > INPUT_DEADZONE))))
		m_PlayerMovement = WALK_FORWARD_RIGHT;
	if ((actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN) && actionToInput->DoAction(DOACTION_PLAYERMOVELEFT)) ||
		(bGamePad&&((actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN_PAD) && actionToInput->DoAction(DOACTION_PLAYERMOVELEFT_PAD))
		|| (m_fUpDown < -INPUT_DEADZONE && m_fLeftRight < -INPUT_DEADZONE))))
		m_PlayerMovement = WALK_BACKWARDS_LEFT;
	if ((actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN) && actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT)) ||
		(bGamePad&&((actionToInput->DoAction(DOACTION_PLAYERMOVEDOWN_PAD) && actionToInput->DoAction(DOACTION_PLAYERMOVERIGHT_PAD))
		|| (m_fUpDown < -INPUT_DEADZONE && m_fLeftRight > INPUT_DEADZONE))))
		m_PlayerMovement = WALK_BACKWARDS_RIGHT;

	// Aim button (keyboard & gamepad)
	/*
	if (CORE->GetActionToInput()->DoAction(DOACTION_PLAYERAIM) || (bGamePad&&CORE->GetActionToInput()->DoAction(DOACTION_PLAYERAIM_PAD))) 
	{
		if(!m_bWasAiming && !m_oWeapon.AimingWeaponCollision())
			m_bIsAiming = !m_bIsAiming;	
	
		if(m_bWasAiming)
			m_bWasAiming = false;
	}
	else
	{
		m_bIsAiming = false;
		m_bWasAiming = false;
	}

	if(m_bIsAiming && m_oWeapon.AimingWeaponCollision()) {
		m_bIsAiming = false;	
		m_bWasAiming = true;
	}

	if(m_bWasAiming && !m_oWeapon.WasAimingWeaponCollision()) {
		m_bIsAiming = true;	
		m_bWasAiming = false;
	}
	*/
	m_bWasAiming = false;
	if ((CORE->GetActionToInput()->DoAction(DOACTION_PLAYERAIM) || (bGamePad&&CORE->GetActionToInput()->DoAction(DOACTION_PLAYERAIM_PAD))) && !m_oWeapon.AimingWeaponCollision())
	{
		m_bIsAiming = true;
		m_bIsRunning = false;
	}
	else
	{
		m_bIsAiming = false;
	}
	

	// Shoot, run & use buttons (keyboard & gamepad)
	m_bShoot = actionToInput->DoAction(DOACTION_PLAYERSHOOT) || (bGamePad&&actionToInput->DoAction(DOACTION_PLAYERSHOOT_PAD));
	if (!m_bIsRunning && !m_bIsAiming)
	{
		m_bIsRunning = (actionToInput->DoAction(DOACTION_PLAYERRUN) || (bGamePad&&actionToInput->DoAction(DOACTION_PLAYERRUN_PAD))) && !m_bIsAiming 
			&& (m_PlayerMovement == WALK_FORWARD || m_PlayerMovement == WALK_FORWARD_LEFT || m_PlayerMovement == WALK_FORWARD_RIGHT); 
	}
	m_bUsePressed = actionToInput->DoAction(DOACTION_PLAYERUSEITEM) || (bGamePad&&actionToInput->DoAction(DOACTION_PLAYERUSEITEM_PAD));

}

void CPlayer::UpdateCamera(float ElapsedTime)
{
	m_fUpdateCam += ElapsedTime;

	if (IsDead())
		return;

	// Update mouse camera
	if (!m_bIsAiming) 
		m_fMouseSpeed = MOUSE_SPEED;
	else
		m_fMouseSpeed = MOUSE_SPEED_AIM;

	if (m_fDeltaYaw)
	{
		float l_fYaw = m_Camera->GetPlayerRoll() - (m_fDeltaYaw*m_fMouseSpeed);
		m_Camera->SetPlayerRoll(l_fYaw);
	}

	if (m_fDeltaPitch)
	{
		float l_fPitch =  m_Camera->GetPlayerPitch() - (m_fDeltaPitch*m_fMouseSpeed);
		if(l_fPitch < -(float)CAM_PITCH_DOWN) l_fPitch = -(float)CAM_PITCH_DOWN;			// ponemos tope a la camara
		else if(l_fPitch > (float)CAM_PITCH_UP) l_fPitch = (float)CAM_PITCH_UP;

		m_Camera->SetPlayerPitch(l_fPitch);
	}

	// Update player zoom camera
	if (m_fUpdateCam >= (float)PLAYER_UPDATE_CAM_TIME)
	{
		if (m_bIsAiming)
		{
			if (m_fDistance > (float)CAM_DIST_AIM)
				m_fDistance -= (float)CAM_ZOOM_AIM_STEP;
			if (m_fDistanceFromCenter < (float)CAM_DIST_AIM) 
				m_fDistanceFromCenter += (float)CAM_ZOOM_AIM_STEP;
			((CThirdPersonCamera*)m_Camera)->SetFrontDistance(m_fDistance);
			((CThirdPersonCamera*)m_Camera)->SetDistanceFromCenter(m_fDistanceFromCenter);
		}
		else
		{
			if (m_fDistance < (float)CAM_DIST)
				m_fDistance += (float)CAM_ZOOM_AIM_STEP;
			if (m_fDistanceFromCenter > (float)PLAYER_DISTANCE_FROM_CENTER) 
				m_fDistanceFromCenter -= (float)CAM_ZOOM_AIM_STEP;
			((CThirdPersonCamera*)m_Camera)->SetFrontDistance(m_fDistance);
			((CThirdPersonCamera*)m_Camera)->SetDistanceFromCenter(m_fDistanceFromCenter);
		}

		m_fUpdateCam = 0.f;
	}
	
	m_Camera->Update(ElapsedTime);
}

void CPlayer::UpdatePlayerSpeed()
{
	if (m_PlayerMovement == IDDLE)
		return;

	if (m_bIsRunning)
		SetPlayerSpeed(PLAYER_SPEED_RUN);
	else if (m_bIsAiming) {
		if (m_PlayerMovement == WALK_BACKWARDS || m_PlayerMovement == WALK_BACKWARDS_LEFT || m_PlayerMovement == WALK_BACKWARDS_RIGHT)
			SetPlayerSpeed(PLAYER_SPEED_WALK_AIM_BCK);
		else
			SetPlayerSpeed(PLAYER_SPEED_WALK_AIM);
	}
	else {
		if (m_PlayerMovement == WALK_BACKWARDS || m_PlayerMovement == WALK_BACKWARDS_LEFT || m_PlayerMovement == WALK_BACKWARDS_RIGHT)
			SetPlayerSpeed(PLAYER_SPEED_WALK_BCK);
		else
			SetPlayerSpeed(PLAYER_SPEED_WALK);
	}
}

void CPlayer::UpdatePlayerPosition(float ElapsedTime)
{
	if (m_bPlayerDying || m_bIsDead)
		return;

	// Movimiento player
	// Todo lo que afecta al player afecta tambien al controller
	float l_fYaw =m_Camera->GetPlayerRoll();
	float l_fPitch =  m_Camera->GetPlayerPitch();
	Vect3f l_Position = GetPosition();
	Vect3f l_vDirController(0.0f);

	Vect3f l_vUpDown, l_vLeftRight, l_vDir1, l_vDir2;

	l_vUpDown(cos(l_fYaw), 0.0f, sin(l_fYaw));					// up & down
	l_vLeftRight(cos(l_fYaw+ePIf/2), 0.0f, sin(l_fYaw+ePIf/2));	// left & right
	l_vDir1(cos(l_fYaw-ePIf/4), 0.0f, sin(l_fYaw-ePIf/4));		// forward right & backward left
	l_vDir2(cos(l_fYaw+ePIf/4), 0.0f, sin(l_fYaw+ePIf/4));		// backward right & forward left

	switch (m_PlayerMovement) {
	case IDDLE:
		m_bIsRunning = false;
		break;

	case WALK_FORWARD:
		l_vDirController=l_vUpDown;
		break;

	case WALK_BACKWARDS:
		m_bIsRunning = false;
		l_vDirController=-l_vUpDown;
		break;

	case WALK_LEFT:
		l_fYaw += 0.02f;

		l_vDirController=l_vLeftRight;
		break;

	case WALK_RIGHT:
		l_fYaw -= 0.02f;

		l_vDirController=-l_vLeftRight;
		break;

	case WALK_FORWARD_LEFT:
		l_fYaw += 0.01f;

		l_vDirController=l_vDir2;
		break;

	case WALK_FORWARD_RIGHT:
		l_fYaw -= 0.01f;

		l_vDirController=l_vDir1;
		break;

	case WALK_BACKWARDS_LEFT:
		l_fYaw -= 0.01f;
		m_bIsRunning = false;
		l_vDirController=-l_vDir1;
		break;

	case WALK_BACKWARDS_RIGHT:
		l_fYaw += 0.01f;
		m_bIsRunning = false;
		l_vDirController=-l_vDir2;
		break;
	}
	
	m_Camera->SetPlayerYaw(l_fYaw);
	if  (!m_oWeapon.WeaponCollision() || 
		(m_bWasAiming && m_PlayerMovement!=WALK_FORWARD && m_PlayerMovement!=WALK_FORWARD_LEFT && m_PlayerMovement!=WALK_FORWARD_RIGHT)	)
		SetPosition(l_vDirController*GetPlayerSpeed(),ElapsedTime);
}


//En el character modificar les funcions perq afecten tmb al controller
void CPlayer::UpdatePlayerOrientation()
{
	if (m_bPlayerDying || m_bIsDead || m_bUsePressed)
		return;

	float l_fYaw = m_Camera->GetPlayerYaw();
	if (m_bIsAiming || m_bWasAiming)
	{
		SetYaw(-FLOAT_PI_VALUE/2+l_fYaw);
	}
	else
	{
		switch (m_PlayerMovement) 
		{
		case IDDLE:
			/* Move player while moving camera:
			SetYaw(-FLOAT_PI_VALUE/2+l_fYaw);
			if (l_fYaw != mf_Yaw && m_PlayerMovement == IDDLE && !m_bIsAiming) {
				ChangeCharacterAnimation(m_bIsRunning ? RUN_ANIM : WALK_ANIM, ANIMS_DELAY);
				mf_Yaw = l_fYaw;
			}*/			
			break;

		case WALK_FORWARD:
		case WALK_BACKWARDS:
			SetYaw(-FLOAT_PI_VALUE/2+l_fYaw);
			break;

		case WALK_LEFT:
			SetYaw(l_fYaw);
			break;

		case WALK_RIGHT:
			SetYaw(FLOAT_PI_VALUE+l_fYaw);
			break;

		case WALK_FORWARD_LEFT:
		case WALK_BACKWARDS_RIGHT:	
			SetYaw(-FLOAT_PI_VALUE/4+l_fYaw);

			break;

		case WALK_FORWARD_RIGHT:
		case WALK_BACKWARDS_LEFT:
			SetYaw(5*(FLOAT_PI_VALUE/4)+l_fYaw);

			break;
		}
	}
}


void CPlayer::UpdatePlayerAnimations(float ElapsedTime)
{
	CCharacter::Update(ElapsedTime);
	
	if (!CCharacter::IsDead())
	{
		UpdatePlayerMoveAnim(ElapsedTime);
		UpdatePlayerShootAnim(ElapsedTime);
		UpdatePlayerUseAnim(ElapsedTime);
	}
	else
		UpdatePlayerDeadAnim(ElapsedTime);
}

void CPlayer::UpdatePlayerDeadAnim(float ElapsedTime)
{
	// Dead
	if (!m_bPlayerDying)
	{
		CORE->GetScriptManager()->RunCode("sound_player_dying()");
		CORE->GetCamera()->GetObject3D()->SetPitch(-0.5f);		// camara mirando al suelo
		CCharacter::ClearAllAnims();
		ExecuteAction(DYE_ANIM, 0.f, 0.f);
		ClearCycle(WAIT_ANIM, 0.f);
		ChangeCharacterAnimation(DEAD_ANIM, 0.f);
		m_bPlayerDying = true;
		m_bIsDead = false;
		m_fTime = GetAnimationDuration() * 1.5f;
	}
	else
	{
		m_fTime -= ElapsedTime;
		
		if (m_fTime <= 0.0f)
		{
			RemoveAction(DYE_ANIM);
			m_bIsDead = true;
		}
	}
}

void CPlayer::UpdatePlayerMoveAnim(float ElapsedTime)
{
	if (m_bIsAiming || m_tUseAnim==NUM_ANIMS)
		return;

	m_fStepTime += ElapsedTime;

	// Movement
	switch (m_PlayerMovement) {
	case IDDLE:
		ChangeCharacterAnimation(WAIT_ANIM, ANIMS_DELAY);
		break;

	case WALK_FORWARD:
	case WALK_BACKWARDS:
	case WALK_LEFT:
	case WALK_RIGHT:
	case WALK_FORWARD_LEFT:
	case WALK_FORWARD_RIGHT:
	case WALK_BACKWARDS_LEFT:
	case WALK_BACKWARDS_RIGHT:
		
		float t = m_bIsRunning ? 0.33f : 0.66f;
		if (m_fStepTime >= t)
		{
			CORE->GetScriptManager()->RunCode(isInRealWorld() ? "sound_andar_MR()" : "sound_andar_ME()");
			m_fStepTime = 0.f;
		}

		ChangeCharacterAnimation(m_bIsRunning ? RUN_ANIM : WALK_ANIM, ANIMS_DELAY);
		break;
	}
}

void CPlayer::UpdatePlayerShootAnim(float ElapsedTime)
{
	if (m_bIsDead)
		return;

	if (m_bIsAiming)
	{
		float l_pitch = m_Camera->GetPlayerPitch();
		ChangeCharacterAnimation((m_PlayerMovement == IDDLE ? AIM_ANIM : WALK_AIM_ANIM), ANIMS_DELAY);

		// Aim down
		if (l_pitch <= 0.0f)
		{
			ClearCycle((m_PlayerMovement == IDDLE ? AIM_UP_ANIM : WALK_AIM_UP_ANIM), 0.f);
			float weight = -l_pitch * (2.5f - l_pitch);
			BlendCycle(AIM_DOWN_ANIM, 0.f, weight);	
		}
		
		// Aim up
		if (l_pitch > 0.0f)
		{
			ClearCycle((m_PlayerMovement == IDDLE ? AIM_DOWN_ANIM : WALK_AIM_DOWN_ANIM), 0.f);
			float weight = l_pitch * (2.75f + l_pitch);
			BlendCycle(AIM_UP_ANIM, 0.f, weight);	
		}
	}
	else
	{
		ClearCycle(AIM_ANIM, ANIMS_DELAY);
		ClearCycle(AIM_UP_ANIM, ANIMS_DELAY);
		ClearCycle(AIM_DOWN_ANIM, ANIMS_DELAY);
		ClearCycle(WALK_AIM_ANIM, ANIMS_DELAY);
		ClearCycle(WALK_AIM_UP_ANIM, ANIMS_DELAY);
		ClearCycle(WALK_AIM_DOWN_ANIM, ANIMS_DELAY);
	}
}

void CPlayer::UpdatePlayerUseAnim(float ElapsedTime)
{
	// Use
	if (m_bCanUse && m_tUseAnim!=NUM_ANIMS)				// dentro de un trigger
	{
		if (m_bUsePressed)		// pulsador USAR activo
		{
			if (!m_bUseAnim)
			{
				ExecuteAction(m_tUseAnim, ANIMS_DELAY, ANIMS_DELAY);
				m_fUseAnimTime = GetAnimationDuration() * 0.5f;
				m_bUseAnim = true;
			}
			else
			{
				m_fUseAnimTime -= ElapsedTime;
				if (m_fUseAnimTime <= 0.0f)
					m_bUseItem = true;
			}
		}
		else
		{
			if (m_bUseAnim)
			{
				RemoveAction(m_tUseAnim);
				m_bUseAnim = false;
				m_bUseItem = false;
			}
		}
	}
	else
		m_bUseItem = false;
}

void CPlayer::UpdatePlayerWeapon(float ElapsedTime)
{
	m_oWeapon.SetPlayer(this);
	if(m_bShoot)
	{
		m_fShootKeyPressedTime+=ElapsedTime;
	}
	else
	{
		m_fShootKeyPressedTime=0.0f;
	}
	m_oWeapon.Update(ElapsedTime, this);

	// gestión sonido arma
	if (m_bIsAiming && m_bShoot && !m_oWeapon.IsMunitionEmpty())
	{
		if (isInRealWorld())
		{
			m_fShotTime += ElapsedTime;
			if (m_fShotTime>=SHOOT_SOUND_TIME)
			{
				CORE->GetScriptManager()->RunCode("sound_shot()");
				m_fShotTime = 0.f;
			}
		}
		else 
			CORE->GetScriptManager()->RunCode("sound_laser_on()");
	}
	else
	{
		if (!isInRealWorld() && !m_bShoot)
			CORE->GetScriptManager()->RunCode("sound_laser_off()");

		m_fShotTime = SHOOT_SOUND_TIME;
	}
}


void CPlayer::UpdatePlayerHit(float ElapsedTime)
{
	if (!IsDead())
	{
		if (CCharacter::GetIsHit() && !m_bBlood)
		{
			m_fHitTime = PLAYER_HIT_TIME;
			m_bBlood = true;
			CORE->GetScriptManager()->RunCode("sound_player_hit()");
		}

		if (m_bBlood)
		{
			m_fHitTime -= ElapsedTime;
			if (m_fHitTime <= 0.f)
			{
				CCharacter::SetIsHit(false);
				m_bBlood = false;
			}
		}
	}
}

// Teletransporta player
void CPlayer::SetPosition(const Vect3f &Position)
{
	CCharacter::SetPosition(Position);
	if(m_Camera)
		m_Camera->GetObject3D()->SetPosition(Position);
}

// Mueve player
void CPlayer::SetPosition(const Vect3f & Direction, float ElapsedTime)
{
	CCharacter::SetPosition(Direction, ElapsedTime);

	if(m_Camera)
	{
		if (GetPhysicController())
			m_Camera->GetObject3D()->SetPosition(GetPhysicController()->GetPosition());
		else
			m_Camera->GetObject3D()->SetPosition(CAnimatedInstanceModel::GetPosition());
	}

}

void CPlayer::SetYaw(float Yaw)
{
	CCharacter::SetYaw(Yaw);
	m_Camera->SetPlayerYaw(Yaw);
}

void CPlayer::SetPitch(float Pitch)
{
	CCharacter::SetPitch(Pitch);
	m_Camera->SetPlayerPitch(Pitch);
}

void CPlayer::SetRoll(float Roll)
{
	CCharacter::SetRoll(Roll);
	m_Camera->SetPlayerRoll(Roll);
}

void CPlayer::Load()
{
	ChangeWorld(true, GetPosition());
	m_oWeapon.SetWeaponMode(GUN);
	m_oWeapon.SetPlayer(this);
	RegisterPlayer();
}

void CPlayer::Reload()
{
	CORE->GetScriptManager()->RunCode("restart_sound()");

	// Mundo
	m_World.SetWorld(CWorld::ALL_WORLDS);
	ChangeWorld(true, m_vPosIni);
	
	// Player
	CCharacter::Reload();
	RemoveAction(DYE_ANIM);
	CCharacter::SetIsDead(false);
	CCharacter::SetIsHit(false);
	RecoverLife();
	m_bIsDead = false;
	m_bPlayerDying = false;
	m_iNumMirrorPieces = 0;
	m_bIsAiming = false;

	// Arma
	m_oWeapon.SetPlayer(this);
	m_oWeapon.RechargeWeapon();
	m_oWeapon.SetWeaponMode(GUN);
	
}


void CPlayer::ChangeWorld(bool change, const Vect3f& SpawnPos)
{
	if (change)
	{
		m_World.ChangeWorld();
		m_oWeapon.ChangeWeaponMode();
		CORE->GetEnemyManager()->RepositionEnemies(isInRealWorld());
	}
	m_World.LoadWorld();

	SetPosition(SpawnPos);

}

void CPlayer::RegisterPlayer()
{
/*
	lua_State* l_LS = CORE->GetScriptManager()->GetLuaState();
	module(l_LS)
		[
			class_<CPlayer>("CPlayer")
			.def("reload",&CPlayer::Reload)
			.def("set_speed",&CPlayer::SetPlayerSpeed)
		];

	luabind::globals(l_LS)
		[
			"_PLAYER"
		] = getPlayer();		// OJO con esto: si el player se registra la primera vez, al cargar el segundo nivel este puntero ya no es valido, peta !!
*/
}

float CPlayer::GetMaxPlayerLife()
{
	return MAX_LIFE_PLAYER;
}

float CPlayer::GetHitTotalTime() 
{ 
	return PLAYER_HIT_TIME; 
}

void CPlayer::RecoverLife()
{
	SetLife(MAX_LIFE_PLAYER);

}
void CPlayer::PartiallyRecoverLife(float Quantity)
{
	SetLife(GetLife()+Quantity);
}

float CPlayer::GetLanternDistance() 
{ 
	return m_oWeapon.GetLanternDistance();

}

float CPlayer::GetPlayerLifeGUI()
{
	float life = (CCharacter::GetLife()/MAX_LIFE_PLAYER);
	if (life <= 0.01f) life = 0.f;
	else if (life >= 0.98f) life = 1.f;

	return life;
}

float CPlayer::GetPlayerMunitionGUI()
{
	TWeaponMode mode =m_oWeapon.GetWeaponMode();
	float energy = m_oWeapon.GetAvailableMunition();
	if (mode == GUN)
		energy /= m_oWeapon.GetMaxBullets();
	else if (mode == LANTERN)
		energy /= m_oWeapon.GetLanternMaxPowerTime();

	if (energy <= LIFE_THRESHOLD) 
		energy = 0.f;
	else if (energy > 1.0f)
		energy = 1.f;

	return energy;
}