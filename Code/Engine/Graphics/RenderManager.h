#ifndef __CLASS_RENDERMANAGER_H__
#define __CLASS_RENDERMANAGER_H__

#pragma once

#include <d3dx9.h>
#include "Math\Color.h"
#include "EngineDef.h"
#include "Camera\Frustum.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

struct CUSTOMVERTEX
{
    float x, y, z;
    DWORD color;
    static unsigned int getFlags()
    {
   	 return D3DFVF_CUSTOMVERTEX;
    }
};

#define D3DFVF_SCREEN_COLOR_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
struct SCREEN_COLOR_VERTEX
{
	float x,y,z,w;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_SCREEN_COLOR_VERTEX;
	}
};


typedef enum ETypeAlignment { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };
typedef enum ETypeFlip
{
    NONE_FLIP,
    FLIP_X,
    FLIP_Y
};

class CColor;
class CCamera;
class CTexture;


class CRenderManager
{
public:
	CRenderManager()
		: m_hWnd(NULL), m_pD3D(NULL), m_pD3DDevice(NULL), m_bIsOk(false), m_uWidth(0), m_uHeight(0), m_bPaintSolid(TRUE), m_BackbufferColor_debug(colDARKGRAY), m_BackbufferColor_release(colBLACK)
	{
	}
	~CRenderManager() { Done(); }

	bool Init(HWND hWnd, const SInitParams& params);
	void Done();
	//void BeginRendering();
	//void EndRendering();
	void Clear(bool m_Color, bool m_Depth, bool m_Stencil, DWORD Col);
	void SetupMatrices(CCamera* camera);
	void SetTransform(D3DXMATRIX& matrix);
	void SetTransform(Mat44f& m);
	const CFrustum& GetFrustum() { return m_Frustum; }

	LPDIRECT3DDEVICE9 GetDevice() {return m_pD3DDevice;}
	void GetWidthAndHeight(uint32& w, uint32& h) {w = m_uWidth; h = m_uHeight; }

	void SetPaintSolid(bool val) {m_bPaintSolid = val;}
	bool GetPaintSolid() {return m_bPaintSolid;}
	
	CColor GetBackBufferColor_debug() {return m_BackbufferColor_debug;}
	CColor GetBackBufferColor_release() {return m_BackbufferColor_release;}
	
	void DrawLine( const Vect3f &PosA, const Vect3f &PosB, CColor Color);
	void DrawAxis ( float size);
	void DrawGrid ( float sizeV, float sizeH, uint16 num);
	void DrawPlane(float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ );
	void DrawCube ( const Vect3f &size, CColor Color);
	void DrawCircle(float Radius, float coordY, CColor Color, uint16 num);
	void DrawElipse(float a, float b, float coordY, CColor Color, uint16 num);
	void DrawSphere( float radius, uint32 edges, CColor color);
	void DrawCone (float radius , float h, CColor color ,uint16 num);
	void DrawConeInv (float radius , float h, CColor color , uint16 num);
	void DrawCilinder (float radius , float h, CColor color , uint16 num);
	
	void DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color);
	void DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, ETypeFlip flip);	
	void DrawColoredQuad2DTexturedInPixels(RECT Rect, CColor Color, CTexture * Texture, float U0=0.0f, float V0=0.0f, float U1=1.0f, float V1=1.0f);
	void CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos);
	void DrawRectangle2D (const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor,	uint32 edge_w, uint32 edge_h, CColor& edgeColor );
	void DrawTexturedQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *Texture, CColor color = colBLUE);

	void EnableAlphaBlend();
	void DisableAlphaBlend();

private:
	void GetWindowRect(HWND hwnd);
	void Release();

private:
	HWND    	 	m_hWnd;   				// 3D render window handle -> Constructor a NULL
	LPDIRECT3D9	m_pD3D; 					// direct3d interface -> Constructor a NULL
	LPDIRECT3DDEVICE9      m_pD3DDevice;    // direct3d device -> Constructor a NULL
	bool			m_bIsOk;
	uint32 m_uWidth;
	uint32 m_uHeight;
	bool m_bPaintSolid;
	CColor m_BackbufferColor_debug;
	CColor m_BackbufferColor_release;
	CFrustum m_Frustum;
};

#endif
