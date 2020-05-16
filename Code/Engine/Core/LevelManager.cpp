#include <time.h>
#include "LevelManager.h"
#include "xml/XMLTreeNode.h"
#include "StaticMeshManager.h"
#include "AnimatedModelsManager.h"
#include "LightManager.h"
#include "EffectManager.h"
#include "Scripting\ScriptManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Renderer\SceneRendererCommandManager.h"
#include "Renderer\RenderableObjectsTechniqueManager.h"
#include "BillboardManager.h"
#include "ParticleManager.h"
#include "TriggerManager.h"
#include "SoundManager.h"
#include "NodeManager.h"
#include "CoverManager.h"
#include "LogicObjectsManager.h"
#include "include\PhysicsManager.h"
#include "CProcess.h"
#include "Logger.h"
#include "Exception.h"
#include "BulletManager.h"
#include "Base.h"


CLevelManager::CLevelManager(const std::string& shadows_type)
	: m_bIsOk(false), m_bChanging(false), m_bLevelLoaded(false), m_shadowsType(shadows_type)
{
}


CLevelManager::~CLevelManager()
{
}

void CLevelManager::Initialize()
{
#if MULTITHREADED_LOAD
	// Mesh preload thread
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'A', 'H'));
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'I', 'L'));
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'M', 'O'));
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'P', 'T'));
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'U', 'Z'));
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'a', 'i'));
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'j', 'q'));
	m_preloading_meshes.push_back(CORE->GetThreadPool()->enqueue(&CStaticMeshManager::LoadFolder, CORE->GetStaticMeshManager(), "data\\Meshes", 'r', 'z'));
#endif
}

bool CLevelManager::Update()
{
	if (m_bChanging)
	{
		m_bIsOk = false;

		CORE->GetProcess()->PreInit();
		bool l_bIsOk = InitLevel();
		if (l_bIsOk)
			CORE->GetProcess()->Init();

		m_bChanging = false;
		m_bIsOk = l_bIsOk;

		return true;
	}
	else
		m_bIsOk = true;

	return false;
}

void CLevelManager::LoadPaths (const std::string& pathFile)
{
	std::string init_file = pathFile + "/" + INIT_FILE;
	CXMLTreeNode parser;
	if (!parser.LoadFile(init_file.c_str()))
	{
		std::string msg_error = "CLevelManager::LoadLevel->Error al intentar leer el archivo de inicialización del nivel " + init_file;
		LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	m_LevelPaths.m_sXMLPath = pathFile;

	// Obtenemos configuración del nivel
	CXMLTreeNode  l_LevelNode = parser["Level"];
	m_LevelPaths.m_sLevelName = l_LevelNode.GetPszProperty("name","");

	if (l_LevelNode.Exists())
	{
		// Obtenemos path del fichero con los static meshes
		CXMLTreeNode  l_StaticMeshesNode = parser["static_meshes"];	
		if (l_StaticMeshesNode.Exists())
		{
			m_LevelPaths.m_sStaticMeshesPath = l_StaticMeshesNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los animated models
		/*
		CXMLTreeNode  l_AnimatedModelsNode = parser["animated_models"];	
		if (l_AnimatedModelsNode.Exists())
		{
			m_LevelPaths.m_sAnimatedModelsPath = l_AnimatedModelsNode.GetPszProperty("file_path","");
		}
		*/

		// Obtenemos path del fichero con los renderable objects
		CXMLTreeNode  l_RenderableObjectsNode = parser["renderable_objects"];	
		if (l_RenderableObjectsNode.Exists())
		{
			m_LevelPaths.m_sRenderableObjectsPath = l_RenderableObjectsNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con las luces
		CXMLTreeNode  l_LightsNode = parser["lights"];	
		if (l_LightsNode.Exists())
		{
			m_LevelPaths.m_sLightsPath = l_LightsNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los path de ficheros lua
		CXMLTreeNode  l_LuaNode = parser["scripting"];	
		if (l_LuaNode.Exists())
		{
			m_LevelPaths.m_sLuaPath = l_LuaNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con las billboard animations
		CXMLTreeNode  l_BillboardNode = parser["billboard"];	
		if (l_BillboardNode.Exists())
		{
			m_LevelPaths.m_sBillboardPath = l_BillboardNode.GetPszProperty("file_path","");
		}
		// Obtenemos path del fichero con las particles
		CXMLTreeNode  l_ParticlesNode = parser["particles"];	
		if (l_ParticlesNode.Exists())
		{
			m_LevelPaths.m_sParticlesPath = l_ParticlesNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los triggers
		CXMLTreeNode  l_TriggersNode = parser["triggers"];	
		if (l_ParticlesNode.Exists())
		{
			m_LevelPaths.m_sTriggersPath = l_TriggersNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los nodes
		CXMLTreeNode  l_NodeNode = parser["nodes"];	
		if (l_NodeNode.Exists())
		{
			m_LevelPaths.m_sNodesPath = l_NodeNode.GetPszProperty("file_path","");
		}

		// Obtenemos logic object path
		CXMLTreeNode  l_LogicObjectsNode = parser["logic_objects"];	
		if (l_LogicObjectsNode.Exists())
		{
			m_LevelPaths.m_sLogicObjectsPath = l_LogicObjectsNode.GetPszProperty("file_path","");
		}

		// Obtenemos path del fichero con los covers
		CXMLTreeNode  l_CoverNode = parser["covers"];	
		if (l_CoverNode.Exists())
		{
			m_LevelPaths.m_sCoversPath = l_CoverNode.GetPszProperty("file_path","");
		}
	}
}

bool CLevelManager::InitLevel () 
{
	m_bLevelLoaded = false;
	time_t start_load, end_load;
	time(&start_load);

	LoadPaths(m_LevelPaths.m_sXMLPath);

	std::string xml_file = m_LevelPaths.m_sXMLPath + "/";
	std::string l_ManagerError="";
	bool bInitOk = true;

	// Get Managers
	CStaticMeshManager* l_StaticMeshManager = CORE->GetStaticMeshManager();
	CRenderableObjectsLayersManager* l_RenderableObjectsLayersManager = CORE->GetRenderableObjectsLayersManager();
	CBulletManager* l_BulletManager = CORE->GetBulletManager();
	CLightManager* l_LightManager = CORE->GetLightManager();
	CBillboardManager* l_BillboardManager = CORE->GetBillboardManager();
	CParticleManager* l_ParticleManager = CORE->GetParticleManager();
	CTriggerManager* l_TriggerManager = CORE->GetTriggerManager();
	CNodeManager* l_NodeManager = CORE->GetNodeManager();
	CLogicObjectsManager* l_LogicObjectsManager = CORE->GetLogicObjectsManager();
	CCoverManager* l_CoverManager = CORE->GetCoverManager();

	// Destroy all
	if (!l_StaticMeshManager->GetMeshPreLoad())
		l_StaticMeshManager->Destroy();
	l_RenderableObjectsLayersManager->Destroy();
	l_TriggerManager->Destroy();
	l_BulletManager->Destroy();
	l_LightManager->Destroy();
	l_BillboardManager->Destroy();
	l_ParticleManager->Destroy();
	l_NodeManager->Release();
	l_LogicObjectsManager->Destroy();
	l_CoverManager->Release();

	//Inicializamos ParticleManager
	bInitOk = (m_LevelPaths.m_sParticlesPath != "");
	if (bInitOk)
	{
		l_ParticleManager->Load(xml_file + m_LevelPaths.m_sParticlesPath);
	}
	if (!bInitOk && l_ManagerError == "")
		l_ManagerError = "ParticleManager";

	//Inicializamos BillboardManager
	bInitOk = (m_LevelPaths.m_sBillboardPath != "");
	if (bInitOk)
	{
		l_BillboardManager->Load(xml_file + m_LevelPaths.m_sBillboardPath);
	}
	if (!bInitOk && l_ManagerError == "")
		l_ManagerError = "BillboardManager";

	//Inicializamos TriggerManager
	bInitOk = (m_LevelPaths.m_sTriggersPath != "");
	if (bInitOk)
	{
		l_TriggerManager->Init();
		l_TriggerManager->Load(xml_file + m_LevelPaths.m_sTriggersPath);
	}
	if (!bInitOk && l_ManagerError == "")
		l_ManagerError = "TriggerManager";

	// Inicializamos NodeManager
	bInitOk = (m_LevelPaths.m_sNodesPath != "");
	if (bInitOk)
	{
		l_NodeManager->Load(xml_file + m_LevelPaths.m_sNodesPath);
	}
	if (!bInitOk && l_ManagerError == "")
		l_ManagerError = "NodeManager";

	//Inicializamos BulletManager
	bInitOk = l_BulletManager->Init();
	if (!bInitOk && l_ManagerError == "")
		l_ManagerError = "BulletManager";

	// Inicializamos CoverManager
	bInitOk = (m_LevelPaths.m_sCoversPath != "");
	if (bInitOk)
	{
		l_CoverManager->Load(xml_file + m_LevelPaths.m_sCoversPath);
	}
	if (!bInitOk && l_ManagerError == "")
		l_ManagerError = "CoverManager";

#if MULTITHREADED_LOAD
	// Wait for loading thread to finish loading of all game meshes
	for (auto& i : m_preloading_meshes)
		if (i.valid())
			i.get();
#endif

	// Inicializamos StaticMeshManager 
	bInitOk = (m_LevelPaths.m_sStaticMeshesPath!="");
	if (bInitOk)
	{
		bInitOk = (l_StaticMeshManager->GetMeshPreLoad() ? true : l_StaticMeshManager->Load(xml_file+m_LevelPaths.m_sStaticMeshesPath));
	}
	if (!bInitOk && l_ManagerError=="")
		l_ManagerError = "StaticMeshManager";
	
	// Inicializamos RenderableObjectsLayersManager
	bInitOk = (m_LevelPaths.m_sRenderableObjectsPath!="");	
	if (bInitOk)
	{
		l_RenderableObjectsLayersManager->Load(xml_file+m_LevelPaths.m_sRenderableObjectsPath);
		bInitOk = (CORE->GetRenderableObjectsLayersManager()->GetResourcesVector().size() > 0);
	}
	if (!bInitOk && l_ManagerError=="")
		l_ManagerError = "RenderableObjectsLayersManager";

	// Inicializamos LightManager
	bInitOk = (m_LevelPaths.m_sLightsPath!="");
	if (bInitOk)
	{
		l_LightManager->Load(xml_file+m_LevelPaths.m_sLightsPath, m_shadowsType);
	}
	if (!bInitOk && l_ManagerError=="")
		l_ManagerError = "LightManager";

	// Inicializamos LogicObjectsManager
	bInitOk = (m_LevelPaths.m_sLogicObjectsPath!="");
	if (bInitOk)
	{
		l_LogicObjectsManager->Load(xml_file+m_LevelPaths.m_sLogicObjectsPath);
	}
	if (!bInitOk && l_ManagerError=="")
		l_ManagerError = "LogicObjectsManager";

	// Recargamos SceneRendererCommandManager
	CORE->GetSceneRendererCommandManager()->Reload();

	// Comprobamos que todos los managers han sido inicializados correctamente y si no lanzamos una excepción.
	if (bInitOk)
    {
		std::ostringstream oss;
		time(&end_load);
		double dif = difftime(end_load, start_load);
		oss << "CLevelManager::InitLevel->Datos de todos los managers cargados ok!Tiempo total : " << dif;
        LOGGER->AddNewLog(ELOG_LEVEL::ELL_INFORMATION, oss.str().c_str());
    }
    else if (l_ManagerError!="")
    {
		std::string msg_error = "CLevelManager::InitLevel-> Nivel " + m_LevelPaths.m_sLevelName + " cargado satisfactoriamente!\n";
        LOGGER->AddNewLog(ELOG_LEVEL::ELL_ERROR, msg_error.c_str());
        throw CException(__FILE__, __LINE__, msg_error);
	}

	m_bLevelLoaded = bInitOk;

	return bInitOk;
}

void CLevelManager::LoadLevel (const std::string& pathFile)
{
	std::string oldPath = m_LevelPaths.m_sXMLPath;

	if (oldPath!=pathFile)
	{
		m_LevelPaths.m_sXMLPath = pathFile;
		m_bChanging = true;
	}
}

void CLevelManager::ReloadLevel (const std::string& pathFile)
{
	m_LevelPaths.m_sXMLPath = pathFile;
	m_bChanging = true;
}

const std::string& CLevelManager::GetLevelName() 
{ 
	return m_LevelPaths.m_sLevelName; 
}