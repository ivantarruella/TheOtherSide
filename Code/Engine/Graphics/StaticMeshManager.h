#ifndef __CLASS_STATICMESHMANAGER_H__
#define __CLASS_STATICMESHMANAGER_H__
#pragma once

#include <mutex>
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

	void LoadMeshes(const std::string& FolderName, const std::vector<std::string>& MeshNameVect, size_t from, size_t size);

	void SetMeshTexture(const std::string& mesh, const std::string texture, int stage);
	bool GetMeshPreLoad() { return bMeshesPreLoaded; }

	std::mutex& GetMutex() { return m_mutex; }

private:
	bool bMeshesPreLoaded;
	std::mutex m_mutex;
};

#endif