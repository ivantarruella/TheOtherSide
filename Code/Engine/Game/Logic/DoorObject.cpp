#include "DoorObject.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LogicObjectsManager.h"
#include "Scripting\ScriptManager.h"
#include "GUIManager.h"
#include "ActionToInput.h"
#include "MeshInstance.h"
#include "Player.h"
#include "Logger.h"
#include "Base.h"

CDoorObject::CDoorObject()
	: CLogicObject(), m_UseItemFile(""), m_fUseTime(0.f), m_bUseActive(false), m_bAuto(false), m_sSwitchName(""), m_bInside(false), m_bMustClose(false)
{
	m_Type = DOOR;
	SetName("");
}

CDoorObject::CDoorObject(CXMLTreeNode &atts)
	: CLogicObject(), m_UseItemFile(""), m_fUseTime(0.f), m_bUseActive(false), m_bAuto(false), m_sSwitchName(""), m_bInside(false), m_bMustClose(false)
{
	m_Type = DOOR;
	SetName(atts.GetPszProperty("name",""));
	
	float l_closetime = atts.GetFloatProperty("close_time", 0.0f);
	m_bAuto = atts.GetBoolProperty("auto", false);
	if (!m_bAuto)
		m_UseItemFile="UseItem.xml";
	else
		m_sSwitchName = atts.GetPszProperty("switch", "", false);

	m_vDoors.clear();

	std::string l_layer="", l_Renderable_Object="";
	l_layer = atts.GetPszProperty("layer1","");
	if (l_layer!="")
	{
		l_Renderable_Object = atts.GetPszProperty("renderable_object1","");
		CInstanceMesh* mesh=(CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer)->GetResource(l_Renderable_Object);
		if (mesh==NULL)
		{
			std::string msg_error = "CDoorObject::CDoorObject->No se encuentra renderable object " + l_Renderable_Object + " de la puerta!";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		}
		else 
		{
			SDoor* l_door = new SDoor();
			
			float s = mesh->GetStaticMesh()->getStaticMeshMaxPoint().y - mesh->GetStaticMesh()->getStaticMeshMinPoint().y;
			s *= 0.85f;
			l_door->SetSizeY(s);
			l_door->SetPosIni(mesh->GetPosition());
			l_door->SetMesh(mesh);
			l_door->SetLayer(l_layer);
			l_door->SetCloseTime(l_closetime);
			
			m_vDoors.push_back(l_door);
		}
	}	

	l_layer = atts.GetPszProperty("layer2","");
	if (l_layer!="")
	{
		l_Renderable_Object = atts.GetPszProperty("renderable_object2","");
		CInstanceMesh* mesh=(CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer)->GetResource(l_Renderable_Object);
		if (mesh==NULL)
		{
			std::string msg_error = "CDoorObject::CDoorObject->No se encuentra renderable object " + l_Renderable_Object + " de la puerta!";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		}
		else 
		{
			SDoor* l_door = new SDoor();
			
			float s = mesh->GetStaticMesh()->getStaticMeshMaxPoint().y - mesh->GetStaticMesh()->getStaticMeshMinPoint().y;
			s *= 0.85f;
			l_door->SetSizeY(s);
			l_door->SetPosIni(mesh->GetPosition());
			l_door->SetMesh(mesh);
			l_door->SetLayer(l_layer);
			l_door->SetCloseTime(l_closetime);

			m_vDoors.push_back(l_door);
		}
	}	

}

CDoorObject::~CDoorObject()
{
	DisplayUse(false);

	for (size_t i=0; i<m_vDoors.size(); ++i)
	{
		CHECKED_DELETE(m_vDoors[i]);
	}

	m_vDoors.clear();
}

void CDoorObject::Update(float ElapsedTime)
{
	m_fUseTime -= ElapsedTime;

	for (size_t i=0; i<m_vDoors.size(); ++i)
	{
		SDoor* l_door = m_vDoors[i];
		bool bClose = l_door->Update(ElapsedTime, m_bMustClose);
		if (bClose) {
			SetMustClose(false);
			m_fUseTime = DOOR_ANIM_TIME;
		}
		if (l_door->IsOpen() && l_door->GetCloseTime() > 0.0f && !m_bInside)	
		{
			if (m_fUseTime <= 0.0f) 
			{
				l_door->SetStart(true);
				if (i==m_vDoors.size()-1)
					m_fUseTime = DOOR_ANIM_TIME;
				l_door->CloseDoor(ElapsedTime, false);
			}
		}
	}
}

void CDoorObject::DisplayUse(bool show)
{
	if (m_UseItemFile!="")
	{
		if (GetEnabled() && show)
		{
			if (!m_bUseActive)
			{
				//CORE->GetGUIManager()->PushWindows(m_UseItemFile);	
				CORE->GetGUIManager()->GetGUIElement("use_item")->SetVisible(true);
				CORE->GetGUIManager()->GetGUIElement("rotar_espejo")->SetVisible(false);
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

void CDoorObject::Trigger(const std::string& action, CPlayer* player)
{
	if (GetEnabled() && m_fUseTime <= 0.f)
	{
		if (action=="OnEnter")
		{
			player->SetCanUseItem(true);

			if (m_bAuto)		// Abertura automatica por detras
			{
				CDoorObject* l_Obj = (CDoorObject*) CORE->GetLogicObjectsManager()->GetResource(m_sSwitchName);
				if (l_Obj!=NULL)
				{
					std::vector<SDoor*> l_vDoors = l_Obj->GetDoors();

					for (size_t i=0; i<l_vDoors.size(); ++i)
					{
						if (!l_vDoors[i]->IsOpen() && !l_vDoors[i]->IsOpening())	// Miramos estado puertas interruptor principal
						{
							m_vDoors[i]->SetStart(true);
							m_fUseTime = m_vDoors[0]->GetCloseTime() + DOOR_ANIM_TIME;
						}
					}
				}
				m_bInside = true;
			}
			else
				DisplayUse(true);
		}
		else if (action=="OnLeave")
		{
			if (!m_bAuto)
				DisplayUse(false);
			else 
			{
				m_bInside = false;
				m_fUseTime = m_vDoors[0]->GetCloseTime();
			}

			player->SetCanUseItem(false);
		}
		else if (action=="OnTrigger")
		{
			player->SetCanUseItem(true);
			if (!m_bAuto)
			{
				DisplayUse(true);

				if (player->GetUseItem())
				{
					CORE->GetScriptManager()->RunCode("sound_bip()");
					for (size_t i=0; i<m_vDoors.size(); ++i)
					{
						m_vDoors[i]->SetStart(true);
					}
					m_fUseTime = m_vDoors[0]->GetCloseTime() + DOOR_ANIM_TIME;
				}
			}
			else
			{
				CDoorObject* l_Obj = (CDoorObject*) CORE->GetLogicObjectsManager()->GetResource(m_sSwitchName);
				if (l_Obj!=NULL)
				{
					std::vector<SDoor*> l_vDoors = l_Obj->GetDoors();

					for (size_t i=0; i<l_vDoors.size(); ++i)
					{
						if (!l_vDoors[i]->IsOpen() && !l_vDoors[i]->IsOpening())	// Miramos estado puertas interruptor principal
						{
							m_vDoors[i]->SetStart(true);
							m_fUseTime = m_vDoors[0]->GetCloseTime() + DOOR_ANIM_TIME;
						}
					}
				}

				m_bInside = true;
			}
		}
	}
	else
	{
		DisplayUse(false);
		if (action=="OnLeave")
		{
			m_bInside = false;
			m_fUseTime = m_vDoors[0]->GetCloseTime();
		}
		player->SetCanUseItem(false);
	}
}

void CDoorObject::Restart()
{
	m_bMustClose = false; 
	m_bInside = false;
}

bool CDoorObject::IsOpen()
{
	bool bIsOpen = m_vDoors[0]->IsOpen() && m_vDoors[1]->IsOpen();
	return bIsOpen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SDoor class

bool SDoor::Update(float ElapsedTime, bool mustClose)
{
	bool bClosed = false;

	if (!m_bIsOpen && !mustClose)		
		OpenDoor(ElapsedTime);
	else				
		bClosed = CloseDoor(ElapsedTime, mustClose && m_bIsOpen);

	return bClosed;
}

void SDoor::OpenDoor(float ElapsedTime)
{
	if (m_DoorMesh==NULL)
		return;

	if (m_bStart)		
	{
		m_LearpAnim.SetValues(0.0f, m_fSizeY, DOOR_ANIM_TIME, FUNC_INCREMENT);
		m_bStart = false;
		m_bOpening = true;
		CORE->GetScriptManager()->RunCode("sound_puerta()");
		m_fTime = 0.f;
	}

	if (m_bOpening)
	{
		m_fTime +=ElapsedTime;

		m_LearpAnim.Update(ElapsedTime, m_fY);
		if (m_fY>=0.f && m_fY<m_fSizeY)
		{
			Vect3f l_pos = m_vPosIni + Vect3f(0.0f, m_fY, 0.0f);
			if (l_pos.y > m_vPosIni.y+m_fSizeY)
				l_pos.y = m_vPosIni.y+m_fSizeY;

			UpdatePhysXMeshPosition(l_pos);
		}

		if (m_fY>=m_fSizeY || m_fTime >=DOOR_ANIM_TIME)
		{
			m_vPosEnd = m_vPosIni + Vect3f(0.0f, m_fSizeY, 0.0f);
			UpdatePhysXMeshPosition(m_vPosEnd);
			m_bIsOpen = true;
			m_bOpening = false;
		}
	}
}

bool SDoor::CloseDoor(float ElapsedTime, bool start)
{
	if (m_DoorMesh==NULL)
		return false;

	if ((m_bStart || start)	 && !m_bOpening)	
	{
		m_LearpAnim.SetValues(0.0f, -m_fSizeY, DOOR_ANIM_TIME, FUNC_DECREMENT);
		m_bStart = false;
		m_bOpening = true;
		CORE->GetScriptManager()->RunCode("sound_puerta()");
		m_fTime = 0.f;
	}

	if (m_bOpening)
	{
		m_fTime +=ElapsedTime;
		m_LearpAnim.Update(ElapsedTime, m_fY);
		if ( m_fY<0 && m_fY >= -m_fSizeY)
		{
			Vect3f l_pos = m_vPosEnd + Vect3f(0.0f, m_fY, 0.0f);
			if (l_pos.y < m_vPosIni.y)
				l_pos.y = m_vPosIni.y;

			UpdatePhysXMeshPosition(l_pos);
		}

		if (m_fY<=-m_fSizeY || m_fTime >=DOOR_ANIM_TIME)
		{
			UpdatePhysXMeshPosition(m_vPosIni);
			m_bIsOpen = false;
			m_bOpening = false;

			return true;
		}
	}

	return false;
}

void SDoor::UpdatePhysXMeshPosition(const Vect3f& pos)
{
	if (m_DoorMesh!=NULL)
	{
		m_DoorMesh->SetPosition(pos);
		m_DoorMesh->GetPhysicActor()->SetGlobalPosition(pos);
	}
}

