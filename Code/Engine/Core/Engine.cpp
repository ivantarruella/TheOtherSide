#include <stdio.h>
#include "Engine.h"
#include "Process.h"
#include "Core.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "LanguageManager.h"
#include "xml/XMLTreeNode.h"
#include "ActionToInput.h"
#include "EffectManager.h"
#include "TextureManager.h"
#include "LightManager.h"
#include "Renderer\SceneRendererCommandManager.h"
#include "Renderer\RenderableObjectsTechniqueManager.h"
#include "StaticMeshManager.h"
#include "Scripting\ScriptManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "AnimatedModelsManager.h"
#include "GUIManager.h"
#include "Logger.h"
#include "Exception.h"
#include "Console.h"
#include "Base.h"

#include <sstream>

void CEngine::Release()
{
	CHECKED_DELETE(m_pCore);
}

bool CEngine::Init(HWND hWnd)
{
	m_pCore = new CCore();
	m_bIsOk = m_pCore->Init(hWnd, m_pProcess, m_InitParams);
	/* la incializacion del proceso se ejecuta ahora desde el script de carga de nivel
	if (m_bIsOk)
	{
		m_bIsOk = m_pProcess->Init();
	}
	*/

	return m_bIsOk;
}

void CEngine::Update()
{
	float elapsedTime = m_pCore->GetElapsedTime();

	m_pCore->Update(m_InitParams.show_fps);

#ifdef _DEBUG
	bool l_Update = !m_pCore->GetLogRender()->GetVisible() && !m_pCore->GetConsole()->IsActive() && m_pProcess->GetStartGame();
#else
	bool l_Update = m_pProcess->GetStartGame();
#endif

	if (l_Update)
	{
		m_pProcess->Update(elapsedTime);
	}

#ifdef _DEBUG	
	m_pCore->GetLogRender()->Update(elapsedTime);
	m_pCore->GetConsole()->Update(elapsedTime);
#endif
	m_pCore->GetGUIManager()->Update(elapsedTime);

	UpdateInputActions();
}

void CEngine::UpdateInputActions()
{
#ifdef _DEBUG	
	CActionToInput* pActionToInput = m_pCore->GetActionToInput();

	if( pActionToInput->DoAction(DOACTION_RELOADALL) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_ALL()");
	}

	if( pActionToInput->DoAction(DOACTION_RELOADLANGUAGES) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_languages()");
		ReLoadDefaultLanguage();
	}

	if( pActionToInput->DoAction(DOACTION_RELOADACTIONS) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_actions()");
	}

	if( pActionToInput->DoAction(DOACTION_RELOADLIGHTS) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_lights()");
	}

	if( pActionToInput->DoAction(DOACTION_RELOAD_EFFECTS) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_effects()");
	}

	if( pActionToInput->DoAction(DOACTION_RELOAD_TEXTURES) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_textures()");
	}

	if( pActionToInput->DoAction(DOACTION_RELOAD_SCENERENDERERCOMMANDS) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_scene_renderer_commands()");
	}

	if( pActionToInput->DoAction(DOACTION_RELOAD_SCRIPTING) )
	{
		m_pCore->GetScriptManager()->RunCode("reload_logic_objects()");
		m_pCore->GetScriptManager()->RunCode("reload_scripts()");
	}

	if( pActionToInput->DoAction(DOACTION_LOGGER) )
	{
		m_pCore->GetLogRender()->SetVisible(!m_pCore->GetLogRender()->GetVisible());
	}

	if( pActionToInput->DoAction(DOACTION_CONSOLE) )
	{
		m_pCore->GetConsole()->SetActive(!m_pCore->GetConsole()->IsActive());
	}

	if( pActionToInput->DoAction(DOACTION_LOGRENDERPAGEDOWN) )
	{
		m_pCore->GetLogRender()->PageDown();
	}

	if( pActionToInput->DoAction(DOACTION_LOGRENDERPAGEUP) )
	{
		m_pCore->GetLogRender()->PageUp();
	}

	if( pActionToInput->DoAction(DOACTION_LOGRENDERPREVLINE) )
	{
		m_pCore->GetLogRender()->PrevLine();
	}

	if( pActionToInput->DoAction(DOACTION_LOGRENDERNEXTLINE) )
	{
		m_pCore->GetLogRender()->NextLine();
	}
#endif
}

void CEngine::Render()
{
	DWORD nFrameStartTime = GetTickCount();

	//---Render the scene---
	CRenderManager* renderManager = m_pCore->GetRenderManager();
	m_pProcess->RenderScene(renderManager);

	// FPS limit
	if (m_InitParams.max_fps > 0)	{
		while (GetTickCount()-nFrameStartTime < 1000/m_InitParams.max_fps) 
			Sleep(0); 
	}
}

bool CEngine::LoadConfig (const std::string& pathFile)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(pathFile.c_str()))
	{
		std::string msg_error = "CEngine::LoadConfig->Error al intentar leer el archivo de configuracion del Engine: " + pathFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	m_sPathFile = pathFile;

	// Obtenemos configuración del Engine
	CXMLTreeNode  l_EngineNode = parser["Engine"];
	if (l_EngineNode.Exists())
	{
		// Obtenemos configuración de pantalla
		CXMLTreeNode  l_ScreenNode = parser["screen"];	
		if (l_ScreenNode.Exists())
		{
			m_InitParams.fullscreen	= l_ScreenNode.GetBoolProperty("fullscreen",false);
			m_InitParams.show_fps	= l_ScreenNode.GetBoolProperty("show_fps",false);
			m_InitParams.vsync	= l_ScreenNode.GetBoolProperty("vsync",false);
			m_InitParams.exclusiveModeinMouse	= l_ScreenNode.GetBoolProperty("exclusiveModeinMouse",false);
			m_InitParams.x	= l_ScreenNode.GetIntProperty("x_ini",0);
			m_InitParams.y	= l_ScreenNode.GetIntProperty("y_ini",0);
			m_InitParams.w	= l_ScreenNode.GetIntProperty("nWidth",640);
			m_InitParams.h	= l_ScreenNode.GetIntProperty("nHeight",480);
			m_InitParams.max_fps = l_ScreenNode.GetIntProperty("max_fps",0);
			m_InitParams.shadows_type = l_ScreenNode.GetPszProperty("shadows_type", "PREDEFINED");
		}

		// Obtenemos configuración de las fuentes
		CXMLTreeNode  l_FontsNode = parser["fonts"];	
		if (l_FontsNode.Exists())
		{
			m_InitParams.fonts_path	= l_FontsNode.GetPszProperty("file_path","");
		}

		// Obtenemos diferentes idiomas, idioma por defecto, etc.
		CXMLTreeNode  l_LanguagesNode = parser["languages"];	
		if (l_LanguagesNode.Exists())
		{
			m_InitParams.language_def	= l_LanguagesNode.GetPszProperty("default_lang","english");
			int l_count = l_LanguagesNode.GetNumChildren();

			for (int i = 0; i < l_count; ++i)
			{
				m_InitParams.language_desc[ l_LanguagesNode(i).GetPszProperty("id","") ] = l_LanguagesNode(i).GetPszProperty("file_path","");
			}
		}

		// Obtenemos path del fichero de configuración para ActionToInput
		CXMLTreeNode  l_Action2InputNode = parser["action2input"];	
		if (l_Action2InputNode.Exists())
		{
			m_InitParams.actions_path = l_Action2InputNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con el GUI
		CXMLTreeNode  l_GUINode = parser["GUI"];	
		if (l_GUINode.Exists())
		{
			m_InitParams.gui_path = l_GUINode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los sounds
		CXMLTreeNode  l_SoundNode = parser["sound"];	
		if (l_SoundNode.Exists())
		{
			m_InitParams.sounds_path = l_SoundNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los path de ficheros lua
		CXMLTreeNode  l_LuaNode = parser["scripting"];	
		if (l_LuaNode.Exists())
		{
			m_InitParams.lua_path = l_LuaNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los efectos
		CXMLTreeNode  l_EffectsNode = parser["effects"];	
		if (l_EffectsNode.Exists())
		{
			m_InitParams.effects_path = l_EffectsNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los renderable object techniques
		CXMLTreeNode  l_RenderableObjectTechniquesNode = parser["renderable_object_techniques"];	
		if (l_RenderableObjectTechniquesNode.Exists())
		{
			m_InitParams.ROT_path = l_RenderableObjectTechniquesNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los animated models
		CXMLTreeNode  l_AnimatedModelsNode = parser["animated_models"];	
		if (l_AnimatedModelsNode.Exists())
		{
			m_InitParams.animatedmodels_path = l_AnimatedModelsNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los scene renderer commands
		CXMLTreeNode  l_SceneRendererCommandsNode = parser["scene_renderer_commands"];	
		if (l_SceneRendererCommandsNode.Exists())
		{
			m_InitParams.scenerenderercommands_path = l_SceneRendererCommandsNode.GetPszProperty("file_path","");
		}
	}

	return true;
}

bool CEngine::ReLoadDefaultLanguage()
{
	bool bIsOk=false;

	CXMLTreeNode parser;
	if (!parser.LoadFile(m_sPathFile.c_str()))
	{
		std::string msg_error = "CEngine::LoadConfig->Error al intentar leer el archivo de configuracion del Engine: " + m_sPathFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	CXMLTreeNode  l_EngineNode = parser["Engine"];
	if (l_EngineNode.Exists())
	{
		// Obtenemos default language
		CXMLTreeNode  l_LanguagesNode = parser["languages"];	
		if (l_LanguagesNode.Exists())
		{
			m_InitParams.language_def	= l_LanguagesNode.GetPszProperty("default_lang","english");
			m_pCore->GetLanguageManager()->SetCurrentLanguage(m_InitParams.language_def);
			bIsOk=true;
		}
	}

	return bIsOk;
}

bool CEngine::ReLoadConfig ()
{
	if (m_sPathFile != "")
	{
		return LoadConfig (m_sPathFile);
	}

	return false;
}


