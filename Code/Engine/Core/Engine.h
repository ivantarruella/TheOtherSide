#ifndef __CLASS_ENGINE_H__
#define __CLASS_ENGINE_H__

#pragma once

#include <Windows.h>
#include "Utils/Types.h"
#include "EngineDef.h"

class CProcess;
class CCore;
class CRenderManager;

#define FPS_LIMIT	100

class CEngine
{ 

public:
	CEngine()
		: m_bIsOk(false), m_pProcess(NULL), m_pCore(NULL), m_sPathFile("")
	{}
	virtual ~CEngine() { Release(); }

	bool Init(HWND hWnd);
	void Update();
	void Render();
	
	inline void SetProcess(CProcess* proc) { m_pProcess = proc;	}
	CProcess* GetProcess() { return m_pProcess; }
	
	bool LoadConfig(const std::string& pathFile);
	bool ReLoadConfig();
	bool ReLoadDefaultLanguage();


public:
	SInitParams		m_InitParams;		

private:
	void Release();
	void UpdateInputActions();

private:
	bool			m_bIsOk;
	CProcess*		m_pProcess;
	CCore*			m_pCore;
	std::string		m_sPathFile;
};

#endif
