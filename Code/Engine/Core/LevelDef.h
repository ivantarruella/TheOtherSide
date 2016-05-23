#ifndef __CLASS_LEVEL_DEF_H__
#define __CLASS_LEVEL_DEF_H__

#pragma once

#include <string>

struct SLevelPaths
{
	SLevelPaths()
		: m_sXMLPath(""), m_sLevelName(""), 
		  m_sStaticMeshesPath(""), m_sAnimatedModelsPath(""), m_sRenderableObjectsPath(""), m_sLightsPath(""), m_sLuaPath(""),  
		  m_sBillboardPath(""), m_sParticlesPath(""), m_sTriggersPath(""), m_sNodesPath(""), m_sLogicObjectsPath(""), m_sCoversPath("")
	{
	}

	std::string m_sXMLPath;
	std::string m_sLevelName;

	std::string m_sStaticMeshesPath;
	std::string m_sAnimatedModelsPath;
	std::string m_sRenderableObjectsPath;
    std::string m_sLightsPath;
	std::string m_sLuaPath;
	std::string m_sBillboardPath;
	std::string m_sParticlesPath;
	std::string m_sTriggersPath;
	std::string m_sNodesPath;
	std::string m_sLogicObjectsPath;
	std::string m_sCoversPath;
};

#endif