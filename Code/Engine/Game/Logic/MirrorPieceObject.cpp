#include "MirrorPieceObject.h"
#include "Player.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "GUIManager.h"
#include "EnemyManager.h"
#include "ActionToInput.h"
#include "Base.h"

CMirrorPieceObject::CMirrorPieceObject()
	: CLogicObject(), m_player(NULL), m_Mesh(NULL), m_bStart(false), m_GUIFile(""), m_bInfoActive(false)
{
	m_Type = MIRROR_PIECE;
}

CMirrorPieceObject::CMirrorPieceObject(CXMLTreeNode &atts)
	: CLogicObject(), m_player(NULL), m_Mesh(NULL), m_bStart(false), m_GUIFile(""), m_bInfoActive(false)
{
	m_Type = MIRROR_PIECE;
	m_GUIFile="MirrorPiece.xml";

	std::string l_MeshName = atts.GetPszProperty("renderable_object", "");
	std::string l_layer = atts.GetPszProperty("layer", "");

	if (l_layer != "" && l_MeshName != "")
	{
		m_Mesh = (CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer)->GetResource(l_MeshName);
	}
	else
	{
		std::string msg_error = "CMirrorPieceObject::CMirrorPieceObject->No se encuentra renderable object " + l_MeshName + " del trozo de espejo!";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}
}

CMirrorPieceObject::~CMirrorPieceObject()
{
}

void CMirrorPieceObject::DisplayInfo(bool show)
{
	if (GetEnabled() && m_GUIFile!="")
	{
		if (show)
		{
			if (!m_bInfoActive)
			{
				CORE->GetEnemyManager()->FreezeAll();
				CORE->GetGUIManager()->PushWindows(m_GUIFile);
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
				m_bStart = true;
			}
		}
	}
}

void CMirrorPieceObject::Update(float ElapsedTime)
{
	if (m_bStart)
	{
		m_player->IncMirrorPiece();
		if (m_Mesh != NULL)
		{
			m_Mesh->SetEnabled(false);
			m_Mesh->SetVisible(false);
		}

		m_bStart = false;
		SetEnabled(false);
	}
}

void CMirrorPieceObject::Trigger(const std::string& action, CPlayer* player)
{
	if (GetEnabled())
	{
		if (action=="OnEnter")
		{
		}
		else if (action=="OnLeave")
		{
		}
		else if (action=="OnTrigger")
		{
			m_player = player;
			DisplayInfo(true);
			if (CORE->GetActionToInput()->DoAction(DOACTION_CLOSETUTORIAL) || CORE->GetActionToInput()->DoAction(DOACTION_CLOSETUTORIAL_PAD))
			{
				DisplayInfo(false);
				SetEnabled(false);
			}
		}
	}
}

void CMirrorPieceObject::Restart()
{
	SetEnabled(true);

	if (m_Mesh!=NULL)
	{
		m_Mesh->SetEnabled(true);
		m_Mesh->SetVisible(true);
	}

	DisplayInfo(false);
}