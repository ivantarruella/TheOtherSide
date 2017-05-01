#include "LifeRecover.h"
#include "Player.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "LightManager.h"
#include "Scripting\ScriptManager.h"
#include "Base.h"

#define LIFE_INC_TIME	0.05f	// incrementa vida cada x seg.
#define LIFE_INC_VALUE	(m_player->GetMaxPlayerLife()/100.0f)	// cuanto incrementa de vida

#define MAX_LIFE_RECOVER_VALUE	(m_player->GetMaxPlayerLife()*0.75f)

#define LIGHT_VAR_INTENSITY		0.1f

CLifeRecover::CLifeRecover()
	: CLogicObject(), m_fTime(0.0f), m_bStart(false), m_player(NULL), m_fMaxLifeRecover(0.0f), m_EmitterMaxRate(0.0f), m_OnTexture(NULL), m_OffTexture(NULL), m_Mesh(NULL), m_Light(NULL)
{
	m_Type = LIFE_RECOVER;
	m_fMaxLifeRecover = MAX_LIFE_RECOVER_VALUE;
}

CLifeRecover::CLifeRecover(CXMLTreeNode &atts)
	: CLogicObject(), m_fTime(0.0f), m_bStart(false), m_player(NULL), m_fMaxLifeRecover(0.0f), m_EmitterMaxRate(0.0f), m_OnTexture(NULL), m_OffTexture(NULL), m_Mesh(NULL), m_Light(NULL)
{
	m_Type = LIFE_RECOVER;
	m_fMaxLifeRecover = MAX_LIFE_RECOVER_VALUE;

	std::string l_MeshName = atts.GetPszProperty("static_mesh", "");
	std::string l_layer = atts.GetPszProperty("layer", "");

	if (l_layer != "" && l_MeshName != "")
	{
		m_Mesh = (CInstanceMesh*)CORE->GetRenderableObjectsLayersManager()->GetResource(l_layer)->GetResource(l_MeshName);
	}
	else
	{
		std::string msg_error = "CLifeRecover::CLifeRecover->No se encuentra renderable object " + l_MeshName + " de la maquina de recarga de vida!";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	std::string l_TextureNameON = atts.GetPszProperty("on_texture", "");
	std::string l_TextureNameOFF = atts.GetPszProperty("off_texture", "");
	std::string l_ParticleEmitter = atts.GetPszProperty("particle_emitter", "");

	m_OnTexture = (CTexture*) CORE->GetTextureManager()->GetTexture(l_TextureNameON);
	m_OffTexture = (CTexture*) CORE->GetTextureManager()->GetTexture(l_TextureNameOFF);
	m_Emitter = (CParticleEmitter*) CORE->GetParticleManager()->GetParticleEmitter(l_ParticleEmitter);

	if (!m_OnTexture || !m_OffTexture)
	{
		std::string msg_error = "CLifeRecover::CLifeRecover->No se encuentra textura de la maquina de recarga de vida!";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}

	std::string l_LightName = atts.GetPszProperty("light", "");
	if (l_LightName != "")
	{
		m_Light = (CLight*)CORE->GetLightManager()->GetLightByName(l_LightName);

		if (m_Light == NULL)
		{
			std::string msg_error = "CLifeRecover::CLifeRecover->No se encuentra la luz " + l_LightName + " de la maquina de recarga de vida!";
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		}
	}
}

CLifeRecover::~CLifeRecover()
{
}

void CLifeRecover::Update(float ElapsedTime)
{
	if (m_player == NULL)
		return;

	if (m_bStart)
	{
		if (m_player->GetLife()<(m_player->GetMaxPlayerLife()))
		{
			CORE->GetScriptManager()->RunCode("sound_carga_vida_on()");
			m_fTime += ElapsedTime;

			if (m_fTime >= LIFE_INC_TIME)
			{
				m_player->PartiallyRecoverLife(LIFE_INC_VALUE);
				m_fMaxLifeRecover -= LIFE_INC_VALUE;
				if (m_Light != NULL){
					m_Light->SetVarIntensity(m_player->GetLife()<(m_player->GetMaxPlayerLife()));
					m_Light->SetVarTime(LIGHT_VAR_INTENSITY);
					m_Emitter->SetMaxRate(m_EmitterMaxRate*10.0f);
				}
				m_fTime = 0.0f;
			}

			if (m_fMaxLifeRecover <= 0.0f)
			{
				SetEnabled(false);
				CORE->GetScriptManager()->RunCode("sound_carga_vida_off()");

				if (m_Mesh != NULL && m_OffTexture != NULL)
					m_Mesh->GetStaticMesh()->SetTexture(m_OffTexture);

				if (m_Light != NULL)
				{
					m_Light->SetVarIntensity(false);
					m_Light->SetVarTime(0.f);
					//m_Light->SetEnabled(false);
					m_Light->SetActive(false);
				}

				if (m_Emitter != NULL) {
					m_Emitter->SetEnabled(false);
				}
			}
		}
		else {
			CORE->GetScriptManager()->RunCode("sound_carga_vida_off()");
			if (m_Emitter  != NULL)
				m_Emitter->SetMaxRate(m_EmitterMaxRate);
		}
	}
}

void CLifeRecover::Trigger(const std::string& action, CPlayer* player)
{
	if (GetEnabled())
	{
		if (action=="OnEnter")
		{
			m_player = player;
			m_bStart = true;
			m_EmitterMaxRate=m_Emitter->GetMaxRate();
		}
		else if (action=="OnLeave")
		{
			m_bStart = false;
			if (m_Light != NULL){
				m_Light->SetVarIntensity(false);
				m_Light->SetVarTime(0.f);
			}
			if (m_Emitter  != NULL)
				m_Emitter->SetMaxRate(m_EmitterMaxRate);

			CORE->GetScriptManager()->RunCode("sound_carga_vida_off()");
		}
		else if (action=="OnTrigger")
		{
			m_player = player;
			m_bStart = true;
		}
	}
	else 
	{
		m_bStart = false;
		if (m_Light != NULL){
			m_Light->SetVarIntensity(false);
			m_Light->SetVarTime(0.f);
			//m_Light->SetEnabled(false);
			m_Light->SetActive(false);
		}
	}
}

void CLifeRecover::Restart()
{
	m_fMaxLifeRecover = MAX_LIFE_RECOVER_VALUE;
	SetEnabled(true);

	if (m_Mesh != NULL && m_OnTexture != NULL)
		m_Mesh->GetStaticMesh()->SetTexture(m_OnTexture);

	if (m_Light != NULL){
		m_Light->SetVarIntensity(false);
		m_Light->SetVarTime(0.f);
		//m_Light->SetEnabled(true);
		m_Light->SetActive(true);
	}

	if (m_Emitter != NULL)
		m_Emitter->SetEnabled(true);
}