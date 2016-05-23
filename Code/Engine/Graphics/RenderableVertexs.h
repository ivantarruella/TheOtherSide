#pragma once
#ifndef __CLASS_RENDERABLEVERTEXS_H__
#define __CLASS_RENDERABLEVERTEXS_H__

#include "d3dx9.h"
#include "Base.h"

class CRenderManager;
class CEffectTechnique;

class CRenderableVertexs
{
protected:
	LPDIRECT3DVERTEXBUFFER9 m_VB;
	LPDIRECT3DINDEXBUFFER9 m_IB;
	size_t m_IndexCount, m_VertexCount;

public:
	CRenderableVertexs()
		: m_VB(NULL), m_IB(NULL), m_IndexCount(0), m_VertexCount(0)
	{
	}
	virtual ~CRenderableVertexs() 
	{
		CHECKED_RELEASE(m_VB);
		CHECKED_RELEASE(m_IB);
	}
	
	virtual bool Render(CRenderManager *RM) const = 0;
	virtual bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique) const = 0;
	virtual bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique, int BaseVertexIndex, uint16 MinVertexIndex, uint16 NumVertices, uint16 startIndex, uint16 FaceCount) const = 0;

	virtual inline size_t GetFacesCount() const
	{
		return m_IndexCount/3;
	}
	virtual inline size_t GetVertexsCount() const
	{
		return m_VertexCount;
	}

	virtual inline unsigned short GetVertexType() const = 0;
	virtual inline size_t GetVertexSize() const = 0;
	virtual inline size_t GetIndexSize() const = 0;
};

#endif