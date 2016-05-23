#include "TutorialObject.h"
#include "ActionToInput.h"
#include "GUIManager.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Base.h"

CTutorialObject::CTutorialObject()
	: m_bInfoActive(false), m_XMLFile(""), m_fTime(0.f), m_bUseActive(false), m_bIsAuto(false)
{
	m_Type = TUTORIAL;
}

CTutorialObject::CTutorialObject(CXMLTreeNode &atts)
	: CLogicObject(atts), m_bInfoActive(false), m_XMLFile(""), m_fTime(0.f), m_bUseActive(false), m_bIsAuto(false)
{
	m_Type = TUTORIAL;
	m_XMLFile = atts.GetPszProperty("xml_file","");
	m_UseItemFile="UseItem.xml";	
	m_bIsAuto = atts.GetBoolProperty("auto", false);		// si tutorial tipo 'auto' se activa de manera automatica sin apretar 'usar'
}

CTutorialObject::~CTutorialObject()
{
	DisplayInfo(false);
	DisplayUse(false);
}

void CTutorialObject::Update(float ElapsedTime)
{
	m_fTime += ElapsedTime;
}

void CTutorialObject::DisplayUse(bool show)
{
	if (GetEnabled() && m_UseItemFile!="")
	{
		if (show)
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

void CTutorialObject::DisplayInfo(bool show)
{
	if (GetEnabled() && m_XMLFile!="")
	{
		if (show)
		{
			if (!m_bInfoActive)
			{
				CORE->GetEnemyManager()->FreezeAll();
				CORE->GetGUIManager()->PushWindows(m_XMLFile);
				m_bInfoActive = true;
			}
		}
		else
		{
			if (m_bInfoActive)
			{
				CORE->GetEnemyManager()->UnfreezeAll();
				CORE->GetGUIManager()->PopWindows();
				m_bInfoActive = false;
			}
		}
	}
}

void CTutorialObject::Trigger(const std::string& action, CPlayer* player)
{
	if (GetEnabled())
	{
		if (action=="OnEnter")
		{
			if (!m_bIsAuto) {
				DisplayUse(true);
				player->SetCanUseItem(true);
			}
			else
				DisplayInfo(true);
		}
		else if (action=="OnLeave")
		{
			player->SetCanUseItem(false);
			DisplayUse(false);
			DisplayInfo(false);
			if (m_bIsAuto)
				SetEnabled(false);
		}
		else if (action=="OnTrigger")
		{
			if (m_fTime > TUTORIAL_USE_TIME)
			{
				player->SetCanUseItem(true);

				// Abrir tutorial (mediante boton usar, si no es auto)
				if (!m_bIsAuto && player->GetUseItem())
				{	
					DisplayUse(false);
					DisplayInfo(true);

					m_fTime = 0.f;
				}
				
				// Cerrar tutorial
				if (m_bInfoActive && (CORE->GetActionToInput()->DoAction(DOACTION_CLOSETUTORIAL) || CORE->GetActionToInput()->DoAction(DOACTION_CLOSETUTORIAL_PAD)))
				{	
					player->SetCanUseItem(false);
					DisplayInfo(false);
					SetEnabled(false);
					/*if (m_bIsAuto)
						SetEnabled(false);
					else
						DisplayUse(true);
					*/
					m_fTime = 0.f;
				}

				if (!m_bIsAuto)
					DisplayUse(!player->GetUseButtonPressed());
			}
		}
	}
	else
		DisplayUse(false);

}