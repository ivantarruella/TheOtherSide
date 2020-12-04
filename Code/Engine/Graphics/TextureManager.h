#pragma once
#ifndef __CLASS_TEXTUREMANAGER_H__
#define __CLASS_TEXTUREMANAGER_H__

#include <vector>
#include "Texture.h"
#include "Utils/MapManager.h"

class CTextureManager : public CMapManager<CTexture>
{
public:
	CTextureManager();
	virtual ~CTextureManager();
	void Reload ();
	CTexture * GetTexture(const std::string &Filename);
	void LoadTextures(const std::string& FolderName, const std::vector<std::string>& TexturesVect, size_t from, size_t size);
};

#endif