#ifndef __CLASS_LEVELMANAGER_H__
#define __CLASS_LEVELMANAGER_H__

#pragma once

#include "LevelDef.h"
#include <string>
#include <future>

#define	INIT_FILE	"init.xml"


class CLevelManager 
{
public:
	CLevelManager(const std::string& shadows_type);
	~CLevelManager();

	void Initialize();
	bool Update();
	void LoadLevel (const std::string& pathFile);
	void ReloadLevel (const std::string& pathFile);

	bool IsOk() { return m_bIsOk;}
	bool IsLevelLoaded() { return m_bLevelLoaded;  }

	const std::string& GetLevelName();

private:	
	void LoadPaths(const std::string& pathFile);
	bool InitLevel();

private:
	SLevelPaths m_LevelPaths;
	bool		m_bIsOk;
	bool		m_bChanging;
	bool	    m_bLevelLoaded;
	std::string m_shadowsType;
	
	std::vector<std::future<void>>	m_loading_meshes;
	std::vector<std::future<void>>	m_loading_textures;
};

#endif