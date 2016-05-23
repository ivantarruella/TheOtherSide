#ifndef __CLASS_ANIMATEDMODELSMANAGER_H__
#define __CLASS_ANIMATEDMODELSMANAGER_H__
#pragma once

#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "xml/XMLTreeNode.h"
#include "Utils/MapManager.h"

#include <string>

class CAnimatedModelsManager : public CMapManager<CAnimatedCoreModel>
{
private:
	std::string m_FileName;

public:
	CAnimatedModelsManager();
	~CAnimatedModelsManager();
	CAnimatedCoreModel * GetCore(const std::string &Name, const std::string &Path);
	CAnimatedInstanceModel * CreateInstance(const std::string &Name);
	CAnimatedInstanceModel * CreateInstance(const CXMLTreeNode& XmlData);
	CAnimatedInstanceModel * CreateInstance(const std::string &CoreName, const std::string &Name, const Vect3f &pos);
	bool Load(const std::string& FileName);
	void Reload();
};

#endif