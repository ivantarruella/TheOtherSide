#pragma once
#ifndef __CLASS_EFFECTMANAGER_H__
#define __CLASS_EFFECTMANAGER_H__

#include "Utils/MapManager.h"
#include "Matrix44.h"
#include "Vector3.h"
#include "Effect.h"

class CEffectTechnique;

class CEffectManager : public CMapManager<CEffectTechnique>
{
private:
	typedef std::map<int,std::string> TDefaultTechniqueEffectMap;
	TDefaultTechniqueEffectMap m_DefaultTechniqueEffectMap;
	Mat44f m_WorldMatrix, m_ProjectionMatrix, m_ViewMatrix,	m_ViewProjectionMatrix;
	Mat44f m_LightViewMatrix, m_ShadowProjectionMatrix;
	Vect3f m_CameraEye;
	CMapManager<CEffect> m_Effects;
	
	CEffectTechnique *m_StaticMeshTechnique;
	CEffectTechnique *m_AnimatedModelTechnique;
	std::string m_Path;

	float m_fMirrorDarkCoef;
	float m_fMirrorAnimCoef;

public:
	CEffectManager();
	~CEffectManager();

	const Mat44f & GetWorldMatrix() const{return m_WorldMatrix;}
	const Mat44f & GetProjectionMatrix() const{return m_ProjectionMatrix;}
	const Mat44f & GetViewMatrix() const{return m_ViewMatrix;}
	const Mat44f & GetViewProjectionMatrix(){return m_ViewProjectionMatrix;}
	const Vect3f & GetCameraEye(){return m_CameraEye;}
	const Mat44f & GetLightViewMatrix() const{return m_LightViewMatrix;}
	const Mat44f & GetShadowProjectionMatrix(){return m_ShadowProjectionMatrix;}

	void ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix, const Vect3f &CameraEye);
	void SetWorldMatrix(const Mat44f &Matrix);
	void SetProjectionMatrix(const Mat44f &Matrix);
	void SetViewMatrix(const Mat44f &Matrix);
	void SetViewProjectionMatrix(const Mat44f &ViewProjectionMatrix);
	void SetLightViewMatrix(const Mat44f &Matrix);
	void SetShadowProjectionMatrix(const Mat44f &Matrix);
	void SetCameraEye(const Vect3f &CameraEye);

	float GetMirrorDarkCoef();
	void SetMirrorDarkCoef(float coef);
	float GetMirrorAnimCoef();
	void SetMirrorAnimCoef(float coef);

	void Load(const std::string &FileName);
	void Reload();

	std::string GetTechniqueEffectNameByVertexDefault(unsigned short VertexType);
	size_t GetMaxLights() const;

	CEffect * GetEffect(const std::string &Name);
	CEffectTechnique * GetEffectTechnique(const std::string &Name);
	CEffectTechnique * GetStaticMeshTechnique() const;

	void SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique);
	CEffectTechnique * GetAnimatedModelTechnique() const;
	void SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique);
	void CleanUp();
};

#endif