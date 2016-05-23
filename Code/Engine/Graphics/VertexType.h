#pragma once
#ifndef VERTEX_TYPE_H
#define VERTEX_TYPE_H

#include "d3d9.h"


#define VERTEX_TYPE_GEOMETRY		0x0001
#define VERTEX_TYPE_NORMAL			0x0002
#define VERTEX_TYPE_TANGENT			0x0004
#define VERTEX_TYPE_BINORMAL		0x0008
#define VERTEX_TYPE_TEXTURE1		0x0010
#define VERTEX_TYPE_TEXTURE2		0x0020
#define VERTEX_TYPE_DIFFUSE			0x0040
#define VERTEX_TYPE_BLENDWEIGHT		0x0080
#define VERTEX_TYPE_BLENDINDICES	0x0100
#define VERTEX_TYPE_SCREEN			0x0200
#define VERTEX_TYPE_RNM				0x0400
#define VERTEX_TYPE_RNM_SPEC		0x0800

// Coordenadas pantalla + Textura
struct SCREEN_TEXTURE_VERTEX
{
	float x,y,z,w;
	float u, v;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_SCREEN|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return (D3DFVF_XYZRHW|D3DFVF_TEX1);
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Coordenadas pantalla + Color + Textura
struct SCREEN_TEXTURE_COLORED_VERTEX
{
	float x,y,z,w;
	unsigned long color;
	float u, v;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_SCREEN|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria
struct TXYZ_VERTEX		
{
	float x, y, z;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Color
struct TCOLORED_VERTEX		
{
	float x, y, z;
	unsigned long color;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_DIFFUSE;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Normal + Color
struct TNORMAL_COLORED_VERTEX		
{
	float x, y, z;
	float nx, ny, nz;
	unsigned long color;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL |  VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Textura1
struct TTEXTURE1_VERTEX		
{
	float x, y, z;
	float u, v;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_TEX1;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Textura1 + Textura2
struct TTEXTURE1_TEXTURE2_VERTEX		
{
	float x, y, z;
	float u1, v1;
	float u2, v2;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEX2;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

};

// Geometria + Normal + Textura1
struct TNORMAL_TEXTURE1_VERTEX		
{
	float x, y, z;
	float nx, ny, nz;
	float u1, v1;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Normal + Textura1 + BumpMap 
/*
struct TNORMAL_TEXTURE1_BUMPMAP_VERTEX		
{
	float x, y, z;
	float nx, ny, nz;
	float u1, v1;


	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TANGENT;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};
*/

// Geometria + Normal + Textura1 + Textura2
struct TNORMAL_TEXTURE1_TEXTURE2_VERTEX		
{
	float x, y, z;
	float nx, ny, nz;
	float u1, v1;
	float u2, v2;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Textura1 + Color
struct TTEXTURE1_COLORED_VERTEX	
{
	float x, y, z;
	unsigned long color;
	float u1, v1;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ| D3DFVF_DIFFUSE | D3DFVF_TEX1;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Textura1 + Textura2 + Color
struct TTEXTURE1_TEXTURE2_COLORED_VERTEX	
{
	float x, y, z;
	float u1, v1;
	float u2, v2;
	unsigned long color;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2 | VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_DIFFUSE;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Normal + Textura1 + Color
struct TNORMAL_TEXTURE1_COLORED_VERTEX		
{
	float x, y, z;
	float nx, ny, nz;
	float u1, v1;
	unsigned long color;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

// Geometria + Normal + Textura1 + Textura2 + Color
struct TNORMAL_TEXTURE1_TEXTURE2_COLORED_VERTEX		
{
	float x, y, z;
	float nx, ny, nz;
	float u1, v1;
	float u2, v2;
	unsigned long color;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2 | VERTEX_TYPE_DIFFUSE;
	}

	static inline unsigned int GetFVF()
	{
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_DIFFUSE;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static void ReleaseVertexDeclaration();
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
};

struct CAL3D_HW_VERTEX
{
	float x, y, z;
	float weights[4];
	float indices[4];
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_BLENDINDICES | VERTEX_TYPE_BLENDWEIGHT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TANGENT;		
	}
	static inline unsigned int GetFVF()
	{
	
		return 0;
	}

	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
	static void ReleaseVertexDeclaration();
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX
{
	float x, y, z;
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;
	
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TANGENT;		
	}
	static inline unsigned int GetFVF(){

		return 0 ;
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
	static void ReleaseVertexDeclaration();
};


struct TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_VERTEX
{
	float x, y, z;
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;
	float tu2,tv2;
	
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2 | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TANGENT;		
	}
	static inline unsigned int GetFVF(){

		return 0 ;
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
	static void ReleaseVertexDeclaration();
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_VERTEX
{
	float x, y, z;
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;
	float tu2,tv2;
	
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2 | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_RNM;		
	}
	static inline unsigned int GetFVF(){

		return 0 ;
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
	static void ReleaseVertexDeclaration();
};

struct TNORMAL_TANGENT_BINORMAL_TEXTURE1_TEXTURE2_RNM_SPECULAR_VERTEX
{
	float x, y, z;
	float nx, ny, nz, nw;
	float tangentx, tangenty, tangentz, tangentw;
	float binormalx, binormaly, binormalz, binormalw;
	float tu,tv;
	float tu2,tv2;
	
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TEXTURE1 | VERTEX_TYPE_TEXTURE2 | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_RNM | VERTEX_TYPE_RNM_SPEC;		
	}
	static inline unsigned int GetFVF(){

		return 0 ;
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
	static void ReleaseVertexDeclaration();
};


#endif
