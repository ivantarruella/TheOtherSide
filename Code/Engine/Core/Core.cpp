#include <Windows.h>
#include <stdio.h>
#include "Core.h"
#include "Process.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "LanguageManager.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "ActionToInput.h"	
#include "StaticMeshManager.h"
#include "AnimatedModelsManager.h"
#include "LightManager.h"
#include "EffectManager.h"
#include "Scripting\ScriptManager.h"
#include "include\PhysicsManager.h"
#include "Logger.h"
#include "Exception.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Renderer\SceneRendererCommandManager.h"
#include "Renderer\RenderableObjectsTechniqueManager.h"
#include "BillboardManager.h"
#include "ParticleManager.h"
#include "GUIManager.h"
#include "TriggerManager.h"
#include "SoundManager.h"
#include "NodeManager.h"
#include "CoverManager.h"
#include "LogicObjectsManager.h"
#include "LevelManager.h"
#include "LogRender.h"
#include "Random.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include <ctime>
#include "Console.h"
#include "Base.h"


void CCore::Done()
{
	if (m_bIsOk)
	{
		Release();
		m_bIsOk=false;
	}
}

void CCore::Release()
{
#ifdef _DEBUG
	CHECKED_DELETE(m_pConsole);
	CHECKED_DELETE(m_pLogRender);
#endif
	CHECKED_DELETE(m_pNodeManager);
	CHECKED_DELETE(m_pCoverManager);
	CHECKED_DELETE(m_pEnemyManager);
	CHECKED_DELETE(m_pTimer);
	CHECKED_DELETE(m_pRenderManager);
	CHECKED_DELETE(m_pInputManager);
	CHECKED_DELETE(m_pActionToInput);
	CHECKED_DELETE(m_pFontManager);
	CHECKED_DELETE(m_pLanguageManager);
	CHECKED_DELETE(m_pTextureManager);
	CHECKED_DELETE(m_pStaticMeshManager);
	CHECKED_DELETE(m_pAnimatedModelsManager);
	CHECKED_DELETE(m_pLightManager);
	CHECKED_DELETE(m_pEffectManager);
	CHECKED_DELETE(m_pLogicObjectsManager);
	CHECKED_DELETE(m_pRenderableObjectsLayersManager);
	CHECKED_DELETE(m_pSceneRendererCommandManager);
	CHECKED_DELETE(m_pRenderableObjectsTechniqueManager);
	CHECKED_DELETE(m_pBillboardManager);
	CHECKED_DELETE(m_pParticleManager);
	CHECKED_DELETE(m_pTriggerManager);
	CHECKED_DELETE(m_pSoundManager);
	CHECKED_DELETE(m_pRandom);
	CHECKED_DELETE(m_pScriptManager);
	CHECKED_DELETE(m_pGUIManager);
	CHECKED_DELETE(m_pLevelManager);
	CHECKED_DELETE(m_pBulletManager);
	CHECKED_DELETE(m_pPhysicsManager);
}

bool CCore::Init(HWND hWnd, CProcess* Process, const SInitParams& params)
{
	m_pProcess = Process;

#ifdef _DEBUG
	m_pConsole = new CConsole();
	m_pLogRender = new CLogRender();
#endif

	m_pTimer = new CTimer(30);
	m_pRenderManager = new CRenderManager();
	m_pInputManager = new CInputManager();
	m_pActionToInput = new CActionToInput();
	m_pFontManager = new CFontManager();
	m_pLanguageManager = new CLanguageManager();
	m_pTextureManager = new CTextureManager();
	m_pLightManager = new CLightManager();
	m_pEffectManager = new CEffectManager();
	m_pRenderableObjectsTechniqueManager = new CRenderableObjectsTechniqueManager();
	m_pStaticMeshManager = new CStaticMeshManager();
	m_pAnimatedModelsManager = new CAnimatedModelsManager;
	m_pRenderableObjectsLayersManager = new CRenderableObjectsLayersManager();
	m_pSceneRendererCommandManager = new CSceneRendererCommandManager();
	m_pScriptManager = new CScriptManager();
	m_pPhysicsManager = new CPhysicsManager();
	m_pBillboardManager = new CBillboardManager();
	m_pParticleManager = new CParticleManager();
	m_pSoundManager = new CSoundManager();
	m_pNodeManager = new CNodeManager();
	m_pCoverManager = new CCoverManager();
	m_pTriggerManager = new CTriggerManager();
	m_pLogicObjectsManager = new CLogicObjectsManager();
	m_pLevelManager = new CLevelManager();
	m_pEnemyManager = new CEnemyManager();
	m_pBulletManager = new CBulletManager();

	std::string l_ManagerError="";

	// Inicializamos Random
	unsigned int seed = (unsigned int)time(NULL);
	m_pRandom = new CRandom(seed);

#ifdef _DEBUG	
	// Inicializamos Console
	m_bIsOk = m_pConsole->Init();

	// Inicializamos LogRender
	m_pLogRender->SetLinePerPage(10);
#endif

	// Inicializamos RenderManager
	if (m_bIsOk)
	{	
		m_bIsOk = m_pRenderManager->Init(hWnd, params);
		if (!m_bIsOk)
			l_ManagerError = "RenderManager";
	}

	// Inicializamos GUIManager
	if (m_bIsOk) 
	{
		uint32 w,h;
		m_pRenderManager->GetWidthAndHeight(w,h);
		m_pGUIManager = new CGUIManager(Vect2i(w,h));
		m_bIsOk = m_pGUIManager->Init(params.gui_path);
		if (!m_bIsOk)
			l_ManagerError = "GUIManager";
	}

	// Inicializamos InputManager
	if (m_bIsOk)
	{		
		m_bIsOk = m_pInputManager->Init(hWnd, Vect2i (params.w, params.h), params.exclusiveModeinMouse); 
		if (!m_bIsOk)
			l_ManagerError = "InputManager";
	}

	// Inicializamos ActionToInput
	if (m_bIsOk)
	{	
		m_bIsOk = m_pActionToInput->Init(m_pInputManager);
		if (!m_bIsOk)
			l_ManagerError = "ActionToInput";
		else
			m_pActionToInput->LoadXML(params.actions_path);		
	}

	// Inicializamos FontManager y cargamos las fuentes del xml
	if (m_bIsOk)
	{
		m_bIsOk =  m_pFontManager->Init(m_pRenderManager); 
		if (!m_bIsOk)
			l_ManagerError = "FontManager";
		else		
			m_pFontManager->LoadTTFs(params.fonts_path);	
	}

	// Inicializamos LanguageManager y cargamos todos los idiomas
	if (m_bIsOk)
	{	
		std::map<std::string, std::string>::const_iterator itLang=params.language_desc.begin();
		std::map<std::string, std::string>::const_iterator itLangEnd = params.language_desc.end();
		for(; itLang != itLangEnd; ++itLang)
		{
			m_pLanguageManager->SetXmlFile(itLang->second);		
		}	
		m_pLanguageManager->LoadXMLs();
		m_pLanguageManager->SetCurrentLanguage(params.language_def);
	}

	// Inicializamos SoundManager
	if (m_bIsOk)
	{
		m_bIsOk=m_pSoundManager->Init();
		if (!m_bIsOk)
			l_ManagerError = "SoundManager";
		else	
			m_bIsOk = m_pSoundManager->LoadSounds(params.sounds_path);
	}

	// Inicializamos ScriptManager
	if (m_bIsOk)
	{
		m_pScriptManager->Initialize();
		if (m_pScriptManager->IsOk())
			m_pScriptManager->Load(params.lua_path);
	}

	// Inicializamos EffectManager
	m_bIsOk = (params.effects_path!="");
	if (m_bIsOk)
	{
		m_pEffectManager->Load(params.effects_path);
	}
	if (!m_bIsOk && l_ManagerError=="")
		l_ManagerError = "EffectManager";

	// Inicializamos CRenderableObjectTechniqueManager
	m_bIsOk = (params.ROT_path!="");
	if (m_bIsOk)
	{
		m_pRenderableObjectsTechniqueManager->Load(params.ROT_path);
	}
	if (!m_bIsOk && l_ManagerError=="")
		l_ManagerError = "RenderableObjectTechniqueManager";

	// Inicializamos AnimatedModelsManager
	m_bIsOk = (params.animatedmodels_path!="");
	if (m_bIsOk)
	{
		m_bIsOk = m_pAnimatedModelsManager->Load(params.animatedmodels_path);
	}
	if (!m_bIsOk && l_ManagerError=="")
		l_ManagerError = "AnimatedModelsManager";

	// Inicializamos PhysicManager
	if (m_bIsOk)
	{
		m_bIsOk = m_pPhysicsManager->Init();
		if (!m_bIsOk && l_ManagerError=="")
			l_ManagerError = "PhysicManager";
	}	

	// Inicializamos SceneRendererCommandManager
	if (m_bIsOk)
	{
		m_pSceneRendererCommandManager->Load(params.scenerenderercommands_path);
	}

	// Comprobamos que todos los managers han sido inicializados correctamente y si no lanzamos una excepción.
	if (m_bIsOk)
    {
        std::string msg_Ok = "CCore::Init-> Todos los managers inicializados correctamente" ;
        LOGGER->AddNewLog(ELL_INFORMATION, msg_Ok.c_str());
    }
    else
    {
		Release();
		std::string msg_error = "CCore::Init-> Error al inicializar " + l_ManagerError + " !\n";
        LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
        throw CException(__FILE__, __LINE__, msg_error);
	}

	return m_bIsOk;
}

void CCore::Update(bool show_fps)
{
	m_pTimer->Update();

	float elapsedTime = GetElapsedTime();
	
	m_pInputManager->Update();

	if (!m_pLevelManager->Update())
	{
	#ifdef _DEBUG
		bool l_Update = m_pProcess->GetStartGame() && m_pLevelManager->IsOk() && !m_pLogRender->GetVisible() && !m_pConsole->IsActive();
	#else
		bool l_Update = m_pProcess->GetStartGame() && m_pLevelManager->IsOk();
	#endif
		if (l_Update)
		{
			//m_pPhysicsManager->WaitForSimulation();	//???
			m_pEnemyManager->Update(elapsedTime);
			m_pLogicObjectsManager->Update(elapsedTime);
			m_pLightManager->Update(elapsedTime);
			m_pParticleManager->Update(elapsedTime);
			m_pBulletManager->Update(elapsedTime);
			m_pRenderableObjectsLayersManager->Update(elapsedTime);
			m_pPhysicsManager->Update(elapsedTime);
			SetCamera(m_pProcess->GetCamera());
		}
	}

	m_pSoundManager->Update(elapsedTime);
	UpdateFPS(elapsedTime, show_fps);
}


void CCore::UpdateFPS(float elapsedTime, bool show_fps)
{
#ifdef _DEBUG
	m_bShowFPS = show_fps && m_pProcess->GetStartGame() && !m_pLogRender->GetVisible() && !m_pConsole->IsActive();
#else
	m_bShowFPS = show_fps && m_pProcess->GetStartGame();
#endif

	if (m_bShowFPS)
	{
		m_fTime += 10 * elapsedTime;			
		if (m_fTime >= 1.0f)
		{	
			m_fFPS = m_pTimer->GetFPS();
			m_fTime = 0.0f;
		}
	}
}

void CCore::DrawFPS()
{
	if (m_bShowFPS)
	{
		std::ostringstream buff;
		buff<<(int)m_fFPS;
		uint32 l_font1 = GetFontManager()->GetTTF_Id("arial");
		uint32 w,h;
		GetRenderManager()->GetWidthAndHeight(w, h);
		CColor col = colGREEN;
		if (m_fFPS <= 25.0f)
			col = colRED;
		else if (m_fFPS > 25.0f && m_fFPS <= 50.0f)
			col = colYELLOW;

		GetFontManager()->DrawText( w - 25, 0, col, l_font1, buff.str().c_str() );
	}
}