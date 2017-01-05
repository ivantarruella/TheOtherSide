#include "GameProcess.h"
#include "luabind\luabind.hpp"
#include "Scripting\ScriptManager.h"
#include "RenderManager.h"
#include "Renderer\SceneRendererCommandManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LogicObjectsManager.h"
#include "GUIManager.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "ActionToInput.h"
#include "Camera\ThirdPersonCamera.h"
#include "FreeCamera.h"
#include "MirrorObject.h"
#include "SpawnPointObject.h"
#include "TutorialObject.h"
#include "DoorObject.h"
#include "FanObject.h"
#include "ElevatorObject.h"
#include "Lasers.h"
#include "LifeRecover.h"
#include "EnergyRecover.h"
#include "MirrorPieceObject.h"
#include "BrokenMirrorObject.h"
#include "Base.h"

#include <algorithm>
using namespace luabind;

void CGameProcess::PreInit()
{
	if (m_Player != NULL)
	{
		m_fPlayerLife = m_Player->GetLife();
		m_fPlayerEnergy = m_Player->GetWeapon().GetAvailableMunition();
	}
}

bool CGameProcess::Init()
{
	if (!IsOk())
		Load();
	else
		Reload();

	return IsOk();
}

void CGameProcess::Release()
{
	CHECKED_DELETE(m_Camera);
	
#ifdef _DEBUG	
	if(m_Player!=NULL)
		CHECKED_DELETE(m_ProvisionalCamera);
#endif

	SetGammaFactor(0.5f);
}

void CGameProcess::Update(float ElapsedTime)
{
	UpdateInputActions();
	
	if (IsOk() && CORE->GetProcess()->GetStartGame() && CORE->GetLevelManager()->IsOk())
	{	
		if(m_Camera->IsFPC())
			m_Camera->Update(ElapsedTime);

		UpdateGUI(ElapsedTime);
		UpdateGameActions();
	}
}

void CGameProcess::UpdateInputActions()
{
	CActionToInput* actionToInput = CORE->GetActionToInput();
	
	if ((m_Player!=NULL && !m_Player->IsDead()) && (actionToInput->DoAction(DOACTION_MENU) || actionToInput->DoAction(DOACTION_MENU_PAD))
		&& !CORE->GetLogicObjectsManager()->GetTutorialActive())
	{
		CORE->GetScriptManager()->RunCode("sound_game()");
		CORE->GetGUIManager()->ActiveWindows("Main.xml");
	}

#ifdef _DEBUG	
	if (actionToInput->DoAction(DOACTION_CHANGECAMERATYPE))
		if(m_Player!=NULL)
			ChangeCamera();
#endif
}

void CGameProcess::UpdateGameActions()
{
	if (m_Player == NULL)
		return;

	if (!m_bReplay && m_Player!=NULL && m_Player->IsDead())
	{
		CORE->GetGUIManager()->PushWindows("Restart.xml");
		m_bReplay = true;
	}

	if (!m_Player->IsDead())
	{
		if (!m_bFinalEnemies && m_Player->GetMirrorPieces() == TOTAL_BROKEN_MIRROR_PIECES)
		{
			CORE->GetScriptManager()->RunCode("create_final_enemies()");
			m_bFinalEnemies = true;
		}
		
		if (!m_bPlayingActionMusic && CORE->GetEnemyManager()->GetNumSoldiers() >= 5) {
			CORE->GetScriptManager()->RunCode("sound_action_music_on()");	// too much enemies, play action music
			m_bPlayingActionMusic = true;
		}

		if (m_bPlayingActionMusic && CORE->GetEnemyManager()->GetNumSoldiers() <= 2) {
			CORE->GetScriptManager()->RunCode("sound_action_music_off()");	// few enemies, stop action music
			m_bPlayingActionMusic = false;
		}
	}
}

void CGameProcess::InitGUIData()
{
	m_sMirrorPieces.clear();
	m_sMirrorPieces.push_back("trozos_0");
	m_sMirrorPieces.push_back("trozos_1");
	m_sMirrorPieces.push_back("trozos_2");
	m_sMirrorPieces.push_back("trozos_3");
	m_sMirrorPieces.push_back("trozos_4");

	m_sBloodText.clear();
	m_sBloodText.push_back("blood_effect_1");
	m_sBloodText.push_back("blood_effect_2");
	m_sBloodText.push_back("blood_effect_3");
}

void CGameProcess::UpdateGUI(float ElapsedTime)
{
	if (!m_Player)
		return;

	// mirilla
	bool visible = m_Player->GetPlayerAimming() && m_Player->GetPlayerMunitionGUI();
	CORE->GetGUIManager()->GetGUIElement("mirilla")->SetVisible(m_Player->isInRealWorld() && visible);
	CORE->GetGUIManager()->GetGUIElement("mirilla_ME")->SetVisible(!m_Player->isInRealWorld() && visible);

	// vida
	CORE->GetGUIManager()->GetGUIElement("health_level")->SetWidthPercent(m_Player->GetPlayerLifeGUI()*GUI_LIVE_WIDTH);
	
	// energia
	CORE->GetGUIManager()->GetGUIElement("energy_level")->SetWidthPercent(m_Player->GetPlayerMunitionGUI()*GUI_ENERGY_WIDTH);

	// trozos espejo final conseguidos
	for (size_t i=0; i<m_sMirrorPieces.size(); ++i) {
		bool l_visible = (i == m_Player->GetMirrorPieces()) && (CORE->GetLevelManager()->GetLevelName() == "Level02 - Aqueronte Corp.");
		CORE->GetGUIManager()->GetGUIElement(m_sMirrorPieces[i])->SetVisible( l_visible );
	}

	// efecto sangre pantalla completa
	if (!m_Player->IsDead())
	{
		if (m_Player->GetIsHit())
			m_fTimeHit += ElapsedTime;
		else {
			if (m_fTimeHit>0.0f) 
				m_fTimeHit -= ElapsedTime;
			else
				m_fTimeHit = 0.0f;
		}

		if (m_fTimeHit > 0.0f)
		{
			std::string l_blood = "";
			if (m_fTimeHit > 0.0f && m_fTimeHit <= MAX_DAMAGE_TIME)
				l_blood = m_sBloodText[(int)m_fTimeHit % (int)MAX_DAMAGE_TIME];

			if (l_blood!="") {
				for (size_t i=0;i<m_sBloodText.size();++i) 
					CORE->GetGUIManager()->GetGUIElement(m_sBloodText[i])->SetVisible(m_sBloodText[i] == l_blood);
			}
		}
		else
		{
			for (size_t i=0;i<m_sBloodText.size();++i)
				CORE->GetGUIManager()->GetGUIElement(m_sBloodText[i])->SetVisible(false);
		}
	}
	else
	{
		m_fTimeHit = 0.0f;
		for (size_t i=0;i<m_sBloodText.size();++i)
			CORE->GetGUIManager()->GetGUIElement(m_sBloodText[i])->SetVisible(false);
	}
}

void CGameProcess::RenderScene(CRenderManager* rendermanager)
{
	CORE->GetSceneRendererCommandManager()->Execute(*rendermanager);
}

void CGameProcess::RenderDebug(CRenderManager* rendermanager)
{
#ifdef _DEBUG	
	if (CORE->GetProcess()->IsOk())
	{
		CCamera * l_Cam;
		if(!m_Camera->IsFPC())
			l_Cam=m_Camera;
		else
			l_Cam=m_ProvisionalCamera;

		//l_Cam->RenderDebug(*rendermanager);
	}
#endif
}

CCamera* CGameProcess::GetCamera()
{
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w, h);
	float l_Aspect = (float)w/(float)h;
	
	if(m_Player==NULL)
	{
		return new CFreeCamera(0.01f, 50.0f, CAMERA_FOV, l_Aspect,Vect3f(0.0f,1.0f,0.0f));
	}
	else 
	{
#ifdef _DEBUG
		Vect3f l_Pos= m_Player->GetPosition();
		m_ProvisionalCamera=new CFreeCamera(0.01f, 50.0f, CAMERA_FOV, l_Aspect,l_Pos+Vect3f(0.0f,1.0f,0.0f));
#endif
		return new CThirdPersonCamera(CAMERA_ZNEAR, CAMERA_ZFAR, CAMERA_FOV, l_Aspect, m_Player, CAM_ZOOM_WALK,CAM_DIST,CAM_MIN_HEIGHT);
	}
}

#ifdef _DEBUG
void CGameProcess::ChangeCamera()
{
	CCamera* l_Cam;

	l_Cam=m_ProvisionalCamera;
	m_ProvisionalCamera=m_Camera;
	m_Camera=l_Cam;
	
	//Setear parametros de la tpc a la fpc
	if(m_Camera->GetTypeCamera()==TC_FPS)
	{
		m_Camera->GetObject3D()->SetPosition(m_ProvisionalCamera->GetObject3D()->GetPosition()+Vect3f(0.0f,1.0f,0.5f));
		m_Camera->SetPlayerYaw(m_ProvisionalCamera->GetPlayerYaw());
		m_Camera->SetPlayerRoll(m_ProvisionalCamera->GetPlayerRoll());
		m_Camera->SetPlayerPitch(m_ProvisionalCamera->GetPlayerPitch());
	}
}
#endif

void CGameProcess::Load()
{
	InitGUIData();

	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
	if (m_Player !=NULL)
		m_Player->Load();

	m_Camera = GetCamera();
	if (CORE->GetLevelManager()->GetLevelName()=="Level01-Tutorial")
		m_Camera->GetObject3D()->SetPosition(CAMERA_INI_POS);
	if (CORE->GetLevelManager()->GetLevelName()=="Level02 - Aqueronte Corp.")
		m_Camera->GetObject3D()->SetRoll(D3DX_PI/2.0f);

	// Proceso cargado ok
	SetIsOk(true);
}

void CGameProcess::Reload()
{
	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("PLAYER");
	if (m_Player != NULL)
	{
		m_Player->Reload();
		
		// Pone nivel de vida/energia del nivel anterior
		if (m_fPlayerLife>0.0f)
			m_Player->SetLife(m_fPlayerLife);
		
		if (m_fPlayerEnergy>0.0f)
			m_Player->GetWeapon().SetAvailableMunition(m_fPlayerEnergy);
	}

	if (m_Camera!=NULL)
		m_Camera->SetPlayerPitch(0.f);

	// Proceso recargado ok
	m_bReplay = false;
	m_bFinalEnemies = false;
	SetIsOk(true);
}

void CGameProcess::ReloadPlayerWeapon()
{
	if (m_Player != NULL)
		m_Player->GetWeapon().ReloadWeapon();
}

void CGameProcess::SetMouseSensitivity(float sensitivity)
{
	if (m_Player == NULL) 
		return;

	float speed = MOUSE_SENSITIVITY * sensitivity * 2;
	if (speed < MOUSE_SENSITIVITY_MIN)
		speed = MOUSE_SENSITIVITY_MIN;
	else if (speed > MOUSE_SENSITIVITY_MAX)
		speed = MOUSE_SENSITIVITY_MAX;

	m_Player->SetMouseSpeed(speed);
}

float CGameProcess::GetMouseSensitivity()
{
	if (m_Player == NULL)
		return 0.f;

	return m_Player->GetMouseSpeed();
}

void CGameProcess::SetGammaFactor(float gamma)
{
	gamma = (1.0f - gamma/2) + 0.25f;

	for( int i=0; i<256; i++ ) 
	{
		ramp[i+0] = ramp[i+256] = ramp[i+512] = 
		(WORD)min(65535, max(0, pow((float)((i+1) / 256.0), gamma) * 65535 + 0.5));
	}

	SetDeviceGammaRamp(::GetDC(NULL), ramp);
}

void CGameProcess::ChangePlayerAnim(int anim, float delay)
{
	if (m_Player==NULL)
		return;

	m_Player->ChangeCharacterAnimation((tAnimationStates)anim, delay);
}

bool CGameProcess::UseItem()
{
	return m_Player && m_Player->GetUseItem();
}


void CGameProcess::SetPlayerCanUse(bool use)
{
	if (m_Player==NULL)
		return;

	m_Player->SetCanUseItem(use);
}

int CGameProcess::GetNumPiecesFound()
{
	if (m_Player==NULL)
		return 0;

	return m_Player->GetMirrorPieces();
}

bool CGameProcess::IsInRealWorld()
{
	if (m_Player==NULL)
		return 0;

	return m_Player->isInRealWorld();
}

// Triggers
void CGameProcess::MirrorTrigger(const std::string& action, const std::string& mirrorName, const std::string& targetMirror, const std::string& rotatedMirror)
{
	CMirrorObject* l_Obj = (CMirrorObject*) CORE->GetLogicObjectsManager()->GetResource(mirrorName);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player, targetMirror, rotatedMirror);
}

void CGameProcess::TutorialTrigger(const std::string& action, const std::string& Name)
{
	CTutorialObject* l_Obj = (CTutorialObject*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

void CGameProcess::DoorTrigger(const std::string& action, const std::string& Name)
{
	CDoorObject* l_Obj = (CDoorObject*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

void CGameProcess::ElevatorTrigger(const std::string& action, const std::string& Name)
{
	CElevatorObject* l_Obj = (CElevatorObject*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

void CGameProcess::LasersTrigger(const std::string& action, const std::string& Name)
{
	CLasers* l_Obj = (CLasers*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

void CGameProcess::LifeTrigger(const std::string& action, const std::string& Name)
{
	CLifeRecover* l_Obj = (CLifeRecover*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

void CGameProcess::EnergyTrigger(const std::string& action, const std::string& Name)
{
	CEnergyRecover* l_Obj = (CEnergyRecover*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

void CGameProcess::MirrorPieceTrigger(const std::string& action, const std::string& Name)
{
	CMirrorPieceObject* l_Obj = (CMirrorPieceObject*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

void CGameProcess::BrokenMirrorTrigger(const std::string& action, const std::string& Name)
{
	CBrokenMirrorObject* l_Obj = (CBrokenMirrorObject*) CORE->GetLogicObjectsManager()->GetResource(Name);
	assert(l_Obj);
	l_Obj->Trigger(action, m_Player);
}

// Lua functions
void CGameProcess::RegisterLUA()
{
	lua_State* l_LS = CORE->GetScriptManager()->GetLuaState();
	module(l_LS)
		[
			class_<CGameProcess>("CViewerProcess")
			
			// process
			.def("load",&CGameProcess::Load)
			.def("reload",&CGameProcess::Reload)
			
			// player
			.def("get_player",&CGameProcess::GetPlayer)
			.def("change_player_anim",&CGameProcess::ChangePlayerAnim)
			.def("use_item",&CGameProcess::UseItem)
			.def("set_can_use_item",&CGameProcess::SetPlayerCanUse)
			.def("get_num_pieces_found",&CGameProcess::GetNumPiecesFound)
			.def("is_in_real_world",&CGameProcess::IsInRealWorld)

			// setup
			.def("reload_player_weapon",&CGameProcess::ReloadPlayerWeapon)
			.def("get_mouse_sensitivity",&CGameProcess::GetMouseSensitivity)
			.def("set_mouse_sensitivity",&CGameProcess::SetMouseSensitivity)
			.def("set_gamma_factor",&CGameProcess::SetGammaFactor)
			
			// game
			.def("mirror_trigger",&CGameProcess::MirrorTrigger)
			.def("tutorial_trigger",&CGameProcess::TutorialTrigger)
			.def("door_trigger",&CGameProcess::DoorTrigger)
			.def("elevator_trigger",&CGameProcess::ElevatorTrigger)
			.def("lasers_trigger",&CGameProcess::LasersTrigger)
			.def("life_trigger",&CGameProcess::LifeTrigger)
			.def("energy_trigger",&CGameProcess::EnergyTrigger)
			.def("mirror_piece_trigger",&CGameProcess::MirrorPieceTrigger)
			.def("broken_mirror_trigger",&CGameProcess::BrokenMirrorTrigger)
		];

	luabind::globals(l_LS)
		[
			"_GAMEPROCESS"
		] = this;	
}