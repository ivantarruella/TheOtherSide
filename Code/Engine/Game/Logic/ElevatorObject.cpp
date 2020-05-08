#include "ElevatorObject.h"
#include "AnimatedModelsManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LogicObjectsManager.h"
#include "Scripting\ScriptManager.h"
#include "GUIManager.h"
#include "ActionToInput.h"
#include "MeshInstance.h"
#include "Player.h"
#include "PhysicController.h"
#include "ElevatorAnimatedPhysicModel.h"
#include "include\PhysicsManager.h"
#include "Logger.h"
#include "Base.h"

#define ELEVATOR_CONTROLLER_HEIGHT		0.7f		// PhysX
 
CElevatorObject::CElevatorObject()
	: CLogicObject(), m_UseItemFile(""), m_fUseTime(0.f), m_bUseActive(false), 
	  m_AnimatedPhysicModel(NULL), m_bIsUp(false), m_bStart(false), m_player(NULL), m_DoorPhysicMesh(NULL), m_vDoorIniPos(Vect3f(0.0f, 0.0f, 0.0f)),
	  m_sOtherElevator("")
{
	m_Type = ELEVATOR;
}

CElevatorObject::CElevatorObject(CXMLTreeNode &atts)
	: CLogicObject(), m_UseItemFile(""), m_fUseTime(0.f), m_bUseActive(false), 
	 m_AnimatedPhysicModel(NULL), m_bIsUp(false), m_bStart(false), m_player(NULL), m_DoorPhysicMesh(NULL), m_vDoorIniPos(Vect3f(0.0f, 0.0f, 0.0f)),
	 m_sOtherElevator("")
{
	m_Type = ELEVATOR;
	m_UseItemFile="UseItem.xml";

	std::string l_layer1 = atts.GetPszProperty("layer","");	
	std::string l_AnimatedModelName1 = atts.GetPszProperty("animated_model_instance","");
	std::string l_DoorMesh = atts.GetPszProperty("door_physic_mesh","");
	
	if (l_layer1 != "" && l_AnimatedModelName1 !="" && l_DoorMesh != "")
	{
		m_AnimatedPhysicModel = (CElevatorAnimatedPhysicModel*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer1)->GetResource(l_AnimatedModelName1);
		
		if (!m_AnimatedPhysicModel)
		{
			std::string msg_error = "CElevatorObject::CElevatorObject->No se encuentra animated model " + l_AnimatedModelName1 + " del ascensor!";
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		}
		else
		{
			m_AnimatedPhysicModel->ClearAllAnims();
			m_vElevatorIniPos = m_AnimatedPhysicModel->GetPosition();
		}

		m_DoorPhysicMesh = (CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer1)->GetResource(l_DoorMesh);

		if (!m_DoorPhysicMesh)
		{
			std::string msg_error = "CElevatorObject::CElevatorObject->No se encuentra la puerta fisica del ascensor!";
			LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		}
		else
			m_vDoorIniPos = m_DoorPhysicMesh->GetPosition();
	}
	else
	{
		std::string msg_error="";

		if (l_layer1 == "")
			msg_error = "CElevatorObject::CElevatorObject->No se encuentra layer " + l_layer1 + " del ascensor!";
		else if (l_AnimatedModelName1 == "")
			msg_error = "CElevatorObject::CElevatorObject->No se encuentra animated model " + l_AnimatedModelName1 + " del ascensor!";

		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
	}

	m_sOtherElevator = atts.GetPszProperty("link","");
}

CElevatorObject::~CElevatorObject()
{
	DisplayUse(false);
}

void CElevatorObject::DisplayUse(bool show)
{
	if (m_UseItemFile!="")
	{
		if (GetEnabled() && show)
		{
			if (!m_bUseActive)
			{
				CORE->GetGUIManager()->GetGUIElement("rotar_espejo")->SetVisible(false);
				//CORE->GetGUIManager()->PushWindows(m_UseItemFile);	
				CORE->GetGUIManager()->GetGUIElement("use_item")->SetVisible(true);
				m_bUseActive = true;
			}
		}
		else
		{
			if (m_bUseActive)
			{
				//CORE->GetGUIManager()->PopWindows();
				CORE->GetGUIManager()->GetGUIElement("use_item")->SetVisible(false);
				m_bUseActive = false;
			}
		}
	}
}


void CElevatorObject::Update(float ElapsedTime)
{
	if (!m_AnimatedPhysicModel || !m_DoorPhysicMesh)
		return;

	m_fUseTime -= ElapsedTime;
	
	if (m_fUseTime<=0.0f)
	{
		if (m_bStart)
		{
			m_AnimatedPhysicModel->ClearCycle(m_bIsUp ? IDDLEUP_ANIM : IDDLEDOWN_ANIM, 0.f);
			m_AnimatedPhysicModel->ExecuteAction(m_bIsUp ? MOVEDOWN_ANIM : MOVEUP_ANIM, 0.f, 0.f);
			m_bIsUp = !m_bIsUp;
			m_fUseTime = ELEVATOR_ANIM_TOTAL_TIME;
			CORE->GetScriptManager()->RunCode("sound_ascensor()");
			m_bStart = false;
			
			if (m_player != NULL)
			{
				CElevatorObject* l_Other = (CElevatorObject*)CORE->GetLogicObjectsManager()->GetResource(m_sOtherElevator);
				if (l_Other != NULL)
				{
					l_Other->SetStart(true);
					l_Other->SetPlayer(NULL);
				}
			}
		}
		else 
		{
			m_AnimatedPhysicModel->BlendCycle(m_bIsUp ? IDDLEUP_ANIM : IDDLEDOWN_ANIM, 0.f);
			m_DoorPhysicMesh->GetPhysicElement()->m_PhysicActor->SetGlobalPosition(m_vDoorIniPos);
		}
	}
	else
	{
		UpdateAnimationPhysX();
		m_AnimatedPhysicModel->BlendCycle(m_bIsUp ? IDDLEUP_ANIM : IDDLEDOWN_ANIM, 0.f);
	}
}


void CElevatorObject::Trigger(const std::string& action, CPlayer* player)
{
	if (GetEnabled() && m_fUseTime <= 0.f)
	{
		if (action=="OnEnter")
		{
			m_player = player;
			m_player->SetCanUseItem(true);
			DisplayUse(true);
		}
		else if (action=="OnLeave")
		{
			if (player != NULL)
				player->SetCanUseItem(false);

			DisplayUse(false);
			m_player = NULL;
		}
		else if (action=="OnTrigger")
		{
			player->SetCanUseItem(true);
			DisplayUse(!player->GetUseButtonPressed());

			if (player->GetUseItem())
			{
				m_player = player;
				m_player->SetCanUseItem(false);

				// close physic door immediately
				CalSkeleton* l_pSkeleton = m_AnimatedPhysicModel->GetCalModel()->getSkeleton();
				int l_CabineBone = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Ascensor Prop3");				
				CalBone* l_pBone = l_pSkeleton->getBone(l_CabineBone);
				CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
				Vect3f l_newpos = m_AnimatedPhysicModel->GetTransform() * Vect3f(l_vTranslation.x, l_vTranslation.y - ELEVATOR_CONTROLLER_HEIGHT + 0.15f, l_vTranslation.z);
				Vect3f l_doorpos = Vect3f(m_vDoorIniPos.x, l_newpos.y + ELEVATOR_CONTROLLER_HEIGHT, m_vDoorIniPos.z);
				m_DoorPhysicMesh->GetPhysicElement()->m_PhysicActor->SetGlobalPosition(l_doorpos);

				m_bStart = true;
			}
		}
	}
	else
	{
		DisplayUse(false);
		if (m_player != NULL)
			m_player->SetCanUseItem(false);
	}
}

void CElevatorObject::UpdateAnimationPhysX()
{
	CalSkeleton* l_pSkeleton = m_AnimatedPhysicModel->GetCalModel()->getSkeleton();
	int l_CabineBone = l_pSkeleton->getCoreSkeleton()->getCoreBoneId("Ascensor Prop3");
	if (l_CabineBone == -1)
		return;

	// bone translation
	CalBone* l_pBone = l_pSkeleton->getBone(l_CabineBone);
	CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
	Vect3f l_newpos = m_AnimatedPhysicModel->GetTransform() * Vect3f(l_vTranslation.x, l_vTranslation.y - ELEVATOR_CONTROLLER_HEIGHT + 0.15f, l_vTranslation.z);
	
	// door physic position
	Vect3f l_doorpos = Vect3f(m_vDoorIniPos.x, l_newpos.y + ELEVATOR_CONTROLLER_HEIGHT, m_vDoorIniPos.z);
	m_DoorPhysicMesh->GetPhysicElement()->m_PhysicActor->SetGlobalPosition(l_doorpos);

	// player physic position
	if (m_player != NULL)
	{
		Vect3f l_playerpos = m_player->GetPosition();
		Vect3f l_pos = Vect3f(l_playerpos.x, l_newpos.y + ELEVATOR_CONTROLLER_HEIGHT, l_playerpos.z);
		m_player->GetPhysicController()->SetPosition(l_pos);
	}

	// elevator physic position
	m_AnimatedPhysicModel->GetPhysicElement()->m_PhysicActor->SetGlobalPosition(l_newpos);
}

void CElevatorObject::Restart()
{
	if (m_bIsUp)
	{
		m_AnimatedPhysicModel->ClearCycle(m_bIsUp ? IDDLEUP_ANIM : IDDLEDOWN_ANIM, 0.f);
		m_AnimatedPhysicModel->BlendCycle(IDDLEDOWN_ANIM, 0.f);
		m_AnimatedPhysicModel->GetPhysicElement()->m_PhysicActor->SetGlobalPosition(m_vElevatorIniPos);
		m_bIsUp = false;
	}
}