#ifndef __CLASS_STATICMESHMANAGER_H__
#define __CLASS_STATICMESHMANAGER_H__
#pragma once

#include "StaticMesh.h"
#include "Utils/MapManager.h"

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
protected:
	std::string m_FileName;

public:
	CStaticMeshManager();
	~ CStaticMeshManager ();
	bool Load(const std::string &FileName);
	bool LoadFolder (const std::string &FolderName);
	bool Reload();

	void SetMeshTexture(const std::string& mesh, const std::string texture, int stage);
};

#endif