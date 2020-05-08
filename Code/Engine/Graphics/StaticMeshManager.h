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
	bool Reload();

	void LoadFolder(const std::string& FolderName);

	void SetMeshTexture(const std::string& mesh, const std::string texture, int stage);
	bool GetMeshPreLoad() { return bMeshesPreLoaded; }

private:
	bool bMeshesPreLoaded;
};

#endif