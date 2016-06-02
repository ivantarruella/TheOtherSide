#pragma once
#ifndef __CLASS_STATIC_MESH_H__
#define __CLASS_STATIC_MESH_H__

#include <string>
#include <vector>
#include "Math/Vector3.h"
#include "Light.h"

class CTexture;
class CRenderableVertexs;
class CRenderManager;
class CEffectTechnique;
class CRenderableObjectTechnique;
class CFrustum;

class CStaticMesh
{
protected:
	std::vector<CRenderableVertexs*> m_RVs;
	std::vector<std::vector<CTexture *>> m_Textures;
	//std::vector<CEffectTechnique *> m_EffectTechniques;
	std::vector<CRenderableObjectTechnique *> m_RenderableObjectTechniques;
	std::vector<unsigned int>m_VertexTypes;
	std::string m_FileName;
	std::vector<std::string> m_RenderableObjectTechniqueName;
	unsigned int m_NumVertexs, m_NumFaces;
	Vect3f m_vminPoint;
	Vect3f m_vmaxPoint;
	Vect3f m_vcenter;
	float m_fradius;

	std::vector<Vect3f> m_VB;
	std::vector<uint32> m_IB;


public:
	CStaticMesh()
		: m_NumVertexs(0), m_NumFaces(0), m_RVs(NULL), m_Textures(NULL), m_FileName("") ,m_VB(NULL),m_IB(NULL),
			m_vminPoint(Vect3f(0.f,0.f,0.f)), m_vmaxPoint(Vect3f(0.f,0.f,0.f)), m_vcenter(Vect3f(0.f,0.f,0.f)), m_fradius(0.f) //, m_EffectTechniques(NULL)
	{
		m_RenderableObjectTechniques.clear();
		m_IB.clear();
		m_VB.clear();
	}
	~CStaticMesh()
	{
		Release();
	}

	bool Load (const std::string &FileName);
	bool ReLoad ();
	void Render (CRenderManager *RM, const Vect3f& position, const CFrustum* Frustum, bool forwardRender) const;
	void RenderShadow (CRenderManager *RM, const Vect3f& position, const CFrustum* Frustum, CLight* Light) const;
	
	bool GetRenderableObjectTechnique();

	const Vect3f& getStaticMeshCenter() { return m_vcenter; }
	const Vect3f& getStaticMeshMinPoint() { return m_vminPoint; }
	const Vect3f& getStaticMeshMaxPoint() { return m_vmaxPoint; }
	const float getStaticMeshBSRadius() { return m_fradius; }

	const std::vector<Vect3f> & getVB() { return m_VB;}
	const std::vector<uint32> & getIB() { return m_IB;}

	void SetTexture(CTexture* Texture, size_t stage = 0);
	CTexture* GetTexture(size_t stage);

private:
	
	void Release();
};

void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount,size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);
//void VertexCacheOptimisation( void *_VData, uint16 *_IData, size_t _iVCount, size_t _iICount, size_t _iVStride);


#endif