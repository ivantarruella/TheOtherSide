#ifndef __CLASS_CORE_H__
#define __CLASS_CORE_H__

#pragma once

#include <Windows.h>
#include "Singleton.h"
#include "EngineDef.h"
#include "Timer.h"
#include "LogRender.h"
#include "Timer.h"

class CTimer;
class CProcess;
class CCamera;
class CConsole;
class CRenderManager;
class CFontManager;
class CLanguageManager;
class CInputManager;
class CActionToInput;	
class CTextureManager;
class CStaticMeshManager;
class CAnimatedModelsManager;
class CLightManager;
class CEffectManager;
class CRenderableObjectsLayersManager;
class CSceneRendererCommandManager;
class CRenderableObjectsTechniqueManager;
class CScriptManager;
class CPhysicsManager;
class CBillboardManager;
class CParticleManager;
class CGUIManager;
class CTriggerManager;
class CSoundManager;
class CNodeManager;
class CLogicObjectsManager;
class CLevelManager;
class CRandom;
class CEnemyManager;
class CCoverManager;
class CBulletManager;

class CCore: public CSingleton<CCore>
{ 
public:
	CCore() : 
	    m_bIsOk(true), 
		m_bShowFPS(false),
		m_fFPS(0.0f),
		m_fTime(0.0f),
		m_pTimer(NULL),
		m_pProcess(NULL),
		m_pCamera(NULL),
#ifdef _DEBUG		
		m_pConsole(NULL),
		m_pRenderManager(NULL), 
#endif
		m_pFontManager(NULL), 
		m_pLanguageManager(NULL), 
		m_pInputManager(NULL), 
		m_pActionToInput(NULL),
		m_pTextureManager(NULL),
		m_pStaticMeshManager(NULL),
		m_pAnimatedModelsManager(NULL),
		m_pLightManager(NULL),
		m_pEffectManager(NULL),
		m_pRenderableObjectsLayersManager(NULL),
		m_pSceneRendererCommandManager(NULL),
		m_pScriptManager(NULL),
		m_pPhysicsManager(NULL),
		m_pBillboardManager(NULL),
		m_pParticleManager(NULL),
		m_pGUIManager(NULL),
		m_pTriggerManager(NULL),
		m_pRandom(NULL),
		m_pSoundManager(NULL),
		m_pLogicObjectsManager(NULL),
		m_pLevelManager(NULL),
		m_pNodeManager(NULL),
		m_pEnemyManager(NULL),
		m_pBulletManager(NULL),
		m_pCoverManager(NULL)
	  {	 
	  }
	virtual ~CCore(void) {Done();}

	bool Init(HWND hWnd, CProcess* Process, const SInitParams& params);
	bool IsOk() { return m_bIsOk; }
	void Done();
	void Update(bool show_fps);
	void DrawFPS();

	CProcess* GetProcess(){ return m_pProcess; }
#ifdef _DEBUG	
	CLogRender* GetLogRender() {return m_pLogRender;}
	CConsole* GetConsole() {return m_pConsole;}
#endif
	CRenderManager* GetRenderManager() {return m_pRenderManager;}
	CFontManager* GetFontManager() {return m_pFontManager;}
	CLanguageManager* GetLanguageManager()	{return m_pLanguageManager;}
	CInputManager* GetInputManager() {return m_pInputManager;}
	CActionToInput* GetActionToInput()	{return m_pActionToInput;}
	CTextureManager* GetTextureManager() {return m_pTextureManager;}
	CStaticMeshManager* GetStaticMeshManager() {return m_pStaticMeshManager;}
	CAnimatedModelsManager* GetAnimatedModelsManager() {return m_pAnimatedModelsManager;}
	CLightManager* GetLightManager() {return m_pLightManager;}
	CEffectManager* GetEffectManager() {return m_pEffectManager;}
	CRenderableObjectsLayersManager* GetRenderableObjectsLayersManager(){ return m_pRenderableObjectsLayersManager;}
	CSceneRendererCommandManager* GetSceneRendererCommandManager(){ return m_pSceneRendererCommandManager;}
	CRenderableObjectsTechniqueManager* GetRenderableObjectsTechniqueManager(){ return m_pRenderableObjectsTechniqueManager;}
	CScriptManager* GetScriptManager(){ return m_pScriptManager;}
	CPhysicsManager* GetPhysicsManager(){ return m_pPhysicsManager;}
	CBillboardManager* GetBillboardManager(){ return  m_pBillboardManager;}
	CParticleManager* GetParticleManager() { return m_pParticleManager;}
	CGUIManager* GetGUIManager() { return m_pGUIManager;}
	CTriggerManager* GetTriggerManager() { return m_pTriggerManager;}
	CSoundManager* GetSoundManager() { return m_pSoundManager;}
	CNodeManager* GetNodeManager() { return m_pNodeManager;}
	CLogicObjectsManager* GetLogicObjectsManager() { return m_pLogicObjectsManager;}
	CLevelManager* GetLevelManager() { return m_pLevelManager; }
	CRandom* GetRandom() {return m_pRandom;} 
	CEnemyManager* GetEnemyManager() {return m_pEnemyManager;} 
	CBulletManager* GetBulletManager() {return m_pBulletManager;}
	CCoverManager* GetCoverManager() {return m_pCoverManager;}

	void SetCamera(CCamera* Camera) { m_pCamera = Camera; }
	CCamera* GetCamera(){ return m_pCamera; }

	float GetTime(){ return m_fTime; }
	
	float GetElapsedTime() { return m_pTimer->GetElapsedTime(); }

private:
	void Release();
	void UpdateFPS(float elapsedTime, bool show_fps);

private:
	bool			m_bIsOk;
	bool			m_bShowFPS;
	float			m_fFPS;
	float			m_fTime;
	
	CTimer*			m_pTimer;
	CProcess*		m_pProcess;	
	CCamera*		m_pCamera;
#ifdef _DEBUG	
	CLogRender*		m_pLogRender;
	CConsole*       m_pConsole;
#endif
	CRenderManager* m_pRenderManager;
	CFontManager* m_pFontManager;
	CLanguageManager* m_pLanguageManager;
	CInputManager* m_pInputManager;
	CActionToInput* m_pActionToInput;	
	CTextureManager* m_pTextureManager;
	CStaticMeshManager* m_pStaticMeshManager;
	CAnimatedModelsManager* m_pAnimatedModelsManager;
	CLightManager* m_pLightManager;
	CEffectManager* m_pEffectManager;
	CRenderableObjectsLayersManager* m_pRenderableObjectsLayersManager;	
	CSceneRendererCommandManager* m_pSceneRendererCommandManager;
	CRenderableObjectsTechniqueManager* m_pRenderableObjectsTechniqueManager;
	CScriptManager* m_pScriptManager;
	CPhysicsManager* m_pPhysicsManager;
	CBillboardManager* m_pBillboardManager;
	CParticleManager* m_pParticleManager;
	CGUIManager* m_pGUIManager;
	CTriggerManager* m_pTriggerManager;
	CSoundManager* m_pSoundManager;
	CNodeManager* m_pNodeManager;
	CLogicObjectsManager* m_pLogicObjectsManager;
	CLevelManager* m_pLevelManager;
	CCoverManager* m_pCoverManager;
	CRandom* m_pRandom;
	CEnemyManager* m_pEnemyManager;
	CBulletManager* m_pBulletManager;
};


#endif
