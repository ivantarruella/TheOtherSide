#include "BrokenMirrorObject.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LogicObjectsManager.h"
#include "MirrorObject.h"
#include "GUIManager.h"
#include "ActionToInput.h"
#include "Player.h"
#include "Base.h"

CBrokenMirrorObject::CBrokenMirrorObject()
	: m_UseItemFile(""), m_fTime(0.f), m_bUseActive(false), m_player(NULL), m_bIsBroken(true), m_BrokenMirrorMesh(NULL), m_sFinalMirrorTrigger(""),
	  m_bDone(false), m_bStart(false), m_sLink(""), m_bTextActive(false)
{
	m_Type = BROKEN_MIRROR;
}

CBrokenMirrorObject::CBrokenMirrorObject(CXMLTreeNode &atts)
	: CLogicObject(atts), m_UseItemFile(""), m_fTime(0.f), m_bUseActive(false), m_player(NULL), m_bIsBroken(true), m_BrokenMirrorMesh(NULL), m_sFinalMirrorTrigger(""),
	  m_bDone(false), m_bStart(false), m_sLink(""), m_bTextActive(false)
{
	m_Type = BROKEN_MIRROR;
	m_UseItemFile="MountMirror.xml";	
	m_sNotComplete="MissingPieces.xml";

	std::string l_Layer = atts.GetPszProperty("layer","");
	std::string l_BrokenMirrorName = atts.GetPszProperty("renderable_object","");
	if (l_Layer != "" && l_BrokenMirrorName != "")
	{
		m_BrokenMirrorMesh = (CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_Layer)->GetResource(l_BrokenMirrorName);
		m_BrokenMirrorMesh->SetVisible(true);
	}

	m_sFinalMirrorTrigger = atts.GetPszProperty("final_mirror_trigger","");
	m_sLink = atts.GetPszProperty("link","");

	if (l_Layer=="" || l_BrokenMirrorName=="" || m_sFinalMirrorTrigger=="" || m_BrokenMirrorMesh==NULL || m_sLink=="")
	{
		std::string msg_error = "CBrokenMirrorObject::CBrokenMirrorObject->No se encuentra renderable object " + l_BrokenMirrorName + " del espejo roto!";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}
}

CBrokenMirrorObject::~CBrokenMirrorObject()
{
	DisplayUse(false);
}

void CBrokenMirrorObject::Update(float ElapsedTime)
{
	if (m_sFinalMirrorTrigger=="" || m_BrokenMirrorMesh==NULL)
		return;

	if (m_bIsBroken && !m_bDone)
	{
		CMirrorObject* l_FinalMirror = (CMirrorObject*)CORE->GetLogicObjectsManager()->GetResource(m_sFinalMirrorTrigger);
		if (l_FinalMirror!=NULL)
		{
			l_FinalMirror->SetMirrorON(false);
			l_FinalMirror->GetRenderableObject()->SetVisible(false);
		}
		m_bDone = true;
	}

	if (/*m_bStart &&*/  m_player!=NULL)
	{
		if (/*m_bIsBroken &&*/ m_player->GetMirrorPieces() == TOTAL_BROKEN_MIRROR_PIECES)
		{
			m_bIsBroken = false;
			SetEnabled(false);
			
			// Link otro mundo
			if (m_sLink!="")
			{
				CBrokenMirrorObject* l_OhterMirrorTrigger = (CBrokenMirrorObject*)CORE->GetLogicObjectsManager()->GetResource(m_sLink);
				if (l_OhterMirrorTrigger!=NULL) 
				{
					CMirrorObject* l_OhterMirrorMesh = (CMirrorObject*)CORE->GetLogicObjectsManager()->GetResource(l_OhterMirrorTrigger->GetFinalMirrorTrigger());
					l_OhterMirrorTrigger->GetBrokenMirrorMesh()->SetVisible(false);
					l_OhterMirrorMesh->GetRenderableObject()->SetVisible(true);
					l_OhterMirrorMesh->SetCanChangeWorld(false);
					l_OhterMirrorMesh->SetMirrorON(true);
					l_OhterMirrorTrigger->SetEnabled(false);
				}
			}
		}

		m_bStart = false;
	}
}

void CBrokenMirrorObject::DisplayUse(bool show)
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

void CBrokenMirrorObject::Trigger(const std::string& action, CPlayer* player)
{
	if (GetEnabled())
	{
		if (action=="OnEnter")
		{
			DisplayUse(true);
			player->SetCanUseItem(true);
		}
		else if (action=="OnLeave")
		{
			player->SetCanUseItem(false);
			DisplayUse(false);
		}
		else if (action=="OnTrigger")
		{
			m_player = player;
			m_player->SetCanUseItem(true);
			m_bStart = m_player->GetUseItem();
			DisplayUse(!m_bStart);

			if (m_bStart && !m_bTextActive && player->GetMirrorPieces() < TOTAL_BROKEN_MIRROR_PIECES)
			{
				CORE->GetGUIManager()->PushWindows(m_sNotComplete);
				m_bTextActive = true;
			}

			if (m_bTextActive && (CORE->GetActionToInput()->DoAction(DOACTION_CLOSETUTORIAL) || CORE->GetActionToInput()->DoAction(DOACTION_CLOSETUTORIAL_PAD)))
			{
				CORE->GetGUIManager()->PopWindows();
				m_bTextActive = false;
			}
		}
	}
	else
	{
		if (!m_bIsBroken && m_bDone)
		{
			CMirrorObject* l_FinalMirror = (CMirrorObject*)CORE->GetLogicObjectsManager()->GetResource(m_sFinalMirrorTrigger);
			l_FinalMirror->GetRenderableObject()->SetVisible(true);
			l_FinalMirror->SetCanChangeWorld(false);
			l_FinalMirror->SetMirrorON(true);
			m_bDone = false;
			m_BrokenMirrorMesh->SetVisible(false);
		}
	}
}

void CBrokenMirrorObject::Restart()
{
	m_bIsBroken = true;
	m_bDone = false;
	m_bStart = false;

	if (m_BrokenMirrorMesh!=NULL)
		m_BrokenMirrorMesh->SetVisible(true);

	if (m_sFinalMirrorTrigger!="")
	{
		CMirrorObject* l_FinalMirror = (CMirrorObject*)CORE->GetLogicObjectsManager()->GetResource(m_sFinalMirrorTrigger);
		l_FinalMirror->SetMirrorON(false);
	}

	SetEnabled(true);
}