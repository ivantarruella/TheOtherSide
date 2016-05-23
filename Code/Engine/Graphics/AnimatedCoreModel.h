#ifndef __CLASS_ANIMATEDCOREMODEL_H__
#define __CLASS_ANIMATEDCOREMODEL_H__
#pragma once


#include <string>
#include <vector>

#include "cal3d\coremodel.h"
#include "cal3d\hardwaremodel.h"
#include "cal3d\cal3d.h"
#include "EffectTechnique.h"

#define MAXBONES 29

class CRenderableVertexs;

class CAnimatedCoreModel
{
private:
	CalCoreModel* m_CalCoreModel;
	//Objeto que se crea y destruye
	CalHardwareModel *m_CalHardwareModel;
	//VB e IB
	CRenderableVertexs *m_RenderableVertexs;

	std::string m_Name;
	//std::string m_MeshFilename;
	std::vector<std::string> m_vMeshFilename;
	std::string m_SkeletonFilename;
	std::vector<std::string> m_TextureFilenameList;
	std::string m_Path;
	CEffectTechnique* m_pDeffaultEffectTechnique;

	// calculo bounding box y bounding sphere
	Vect3f m_VtxMin, m_VtxMax, m_VtxCenter,m_VtxBBSize;
	float m_fRadius;

public:
	CAnimatedCoreModel();
	~CAnimatedCoreModel();

	CalCoreModel *GetCoreModel( ) { return m_CalCoreModel; }
	const std::string & GetTextureName( size_t id );
	const std::string & GetCoreModelPath() { return m_Path; }
	size_t GetNumTextures( ) const { return m_TextureFilenameList.size(); }
	void Load(const std::string &Path);
	
	CalHardwareModel * GetCalHardwareModel() const { return m_CalHardwareModel;}

	//Para VB e IB
	bool LoadVertexBuffer();
	CRenderableVertexs * GetRenderableVertexs() const {	return m_RenderableVertexs;	}

	CEffectTechnique* GetDeffaultEffectTechnique(){ return m_pDeffaultEffectTechnique;}
	void SetDeffaultEffectTechnique(CEffectTechnique * EffectTechnique){ m_pDeffaultEffectTechnique=EffectTechnique; }

	// BB 
	const Vect3f& GetVtxMin() { return m_VtxMin; }
	const Vect3f& GetVtxMax() { return m_VtxMax; }
	const Vect3f& GetBoxSize();
	// BS
	const Vect3f& GetVtxCenter() { return m_VtxCenter; }
	float GetRadius() { return m_fRadius; }

private:	
	bool LoadMesh(const std::string &MeshFileName);
	bool LoadSkeleton();
	bool LoadAnimation(const std::string &Name, const std::string &Filename);
	void CalcAnimatedCoreModelBB();
	void Release();
};


#endif
