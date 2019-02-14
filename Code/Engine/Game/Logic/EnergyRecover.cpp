#include "EnergyRecover.h"
#include "ActionToInput.h"
#include "GUIManager.h"
#include "Player.h"
#include "TextureManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LightManager.h"
#include "Scripting\ScriptManager.h"
#include "Weapon.h"
#include "Base.h"

#define ENERGY_INC_TIME		0.05f	// incrementa energia cada x seg.
#define ENERGY_INC_VALUE	(3.0f)	// cuanto incrementa de energia

#define MAX_ENERGY_RECOVER_VALUE	150.0f

#define LIGHT_VAR_INTENSITY		0.1f

CEnergyRecover::CEnergyRecover()
	: CLogicObject(), m_fTime(0.0f), m_bStart(false), m_player(NULL), m_fMaxEnergyRecover(0.0f), m_bUseActive(false), m_UseItemFile(""),
	m_OnTexture(NULL), m_OffTexture(NULL), m_Mesh(NULL), m_Light(NULL), m_orientation(0)
{
	m_Type = ENERGY_RECOVER;
	m_fMaxEnergyRecover = MAX_ENERGY_RECOVER_VALUE;
	m_UseItemFile="UseItem.xml";
}

CEnergyRecover::CEnergyRecover(CXMLTreeNode &atts)
	: CLogicObject(), m_fTime(0.0f), m_bStart(false), m_player(NULL), m_fMaxEnergyRecover(0.0f), m_bUseActive(false), m_UseItemFile(""),
	m_OnTexture(NULL), m_OffTexture(NULL), m_Mesh(NULL), m_Light(NULL), m_orientation(0)
{
	m_Type = ENERGY_RECOVER;
	m_fMaxEnergyRecover = MAX_ENERGY_RECOVER_VALUE;
	m_UseItemFile="UseItem.xml";

	std::string l_MeshName = atts.GetPszProperty("static_mesh", "");
	std::string l_layer = atts.GetPszProperty("layer", "");
	m_orientation = atts.GetIntProperty("orientation", 0);

	if (l_layer != "" && l_MeshName != "")
	{
		m_Mesh = (CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer)->GetResource(l_MeshName);
	}
	else
	{
		std::string msg_error = "CEnergyRecover::CEnergyRecover->No se encuentra renderable object " + l_MeshName + " de la maquina de recarga de energia!";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	std::string l_TextureNameON = atts.GetPszProperty("on_texture", "");
	std::string l_TextureNameOFF = atts.GetPszProperty("off_texture", "");
	
	m_OnTexture = (CTexture*) CORE->GetTextureManager()->GetTexture(l_TextureNameON);
	m_OffTexture = (CTexture*) CORE->GetTextureManager()->GetTexture(l_TextureNameOFF);

	if (!m_OnTexture || !m_OffTexture)
	{
		std::string msg_error = "CEnergyRecover::CEnergyRecover->No se encuentra textura de la maquina de recarga de energia!";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	std::string l_LightName = atts.GetPszProperty("light", "");
	if (l_LightName != "")
	{
		m_Light = (CLight*)CORE->GetLightManager()->GetLightByName(l_LightName);

		if (m_Light == NULL)
		{
			std::string msg_error = "CEnergyRecover::CEnergyRecover->No se encuentra la luz " + l_LightName + " de la maquina de recarga de energia!";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		}
		else
			m_Light->SetEndRangeAttenuation(m_Light->GetEndRangeAttenuation()*7.5f);
	}
}

CEnergyRecover::~CEnergyRecover()
{
}

void CEnergyRecover::Update(float ElapsedTime)
{
	if (m_player == NULL)
		return;

	if (m_bStart && !m_player->GetWeapon().IsMunitionFull())
	{
		CORE->GetScriptManager()->RunCode("sound_carga_energia_on()");
		m_player->ChangeCharacterAnimation(RECARGA_ARMA_EST_ANIM, 0.3f);
		m_fTime += ElapsedTime;

		if (m_fTime >= ENERGY_INC_TIME)
		{
			m_player->GetWeapon().PartiallyRechargeWeapon();
			m_fMaxEnergyRecover -= ENERGY_INC_VALUE;
			if (m_Light != NULL){
				m_Light->SetVarIntensity(!m_player->GetWeapon().IsMunitionFull());
				m_Light->SetVarTime(LIGHT_VAR_INTENSITY);
			}
			m_fTime = 0.0f;
		}

		if (m_fMaxEnergyRecover <= 0.0f)
		{
			SetEnabled(false);
			CORE->GetScriptManager()->RunCode("sound_carga_energia_off()");

			if (m_Mesh != NULL && m_OffTexture != NULL)
				m_Mesh->GetStaticMesh()->SetTexture(m_OffTexture);

			if (m_Light != NULL){
				m_Light->SetVarIntensity(false);
				m_Light->SetVarTime(0.f);
				m_Light->SetActive(false);
				m_Light->SetEnabled(false);
			}
		}
	}
	else 
	{
		m_player->ClearCycle(RECARGA_ARMA_EST_ANIM, 0.3f);
		CORE->GetScriptManager()->RunCode("sound_carga_energia_off()");
	}
}

void CEnergyRecover::DisplayUse(bool show)
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

void CEnergyRecover::Trigger(const std::string& action, CPlayer* player)
{
	m_player = player;

	if (m_player==NULL)
		return;

	if (GetEnabled() && !m_player->GetWeapon().IsMunitionFull())
	{
		if (action=="OnEnter")
		{
			player->SetCanUseItem(true);
			player->SetUseItemAnim(RECARGA_ARMA_ANIM);
			DisplayUse(true);
		}
		else if (action=="OnLeave")
		{
			m_bStart = false;
			DisplayUse(m_bStart);
			player->SetCanUseItem(false);
			player->SetUseItemAnim(USE_ANIM);
			if (m_Light != NULL){
				m_Light->SetVarIntensity(false);
				m_Light->SetVarTime(0.f);
			}
			CORE->GetScriptManager()->RunCode("sound_carga_energia_off()");
			m_player = NULL;
		}
		else if (action=="OnTrigger")
		{
			D3DXVECTOR3 vp(-player->GetFront().x, -player->GetFront().y, -player->GetFront().z);
			D3DXVECTOR3 n(-m_Mesh->GetFront().Normalize().x, -m_Mesh->GetFront().Normalize().y, -m_Mesh->GetFront().Normalize().z);
			float dot = D3DXVec3Dot(&n, &vp);
			bool bOriented = false;
			if (m_orientation == 0)
				bOriented = (dot >= 0.80f);
			else if (m_orientation == 1)
				bOriented = (dot <= 0.05f && dot >= -0.40f);		
			else if (m_orientation == 2)
				bOriented = (dot <= 0.40f && dot >= -0.16f);			
			else if (m_orientation == 3)
				bOriented = (dot <= 1.00f && dot >= 0.90f);			
			if (bOriented){
				m_bStart = player->GetUseItem();
				DisplayUse(true);
				player->SetCanUseItem(true);
				player->SetUseItemAnim(RECARGA_ARMA_ANIM);
				if (!m_bStart && m_Light != NULL) {
					m_Light->SetVarIntensity(false);
					m_Light->SetVarTime(0.f);
				}
			}
			else
			{
				m_bStart = player->GetUseItem();
				DisplayUse(false);
				player->SetCanUseItem(false);
				player->SetUseItemAnim(RECARGA_ARMA_ANIM);
			}
		}
	}
	else 
	{
		m_bStart = false;
		DisplayUse(m_bStart);
		player->SetCanUseItem(false);
		player->SetUseItemAnim(USE_ANIM);
		if (m_Light != NULL){
			m_Light->SetVarIntensity(false);
			m_Light->SetVarTime(0.f);
		}
		CORE->GetScriptManager()->RunCode("sound_carga_energia_off()");
		m_player = NULL;
	}
}

void CEnergyRecover::Restart()
{
	m_fMaxEnergyRecover = MAX_ENERGY_RECOVER_VALUE;
	SetEnabled(true);

	if (m_Mesh != NULL && m_OnTexture != NULL)
		m_Mesh->GetStaticMesh()->SetTexture(m_OnTexture);

	if (m_Light != NULL){
		m_Light->SetVarIntensity(false);
		m_Light->SetVarTime(0.f);
		m_Light->SetActive(true);
		m_Light->SetEnabled(true);
	}
}