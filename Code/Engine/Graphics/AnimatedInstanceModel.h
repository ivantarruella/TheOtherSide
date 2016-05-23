#ifndef __CLASS_ANIMATEDINSTANCEMODEL_H__
#define __CLASS_ANIMATEDINSTANCEMODEL_H__
#pragma once

#include "RenderableObject.h"
#include "AnimatedCoreModel.h"
#include "XML\XMLTreeNode.h"
#include <vector>

class CRenderManager;
class CalModel;
class CalAnimation;
class CTexture;
class CRenderableVertexs;
class CRenderableObjectTechnique;
class CLight;
class CFrustum;

#define MAX_DIST_RENDER		100.0f	// distancia maxima de pintado de un modelo animado

class CAnimatedInstanceModel : public CRenderableObject
{
private:
	CalModel *m_CalModel;
	CAnimatedCoreModel *m_AnimatedCoreModel;
	std::vector<CTexture *> m_TextureList;
	std::vector<CTexture *> m_NormalTextureList;
	std::vector<CRenderableObjectTechnique *> m_RenderableObjectTechniques;
	std::vector<unsigned int>m_VertexTypes;
	std::string m_RenderableObjectTechniqueName;

	//RenderModelBySoftware() necesita estas variables miembro:
	//LPDIRECT3DVERTEXBUFFER9 m_pVB;	
	//LPDIRECT3DINDEXBUFFER9 m_pIB;
	//int m_NumVtxs;
	//int m_NumFaces;

public:
	CAnimatedInstanceModel(){}
	CAnimatedInstanceModel(const std::string &Name);
	CAnimatedInstanceModel(const CXMLTreeNode& XmlData);
	CAnimatedInstanceModel(const Vect3f &pos, const std::string &CoreName, const std::string &Name);
	~CAnimatedInstanceModel();

	void Render(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender);
	void RenderDebug(CRenderManager *RM);
	void RenderShadow(CRenderManager *RM, CLight* Light);
	void RenderReflected(CRenderManager *RM, CRenderableObject* Mesh, const CFrustum* Frustum);
	void RenderModelBySoftware(CRenderManager *RM);
	void RenderModelByHardware(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender);
	virtual void Update(float ElapsedTime);
	void Initialize(CAnimatedCoreModel *AnimatedCoreModel);
	void InitD3D(CRenderManager *RM);
	void Destroy();
	
	void ExecuteAction(int Id,  float Time, float TimeOut);
	void RemoveAction(int Id);
	void BlendCycle(int Id, float Time, float weight = 1.0f);
	void ClearCycle(int Id, float Time);
	float GetAnimationDuration();
	std::vector<CalAnimation *> & GetAnimationVector();

	void ClearAllAnims();

	bool GetRenderableObjectTechnique();
	CalModel* GetCalModel() { return m_CalModel; }

	const Vect3f& GetBBSize()	{ return m_AnimatedCoreModel->GetBoxSize();}

private:
	//bool LoadVertexBuffer(CRenderManager *RM);		// RenderModelBySoftware()
	void LoadTextures();
	void DrawAnimatedModel(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender);
	void DrawAnimatedModelShadow(CRenderManager *RM, const CFrustum* Frustum, bool forwardRender);
};


#endif