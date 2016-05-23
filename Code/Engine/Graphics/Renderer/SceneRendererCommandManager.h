#pragma once
#ifndef __CLASS_SCENE_RENDERER_COMMAND_MANAGER_H__
#define __CLASS_SCENE_RENDERER_COMMAND_MANAGER_H__

#include "SceneRendererCommand.h"
#include "Utils\VectorMapManager.h"

#include <string>
#include <sstream>

class CRenderManager;

class CSceneRendererCommandManager
{
private:
	CVectorMapManager<CSceneRendererCommand> m_SceneRendererCommands;

	std::string GetNextName();
	std::string m_FileName;

public:
	CSceneRendererCommandManager();
	~CSceneRendererCommandManager();

	void Load(const std::string &FileName);
	void Reload();
	void Execute(CRenderManager &RM);
	void Release();
};

#endif