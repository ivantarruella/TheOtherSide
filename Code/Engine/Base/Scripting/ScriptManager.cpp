#define __DONT_INCLUDE_MEM_LEAKS__
#include "Process.h"
#include "Scripting\ScriptManager.h"
#include "luabind\luabind.hpp"
#include "Logger.h"
#include "Exception.h"
#include "XML\XMLTreeNode.h"
#include "Core.h"

#include "GUIManager.h"
#include "EffectManager.h"
#include "LanguageManager.h"
#include "FontManager.h"
#include "SoundManager.h"
#include "ParticleManager.h"
#include "ActionToInput.h"
#include "TextureManager.h"
#include "StaticMeshManager.h"
#include "AnimatedModelsManager.h"
#include "LightManager.h"
#include "TriggerManager.h"
#include "LogicObjectsManager.h"
#include "PhysicsManager.h"
#include "Renderer\RenderableObjectsTechniqueManager.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "Renderer\SceneRendererCommandManager.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "BillboardManager.h"
#include "Object3D.h"
#include "Base.h"


using namespace luabind;

#define REGISTER_LUA_FUNCTION(LUA_STATE, FunctionName,AddrFunction){luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}

//Código de la función Alert que se llamará al generarse algún error de LUA
int Alert(lua_State * State)
{
	std::string l_Text;
	int n = lua_gettop(State);
	int i;
	lua_getglobal(State, "tostring");
	for (i=1; i<=n; i++) {
		const char *s;
		lua_pushvalue(State, -1);
		lua_pushvalue(State, i);
		lua_call(State, 1, 1);
		s = lua_tostring(State, -1);
		if (s == NULL) 
			return luaL_error(State, "`tostring' must return a string to `print'");
		if (i>1) l_Text += '\t';
		l_Text += s;
		lua_pop(State, 1); 
	}
	l_Text += '\n';
	LOGGER->AddNewLog(ELL_ERROR, l_Text.c_str());

	return true;
}

//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
	m_LS=luaL_newstate();
	luaL_openlibs(m_LS);
	//Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA
	lua_register(m_LS,"_ALERT",Alert);
	luabind::open(m_LS);
	RegisterLUAFunctions();
	m_bIsOk = true;
}

CScriptManager::CScriptManager()
	: m_bIsOk(false)
{
}

CScriptManager::~CScriptManager()
{
	if (m_bIsOk)
	{
		Destroy();
	}
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
	lua_close(m_LS);
}

//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
	if(luaL_dostring(m_LS,Code.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		LOGGER->AddNewLog(ELL_ERROR, l_Str);
	}
}

//Para ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
	if(luaL_dofile(m_LS, FileName.c_str()))
	{
		const char *l_Str=lua_tostring(m_LS, -1);
		LOGGER->AddNewLog(ELL_ERROR, l_Str);
	}else
	{

		std::string msg_info = "CScriptManager::Load->Cargado el archivo de Scripting: " + FileName;
		LOGGER->AddNewLog(ELL_INFORMATION, msg_info.c_str());
	}
}

void CScriptManager::Load(const std::string &XMLFile){
	m_FileName=XMLFile;
	CXMLTreeNode parser;
	if (!parser.LoadFile(XMLFile.c_str()))
	{
		std::string msg_error = "CScriptManager::Load->Error al intentar leer el archivo de Scripting: " + XMLFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	// Obtenemos el nodo "Actions"
	CXMLTreeNode  l_ScriptsNode = parser["scripting"];
	if (l_ScriptsNode.Exists())
	{
		const char* l_FileName = NULL;
		// Obtenemos los datos para todos los action
		int l_count_scripts = l_ScriptsNode.GetNumChildren();
		for (int i = 0; i < l_count_scripts; ++i)
		{
			if (l_ScriptsNode(i).Exists() && !l_ScriptsNode(i).IsComment() )
			{
				l_FileName = l_ScriptsNode(i).GetPszProperty("file_path","");
				RunFile(l_FileName);


			}
		}
	}
}
CCore * GetCore()
{
	return CORE;
}
void LogText(const std::string &Text )
{

	//CLogger* logger=GetLogger();
	LOGGER->AddNewLog(ELL_INFORMATION,Text.c_str());
}

CLogger* GetLogger()
{
	return LOGGER;
}

void CScriptManager::RegisterLUAFunctions()
{
	RegisterCore();
	RegisterProcess();
	RegisterGUI();
	RegisterObject3D();
	RegisterVect3f();
	RegisterScriptManager();
	RegisterSoundManager();
	RegisterParticleManager();
	RegisterROTManager();
	RegisterROLManager();
	RegisterEffectManager();
	RegisterLanguageManager();
	RegisterFontManager();
	RegisterInputManager();
	RegisterTextureManager();
	RegisterStaticMeshManager();
	RegisterAnimatedModelsManager();
	RegisterLightManager();
	RegisterSceneRendererCommandManager();
	RegisterTriggerManager();
	RegisterLogicObjectsManager();
	RegisterPhysicsManager();
	RegisterLevelManager();
	RegisterEnemyManager();
	RegisterBillboardManager();

	CORE->GetProcess()->RegisterLUA();

	REGISTER_LUA_FUNCTION(m_LS, "log", &LogText);
}

void CScriptManager::RegisterROTManager(){
	module(m_LS) [
		
		class_<CMapManager<CRenderableObjectTechnique>>("CMapManagerROTM"),
			class_<CRenderableObjectsTechniqueManager,CMapManager<CRenderableObjectTechnique>>("CRenderableObjectsTechniqueManager")
			.def("load",&CRenderableObjectsTechniqueManager::Load)
			.def("reload",&CRenderableObjectsTechniqueManager::Reload)
	];
	luabind::globals(m_LS)
		[
			"_ROTM"
		] = CORE->GetRenderableObjectsTechniqueManager();
}

void CScriptManager::RegisterROLManager(){
	module(m_LS) [
		
		class_<CMapManager<CRenderableObjectsLayersManager>>("CMapManagerROLM"),
			class_<CRenderableObjectsLayersManager,CMapManager<CRenderableObjectsLayersManager>>("CRenderableObjectsLayersManager")
			.def("load",&CRenderableObjectsLayersManager::Load)
			.def("reload",&CRenderableObjectsLayersManager::Reload)
			.def("set_layer_enabled",&CRenderableObjectsLayersManager::SetLayerEnabled)
			.def("get_layer_enabled",&CRenderableObjectsLayersManager::GetLayerEnabled)
	];
	luabind::globals(m_LS)
		[
			"_ROLM"
		] = CORE->GetRenderableObjectsLayersManager();
}

void CScriptManager::RegisterEffectManager(){
	module(m_LS)
		[
			class_<CMapManager<CEffectTechnique>>("CMapManagerEM"),
			class_<CEffectManager,CMapManager<CEffectTechnique>>("CEffectManager")
			.def("load",&CEffectManager::Load)
			.def("reload",&CEffectManager::Reload)
			.def("set_mirror_dark_coef",&CEffectManager::SetMirrorDarkCoef)
			.def("get_mirror_dark_coef",&CEffectManager::GetMirrorDarkCoef)
		];
	luabind::globals(m_LS)
		[
			"_EFFECTM"
		] = CORE->GetEffectManager();
}

void CScriptManager::RegisterScriptManager(){
	module(m_LS)
		[
			class_<CScriptManager>("CScriptManager")
			.def("load",&CScriptManager::Load)
			.def("reload",&CScriptManager::Reload)
			.def("run_script",&CScriptManager::RunCode)

		];
	luabind::globals(m_LS)
		[
			"_SCRIPTM"
		] = CORE->GetScriptManager();
}

void CScriptManager::RegisterLanguageManager(){
	module(m_LS)
		[
			class_<CLanguageManager>("CLanguageManager")
			.def("reload",&CLanguageManager::LoadXMLs)

		];
	luabind::globals(m_LS)
		[
			"_LANGM"
		] = CORE->GetLanguageManager();
}

void CScriptManager::RegisterFontManager(){
	module(m_LS)
		[
			class_<CFontManager>("CFontManager")
			.def("reload",&CFontManager::ReloadTTFs)

		];
	luabind::globals(m_LS)
		[
			"_FONTM"
		] = CORE->GetFontManager();
}

void CScriptManager::RegisterParticleManager(){
	module(m_LS)
		[
			class_<CParticleManager>("CParticleManager")
			.def("reload",&CParticleManager::Reload)

		];
	luabind::globals(m_LS)
		[
			"_PARTM"
		] = CORE->GetParticleManager();
}

void CScriptManager::RegisterBillboardManager(){
	module(m_LS)
		[
			class_<CBillboardManager>("CBillboardManager")
			.def("reload",&CBillboardManager::Reload)

		];
	luabind::globals(m_LS)
		[
			"_BILLBM"
		] = CORE->GetBillboardManager();
}

void CScriptManager::RegisterSoundManager(){
	module(m_LS)
		[
			class_<CSoundManager>("CSoundManager")
			.def("create_source",&CSoundManager::CreateSource)
			.def("delete_source",&CSoundManager::DeleteSource)
			.def("reset",&CSoundManager::Reset)
			.def("stop",&CSoundManager::Stop)
			.def("play_action_2D",&CSoundManager::PlayAction2D)
			.def("play_action_3D",&CSoundManager::PlayAction3D)
			.def("play_source_2D",&CSoundManager::PlaySource2D)
			.def("play_source_3D",&CSoundManager::PlaySource3D)
			.def("set_source_position",&CSoundManager::SetSourcePosition)
			.def("set_source_velocity",&CSoundManager::SetSourceVelocity)
			.def("set_source_gain",&CSoundManager::SetSourceGain)
			.def("get_source_gain",&CSoundManager::GetSourceGain)
			.def("pause_source",&CSoundManager::PauseSource)
			.def("stop_source",&CSoundManager::StopSource)
			.def("fade_in_source",&CSoundManager::FadeInSource)
			.def("fade_out_source",&CSoundManager::FadeOutSource)
			.def("set_gain",&CSoundManager::SetGain)
			.def("get_gain",&CSoundManager::GetGain)
			.def("pause",&CSoundManager::Pause)
			.def("reset",&CSoundManager::Reset)
		];
	luabind::globals(m_LS)
		[
			"_SOUNDM"
		] = CORE->GetSoundManager();
}

void CScriptManager::RegisterInputManager(){
	module(m_LS)
		[
			class_<CActionToInput>("CActionToInput")
			.def("reload",&CActionToInput::ReLoadXML)

		];
	luabind::globals(m_LS)
		[
			"_INPUTM"
		] = CORE->GetActionToInput();
}

void CScriptManager::RegisterTextureManager(){
	module(m_LS)
		[
			class_<CTextureManager>("CTextureManager")
			.def("reload",&CTextureManager::Reload)

		];
	luabind::globals(m_LS)
		[
			"_TEXTUREM"
		] = CORE->GetTextureManager();
}

void CScriptManager::RegisterStaticMeshManager(){
	module(m_LS)
		[
			class_<CStaticMeshManager>("CStaticMeshManager")
			.def("reload",&CStaticMeshManager::Reload)
			.def("set_texture",&CStaticMeshManager::SetMeshTexture)
		];
	luabind::globals(m_LS)
		[
			"_STATICMESHM"
		] = CORE->GetStaticMeshManager();
}

void CScriptManager::RegisterAnimatedModelsManager(){
	module(m_LS)
		[
			class_<CAnimatedModelsManager>("CAnimatedModelsManager")
			.def("reload",&CAnimatedModelsManager::Reload)

		];
	luabind::globals(m_LS)
		[
			"_ANIMATEDMODELSM"
		] = CORE->GetAnimatedModelsManager();
}

void CScriptManager::RegisterLightManager(){
	module(m_LS)
		[
			class_<CLightManager>("CLightManager")
			.def("load",&CLightManager::Load)
			.def("reload",&CLightManager::Reload)
			.def("set_light_enabled",&CLightManager::SetLightEnabled)
			.def("set_light_active",&CLightManager::SetLightActive)
			.def("set_start_moving",&CLightManager::SetLightMoving)
		];
	luabind::globals(m_LS)
		[
			"_LIGHTM"
		] = CORE->GetLightManager();
}

void CScriptManager::RegisterTriggerManager(){
	module(m_LS)
		[
			class_<CTriggerManager>("CTriggerManager")
			.def("load",&CTriggerManager::Load)
			.def("reload",&CTriggerManager::Reload)
			.def("set_active",&CTriggerManager::SetActive)
		];
	luabind::globals(m_LS)
		[
			"_TRIGGERM"
		] = CORE->GetTriggerManager();
}

void CScriptManager::RegisterLogicObjectsManager(){
	module(m_LS)
		[
			class_<CLogicObjectsManager>("CLogicObjectsManager")
			.def("load",&CLogicObjectsManager::Load)
			.def("reload",&CLogicObjectsManager::Reload)
			.def("change_state", &CLogicObjectsManager::SetObjectEnabled)
			.def("change_state_all", &CLogicObjectsManager::SetAllObjectsEnabled)
			.def("set_mirror_active", &CLogicObjectsManager::SetMirrorRenderActive)
			.def("get_mirror_active", &CLogicObjectsManager::GetMirrorRenderActive)
			.def("set_mirror_on",&CLogicObjectsManager::SetMirrorON)
			.def("close_door",&CLogicObjectsManager::CloseDoor)
			.def("restart_objects", &CLogicObjectsManager::RestartAllObjects)
		];
	luabind::globals(m_LS)
		[
			"_LOGICOBJECTSM"
		] = CORE->GetLogicObjectsManager();
}

void CScriptManager::RegisterPhysicsManager(){
	module(m_LS)
		[
			class_<CPhysicsManager>("CPhysicsManager")
			.def("init",&CPhysicsManager::Init)
			.def("release",&CPhysicsManager::Release)

		];
	luabind::globals(m_LS)
		[
			"_PHYSXM"
		] = CORE->GetPhysicsManager();
}

void CScriptManager::RegisterSceneRendererCommandManager(){
	module(m_LS)
		[
			class_<CSceneRendererCommandManager>("CSceneRendererCommandManager")
			.def("load",&CSceneRendererCommandManager::Load)
			.def("reload",&CSceneRendererCommandManager::Reload)

		];
	luabind::globals(m_LS)
		[
			"_SCENERENDERERCOMMANDM"
		] = CORE->GetSceneRendererCommandManager();
}

void CScriptManager::RegisterObject3D(){
	module(m_LS)
		[
			class_<CObject3D>("CObject3D")
			   .def("get_yaw",&CObject3D::GetYaw)
			   .def("get_roll",&CObject3D::GetRoll)
/*
	float						GetPitch		() const {return m_fPitch;}
	const Vect3f&				GetPosition		() const {return m_Position;}
	const Vect3f&				GetScale		() const {return m_Scale;}
	bool						GetVisible		() const {return m_Visible;}
	bool						GetCastShadows	() const {return m_CastShadows;}
	Mat44f						GetTransform	();

	//---Set Functions
	void		SetPosition		( const Vect3f& pos )		{m_Position = pos;}
	void		SetScale		( const Vect3f& scale )		{m_Scale = scale;}
	void		SetYaw			( float yaw )				{m_fYaw = yaw;}
	void		SetPitch		( float pitch )				{m_fPitch = pitch;}
	void		SetRoll			( float roll )				{m_fRoll = roll;}
	void		SetVisible		( bool vis )				{m_Visible = vis;}
	void		SetCastShadows  ( bool cast_shadows )		{m_CastShadows = cast_shadows;}*/
		];
}


void CScriptManager::RegisterVect3f(){
  //Vect3f
  module(m_LS) [
    class_<Vect3f>("Vect3f")
      .def(constructor<>())
      .def(constructor<const Vect3f&>())
      .def(constructor<const float>())
      .def(constructor<const float, const float, const float>())
      .def_readwrite("x",&Vect3f::x)
      .def_readwrite("y",&Vect3f::y)
      .def_readwrite("z",&Vect3f::z)
      .def("length",       &Vect3f::Length)
      .def("length_sq",    &Vect3f::SquaredLength)
      .def("normalize",    &Vect3f::Normalize)
      .def("get_proj_zx",  &Vect3f::GetProjZX)
      .def("get_angle_y",  &Vect3f::GetAngleY)
  ];
}


void CScriptManager::RegisterCore(){
	module(m_LS)
		[

			class_<CSingleton<CCore>>("CSingletonCore")
			.scope[
				def("GetSingletonPtr",&CSingleton<CCore>::GetSingletonPtr) //returns static singleton instance
			],
			class_<CCore,CSingleton<CCore>>("CCore")
			.def("get_renderable_objects_technique_manager",&CCore::GetRenderableObjectsTechniqueManager)
			.def("get_renderable_objects_layers_manager",&CCore::GetRenderableObjectsLayersManager)
			.def("get_effect_manager",&CCore::GetEffectManager)
			.def("get_script_manager",&CCore::GetScriptManager)
			.def("get_language_manager",&CCore::GetLanguageManager)
			.def("get_font_manager",&CCore::GetFontManager)
			.def("get_input_manager",&CCore::GetActionToInput)
			.def("get_sound_manager",&CCore::GetSoundManager)
			.def("get_particle_manager",&CCore::GetParticleManager)
			.def("get_texture_manager",&CCore::GetTextureManager)
			.def("get_static_mesh_manager",&CCore::GetStaticMeshManager)
			.def("get_animated_models_manager",&CCore::GetAnimatedModelsManager)
			.def("get_scene_renderer_command_manager",&CCore::GetSceneRendererCommandManager)
			.def("get_light_manager",&CCore::GetLightManager)
			.def("get_gui_manager",&CCore::GetGUIManager)
			.def("get_process",&CCore::GetProcess)

		];

}


void CScriptManager::RegisterProcess() {
	module(m_LS)
		[
			class_<CProcess>("CProcess")
			.def("init", &CProcess::Init)
			.def("reload", &CProcess::Reload)
			.def("get_start_game",&CProcess::GetStartGame)
			.def("set_start_game",&CProcess::SetStartGame)
			.def("get_end_game",&CProcess::GetEndGame)
			.def("set_end_game",&CProcess::SetEndGame)
		];
	luabind::globals(m_LS)
		[
			"_PROC"
		] = CORE->GetProcess();
}



void CScriptManager::RegisterGUI() {
	module(m_LS)
		[
			class_<CGUIManager>("CGUIManager")
			.def("active_windows",&CGUIManager::ActiveWindows)
			.def("push_windows",&CGUIManager::PushWindows)
			.def("pop_windows",&CGUIManager::PopWindows)
			.def("set_visible_pointer_mouse",&CGUIManager::SetVisiblePointerMouse)
			.def("get_state_slider",&CGUIManager::GetStateSlider)
			.def("set_visible_gui_element", &CGUIManager::SetVisibleGuiElement)
			.def("set_active_gui_element", &CGUIManager::SetActiveGuiElement)
			.def("get_button_CheckIn_RadioBox", &CGUIManager::GetButtonCheckInRadioBox)
		];

	luabind::globals(m_LS)
		[
			"_GUI"
		] = CORE->GetGUIManager();
}

void CScriptManager::RegisterLevelManager(){
	module(m_LS)
		[
			class_<CLevelManager>("CLevelManager")
			.def("load_level",&CLevelManager::LoadLevel)
			.def("reload_level",&CLevelManager::ReloadLevel)
			.def("get_level",&CLevelManager::GetLevelName)
			.def("is_ok",&CLevelManager::IsOk)
		];
	luabind::globals(m_LS)
		[
			"_LEVELM"
		] = CORE->GetLevelManager();
}

void CScriptManager::RegisterEnemyManager()
{
	module(m_LS)
		[
			class_<CEnemyManager>("CEnemyManager")
			.def("freeze_all",&CEnemyManager::FreezeAll)
			.def("unfreeze_all",&CEnemyManager::UnfreezeAll)
			.def("reset_all",&CEnemyManager::ResetAll)
			.def("create_enemy",&CEnemyManager::CreateDynamicEnemy)
			.def("activate_enemy",&CEnemyManager::ActivateDynamicEnemy)
		];
	luabind::globals(m_LS)
		[
			"_ENEMYM"
		] = CORE->GetEnemyManager();
}

void CScriptManager::Reload ()
{
	if (m_FileName != "")
	{

		Load (m_FileName);
	
	}
}

