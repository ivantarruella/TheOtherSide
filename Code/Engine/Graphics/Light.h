#pragma once
#ifndef __CLASS_LIGHT_H__
#define __CLASS_LIGHT_H__

#include "Object3D.h"
#include "Named.h"
#include "Color.h"
#include "Camera\Frustum.h"
#include "XML\XMLTreeNode.h"
#include <string>
#include <vector>

#define DYNAMIC_SHADOW_MAP_STAGE	6
#define STATIC_SHADOW_MAP_STAGE		7
#define	SHADOW_MAP_MASK_STAGE		8
#define CUBE_SHADOW_MAP_STAGE		9

#define DYNAMIC_SHADOW_MAP_STAGE2	10
#define STATIC_SHADOW_MAP_STAGE2	11
#define	SHADOW_MAP_MASK_STAGE2		12
#define CUBE_SHADOW_MAP_STAGE2		13

class CRenderManager;
class CTexture;
class CCubeTexture;
class CRenderableObjectsManager;
class CEffect;
class CInstanceMesh;
class CTexture;
class CFrustum;
class CParticleEmitter;

class CLight : public CObject3D, public CNamed
{
public:
	enum TLightType	{ 
		OMNI=0, 
		DIRECTIONAL, 
		SPOT	
	};

protected:
	CColor m_Color;
	TLightType m_Type;
	bool m_RenderShadows;
	float m_StartRangeAttenuation, m_CurrStartRangeAtt;
	float m_EndRangeAttenuation, m_CurrEndRangeAtt;
	static TLightType GetLightTypeByName(const std::string &StrLightType);
	int m_World;
	bool m_bActive;

	// Intermitencia luces
	bool m_bIsIntermittent;
	float m_fTime, m_fChange, m_fTMin, m_fTMax;
	
	// Intesidad variable luces
	float m_fVarTime;
	float m_fOffTime;
	bool m_bVarIntensity;

	// Atenuación luces
	bool m_bVarAtt;
	float m_fAttTime;
	float m_fCurrAttTime;
	float m_fAttInc;
	bool m_bRisingAtt;

	//Shadow map
	bool m_GenerateDynamicShadowMap;
	bool m_GenerateStaticShadowMap;
	bool m_MustUpdateStaticShadowMap;
	bool m_SoftShadow;
	CTexture *m_StaticShadowMap, *m_DynamicShadowMap, *m_ShadowMaskTexture;
	CTexture *m_DynamicShadowMapBlurH, *m_DynamicShadowMapBlurV; 
	CCubeTexture *m_CubeTextNear, *m_CubeTextMedium, *m_CubeTextFar;
	std::vector<CRenderableObjectsManager *> m_StaticShadowMapRenderableObjectsManagers,m_DynamicShadowMapRenderableObjectsManagers;
	Mat44f m_ViewShadowMap, m_ProjectionShadowMap;
	CFrustum m_LightFrustum;
	bool m_BlurShadowMap;

	// Malla asociada a una luz
	CInstanceMesh *m_LightMesh;
	CTexture *m_TextureON, *m_TextureOFF;

	// Propiedades de movimiento
	bool m_bMoving;
	int  m_iMaxMoves;
	float m_fIncMove;

	// Particulas
	std::string m_EmitterName;
	CParticleEmitter* m_Emitter;

public:
	CLight();
	virtual ~CLight();
	
	virtual void Update(float ElapsedTime);
	virtual void RenderDebug(CRenderManager *RM)=0;
	virtual bool isVisible(CRenderManager &RM, const CFrustum* Frustum)=0;
	virtual float GetRadius()=0;

	void SetParameters(CXMLTreeNode &atts, const std::string & shadows_type);
	void SetColor(const CColor &Color);
	const CColor & GetColor() const;
	void SetStartRangeAttenuation(const float StartRangeAttenuation);
	float GetStartRangeAttenuation() const;
	void SetEndRangeAttenuation(const float EndRangeAttenuation);
	float GetEndRangeAttenuation() const;
	void SetRenderShadows(const bool RenderShadows){m_RenderShadows=RenderShadows;}
	bool GetRenderShadows() const;
	void SetType(const TLightType Type) {m_Type = Type;}
	TLightType GetType() const {return m_Type;}
	const CFrustum& GetLightFrustum() { return m_LightFrustum; }
	CInstanceMesh *GetLightMesh() { return m_LightMesh; }

	void SetActive(bool active) { m_bActive = active; }
	bool GetActive() { return m_bActive; }

	void SetWorld(int world) { m_World = world; }
	int GetWorld() { return m_World; }
	bool isAmbient();

	void SetVarTime(float val) { m_fVarTime = val; }
	void SetVarIntensity(bool val) { m_bVarIntensity = val; }	

	// Movimiento
	void SetStartMove(bool move) { m_bMoving = move; }

	//Shadow Map
	virtual void SetShadowMap(CRenderManager *RM)=0;
	virtual void BeginRenderEffectManagerShadowMap(CEffect *Effect, size_t idx);
	
	void SetGenerateDynamicShadowMap(bool GenerateDynamicShadowMap);
	bool GetGenerateDynamicShadowMap() const;
	void SetGenerateStaticShadowMap(bool GenerateStaticShadowMap);
	bool GetGenerateStaticShadowMap() const;
	void SetMustUpdateStaticShadowMap(bool MustUpdateStaticShadowMap);
	bool GetMustUpdateStaticShadowMap() const;
	CTexture * GetStaticShadowMap() const;
	CTexture * GetDynamicShadowMap() const;
	CTexture * GetShadowMaskTexture() const;
	CCubeTexture* GetCubeShadowMap(unsigned int quality) const;
	std::vector<CRenderableObjectsManager *> & GetStaticShadowMapRenderableObjectsManagers();
	std::vector<CRenderableObjectsManager *> & GetDynamicShadowMapRenderableObjectsManagers();
	void GenerateShadowMap(CRenderManager *RM);
	void BlurShadowMapH(CRenderManager *RM);
	void BlurShadowMapV(CRenderManager *RM);
	void DeleteShadowMap(CEffect *Effect, size_t idx);
	void RenderShadowMap(CRenderManager *RM);
	const Mat44f & GetViewShadowMap() const;
	const Mat44f & GetProjectionShadowMap() const;
	void GetShadowsType(const std::string& shadows_type, unsigned int& width, unsigned int& height);
	unsigned int calcShadowMapQuality();
};

#endif