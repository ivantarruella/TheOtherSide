#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
#include <vector>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class CScriptManager
{
	private:
		lua_State *m_LS;
		std::string m_FileName;
		bool m_bIsOk;

	public:
		CScriptManager();
		~CScriptManager();
		void Initialize();
		bool IsOk() { return m_bIsOk; }
		void Destroy();
		void RunCode(const std::string &Code) const;
		void RunFile(const std::string &FileName) const;
		
		void Load(const std::string &XMLFile);
		void Reload();
		lua_State * GetLuaState() const {return m_LS;}

		void RegisterCore();
		void RegisterProcess();
		void RegisterEffectManager();
		void RegisterGUI();
		void RegisterLevelManager();

		void RegisterLUAFunctions();
		void RegisterObject3D();
		void RegisterVect3f();
		void RegisterScriptManager();
		void RegisterROTManager();
		void RegisterROLManager();
		void RegisterLanguageManager();
		void RegisterFontManager();
		void RegisterParticleManager();
		void RegisterBillboardManager();
		void RegisterSoundManager();
		void RegisterInputManager();
		void RegisterTextureManager();
		void RegisterStaticMeshManager();
		void RegisterAnimatedModelsManager();
		void RegisterLightManager();
		void RegisterSceneRendererCommandManager();
		void RegisterTriggerManager();
		void RegisterLogicObjectsManager();
		void RegisterPhysicsManager();
		void RegisterEnemyManager();
};

#endif