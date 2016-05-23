#pragma once
#ifndef __CLASS_TEXTUREMANAGER_H__
#define __CLASS_TEXTUREMANAGER_H__

#include "Texture.h"
#include "Utils/MapManager.h"

class CTextureManager : public CMapManager<CTexture>
{
public:
	CTextureManager();
	virtual ~CTextureManager();
	void Reload ();
	CTexture * GetTexture(const std::string &Filename);
};

#endif