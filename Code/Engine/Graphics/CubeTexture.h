#pragma once
#ifndef __CLASS_CUBETEXTURE_H__
#define __CLASS_CUBETEXTURE_H__

#include "d3dx9.h"
#include "Named.h"
#include <string>

class CCubeTexture : public CNamed
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
	
	LPDIRECT3DCUBETEXTURE9 m_CubeTexture;
	bool m_ViewportStencil;

	LPDIRECT3DSURFACE9 cubeFacePX;
	LPDIRECT3DSURFACE9 cubeFacePY;
	LPDIRECT3DSURFACE9 cubeFacePZ;
	LPDIRECT3DSURFACE9 cubeFaceNX;
	LPDIRECT3DSURFACE9 cubeFaceNY;
	LPDIRECT3DSURFACE9 cubeFaceNZ;

	LPDIRECT3DSURFACE9 m_OldRenderTarget;
	LPDIRECT3DSURFACE9 m_RenderTargetTexture;
	LPDIRECT3DSURFACE9 m_OldDepthStencilRenderTarget;
	LPDIRECT3DSURFACE9 m_DepthStencilRenderTargetTexture;
	
	void Release();
	
public:
	CCubeTexture()
		: m_CubeTexture(NULL), m_ViewportStencil(false)
	{
	}
	~CCubeTexture()
	{
		Release();
	}

	void Activate(size_t StageId);
	void Deactivate(size_t StageId);
	
	bool SetAsRenderTarget(D3DCUBEMAP_FACES face, size_t IdStage=0, bool viewportStencil=false);
	void UnsetAsRenderTarget(size_t IdStage=0);	
	static CCubeTexture::TFormatType GetFormatTypeFromString(const std::string &FormatType);
	bool Create(const std::string &Name, unsigned int Size, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType, TFormatType FormatType);

	LPDIRECT3DCUBETEXTURE9 GetTexture() { return m_CubeTexture; }
	LPDIRECT3DSURFACE9 GetCubeFacePX() { return cubeFacePX; }
	LPDIRECT3DSURFACE9 GetCubeFacePY() { return cubeFacePY; }
	LPDIRECT3DSURFACE9 GetCubeFacePZ() { return cubeFacePZ; }
	LPDIRECT3DSURFACE9 GetCubeFaceNX() { return cubeFaceNX; }
	LPDIRECT3DSURFACE9 GetCubeFaceNY() { return cubeFaceNY; }
	LPDIRECT3DSURFACE9 GetCubeFaceNZ() { return cubeFaceNZ; }
};

#endif
