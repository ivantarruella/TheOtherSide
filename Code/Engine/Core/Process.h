#ifndef __CLASS_PROCESS_H__
#define __CLASS_PROCESS_H__

#pragma once

#include "EngineDef.h"


class CRenderManager;
class CCamera;

class CProcess
{
public:
	CProcess()
		:m_Camera(NULL), m_bIsOk(false), m_bStartGame(false), m_bEndGame(false)
	{
	}	
	virtual ~CProcess(){};

	virtual void PreInit() = 0;
	virtual bool Init() = 0;
	virtual void Release() = 0;
	virtual void Update(float ElapsedTime) = 0;
	virtual void RenderScene(CRenderManager* rendermanager) = 0;
	virtual void RenderDebug(CRenderManager* rendermanager) = 0;
    virtual void Load() = 0;
	virtual void Reload() = 0;
	virtual void RegisterLUA() = 0;

	CCamera* GetCamera() { return m_Camera; }
	
	void SetIsOk(bool ok) { m_bIsOk = ok; }
	bool IsOk() { return m_bIsOk; }

	void SetStartGame(bool start) { m_bStartGame = start; }
	bool GetStartGame() { return m_bStartGame; }
	void SetEndGame(bool end) { m_bEndGame = end; }
	bool GetEndGame() { return m_bEndGame; }

protected:
	CCamera* m_Camera;
	bool m_bIsOk;
	bool m_bStartGame;
	bool m_bEndGame;
};

#endif
