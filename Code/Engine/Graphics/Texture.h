#pragma once
#ifndef __CLASS_TEXTURE_H__
#define __CLASS_TEXTURE_H__

#include "d3dx9.h"
#include "Named.h"
#include <string>

class CTexture : public CNamed
{
public:
	enum TPoolType {
		DEFAULT=0,
		SYSTEMMEM
	};
	enum TUsageType {
		DYNAMIC=0,
		RENDERTARGET
	};
	enum TTextureType {
		TGA=0,
		JPG,
		BMP
	};
	enum TFormatType {
		A8R8G8B8=0,
		R8G8B8,
		X8R8G8B8,
		R32F,
		RG32F,
		A16B16G16R16F
	};

protected:
	LPDIRECT3DTEXTURE9 m_Texture;
	std::string m_FileName;
	bool m_ViewportStencil;

	LPDIRECT3DSURFACE9 m_OldRenderTarget;
	LPDIRECT3DSURFACE9 m_RenderTargetTexture;
	LPDIRECT3DSURFACE9 m_OldDepthStencilRenderTarget;
	LPDIRECT3DSURFACE9 m_DepthStencilRenderTargetTexture;

	unsigned int m_Width;
	unsigned int m_Height;
	
	virtual bool LoadFile();
	void Unload();
	
public:
	CTexture()
		: m_Texture(NULL), m_FileName(""), m_ViewportStencil(false)
	{
	}
	~CTexture()
	{
		Unload();
	}

	const std::string & GetFileName() const;
	bool Load(const std::string &FileName);
	bool Reload();
	void Activate(size_t StageId);
	void Deactivate(size_t StageId);
	
	bool SetAsRenderTarget(size_t IdStage=0, bool viewportStencil=false);
	void UnsetAsRenderTarget(size_t IdStage=0);
	static CTexture::TFormatType GetFormatTypeFromString(const std::string &FormatType);
	bool Create(const std::string &Name, unsigned int Width, unsigned int Height, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType, TFormatType FormatType);
	void CaptureFrameBuffer(size_t IdStage);

	LPDIRECT3DTEXTURE9 GetTexture();

	unsigned int GetWidth() { return m_Width;}
	unsigned int GetHeight() { return m_Height;}
};

#endif
