#include "TestPhysXProcess.h"
#include "luabind\luabind.hpp"
#include "Scripting\ScriptManager.h"
#include "RenderManager.h"
#include "ActionToInput.h"
#include "Player.h"
#include "Camera\ThirdPersonCamera.h"
#include "FreeCamera.h"
#include "Renderer\SceneRendererCommandManager.h"
#include "AnimatedModelsManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "ParticleManager.h"
#include "GUIManager.h"
#include "include\PhysicController.h"
#include "include\PhysicsManager.h"
#include "LightManager.h"
#include "LogicObjectsManager.h"
#include "SpotLight.h"
#include "Soldier.h"
#include "Monster.h"
#include "Player.h"
#include "Base.h"

using namespace luabind;

bool CTestPhysXProcess::Init()
{
	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("player");
	m_Camera = GetCamera();
		
	if (m_Player !=NULL)
		m_Player->RegisterPlayer();

	// Linterna i laser de prueba
	m_Linterna = (CSpotLight*)CORE->GetLightManager()->GetResource("Linterna");
	m_Laser = (CSpotLight*)CORE->GetLightManager()->GetResource("Laser");

	// Gestión mundos
	ChangeWorldActions();

	return true;
}


CCamera* CTestPhysXProcess::GetCamera()
{
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w, h);
	float l_Aspect = (float)w/(float)h;
	
	if(m_Player==NULL)
		return new CFreeCamera(0.01f, 100.0f,55.0f * FLOAT_PI_VALUE/180.0f, l_Aspect,Vect3f(0.f, CONTROLLER_HEIGHT, 0.f));
	else 
		return new CThirdPersonCamera(0.01f, 50.0f, 55.0f * FLOAT_PI_VALUE/180.0f, l_Aspect, m_Player, CAM_ZOOM_WALK,CAM_MIN_DIST,0.7f);
}

void CTestPhysXProcess::Release()
{
	CHECKED_DELETE(m_Camera);
}

void CTestPhysXProcess::Update(float ElapsedTime)
{
	UpdateInputActions();
	
	if (CORE->GetProcess()->GetStartGame())
	{	
		if(m_Player==NULL)
			((CFreeCamera*)m_Camera)->Update(ElapsedTime);
		
		UpdatePlayerWeapon();
		m_fChangeTime += ElapsedTime;
	}
}

void CTestPhysXProcess::UpdateInputActions()
{
	CActionToInput* actionToInput = CORE->GetActionToInput();
	if (actionToInput->DoAction(DOACTION_MENU))
		CORE->GetGUIManager()->ActiveWindows("Main.xml");
}

void CTestPhysXProcess::RenderScene(CRenderManager* rendermanager)
{
	CORE->GetSceneRendererCommandManager()->Execute(*rendermanager);
}

void CTestPhysXProcess::RenderDebug(CRenderManager* rendermanager)
{

}

void CTestPhysXProcess::UpdatePlayerWeapon()
{
	// calculo de las coordenadas del hueso donde esta la pistola para hacer salir la luz desde esta posicion
	Vect3f l_LightPos;
	if (m_Player != NULL)
	{
		l_LightPos  = m_Player->GetPosition();
		CalSkeleton* l_pSkeleton = m_Player->GetCalModel()->getSkeleton();
		int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Dummy_Arma");
		if (l_iBoneId != -1)
		{
			CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);
			CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
			Mat44f l_TransfMat = m_Player->GetTransform();
			l_LightPos = l_TransfMat * Vect3f(l_vTranslation.x, l_vTranslation.y, l_vTranslation.z);
		}

		/*
		if (m_Weapon)
		{
			m_Weapon->GetPhysicActor()->SetGlobalPosition(l_LightPos);
		}
		*/
	
		CThirdPersonCamera* l_Camera=(CThirdPersonCamera*)m_Camera;
		float l_fYaw = l_Camera->GetPlayerYaw();
		float l_fPitch = l_Camera->GetPlayerPitch();

		//if (m_bWorld)		// Linterna sólo en mundo espejo
		{
			if(m_Linterna != NULL)
			{
				m_Linterna->SetVisible(m_Player->GetPlayerAimming());
				m_Linterna->SetPosition(l_LightPos);
				m_Linterna->SetDirection(Vect3f(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch), sin(l_fYaw) * cos(l_fPitch)));
			}
		}

		if(m_Laser != NULL)
		{
			m_Laser->SetVisible(m_Player->GetPlayerShoot() && m_Player->GetPlayerAimming());
			if (m_Laser->GetVisible())
			{
				if (m_Linterna)
					m_Linterna->SetVisible(false);
			}

			m_Laser->SetPosition(l_LightPos);
			m_Laser->SetDirection(Vect3f(cos(l_fYaw) * cos(l_fPitch), sin(l_fPitch), sin(l_fYaw) * cos(l_fPitch)));
		}
	}
}


void CTestPhysXProcess::Reload()
{
	m_Player=(CPlayer*)CORE->GetRenderableObjectsLayersManager()->GetResource("solid")->GetInstance("player");
	if (m_Player)
		m_Player->Reload();

	// Linterna i laser de prueba
	m_Linterna = (CSpotLight*)CORE->GetLightManager()->GetResource("Linterna");
	m_Laser = (CSpotLight*)CORE->GetLightManager()->GetResource("Laser");
}


void CTestPhysXProcess::ChangeWorldActions()
{
	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("solid_MR", !m_bWorld);
	CORE->GetRenderableObjectsLayersManager()->SetLayerEnabled("solid_ME", m_bWorld);

	CORE->GetLightManager()->ChangeWorldLighting(CORE->GetRenderManager(), REAL_WORLD, !m_bWorld);
	CORE->GetLightManager()->ChangeWorldLighting(CORE->GetRenderManager(), MIRROR_WORLD, m_bWorld);
}

void CTestPhysXProcess::ChangeWorld(const std::string& pointName)
{
	Vect3f l_NewPos;

	if (m_fChangeTime > TMIN_WORLD_CHANGE)
	{
		if (CORE->GetActionToInput()->DoAction(DOACTION_PLAYERUSEITEM))
		{
			CObject3D* l_Point = (CObject3D*) CORE->GetLogicObjectsManager()->GetResource(pointName);
			if (l_Point)
			{
				l_NewPos = l_Point->GetPosition();
				m_Player->SetPosition(l_NewPos);
		
				m_bWorld = !m_bWorld;
				ChangeWorldActions();
				
				m_fChangeTime = 0.f;
			}
			else
			{
				std::string msg_error = "CPlayer::ChangeWorld->Punto de spawn " + pointName + " no existe!\n";
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			}
		}
	}
}

void CTestPhysXProcess::RegisterLUA()
{
	lua_State* l_LS = CORE->GetScriptManager()->GetLuaState();
	module(l_LS)
		[
			class_<CViewerProcess>("CViewerProcess")
			.def("change_world",&CViewerProcess::ChangeWorld)
		];

	luabind::globals(l_LS)
		[
			"_GAMEPROCESS"
		] = this;	
	
	if (m_Player !=NULL)
		m_Player->RegisterPlayer();
}