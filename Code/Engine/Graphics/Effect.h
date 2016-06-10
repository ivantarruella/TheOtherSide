#pragma once
#ifndef __CLASS_EFFECT_H__
#define __CLASS_EFFECT_H__

#include <string>
#include "Vector3.h"
#include "EffectsDef.h"
#include "Texture.h"
#include "d3dx9.h"

class CLight;
class CFrustum;

class CEffect
{
private:
	std::string		m_FileName;
	LPD3DXEFFECT	m_Effect;

	bool			m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
	int				m_LightsType[MAX_LIGHTS_BY_SHADER];
	float			m_LightsAngle[MAX_LIGHTS_BY_SHADER];
	float			m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
	float			m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	float			m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsPosition[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsDirection[MAX_LIGHTS_BY_SHADER];
	Vect3f			m_LightsColor[MAX_LIGHTS_BY_SHADER];

	bool			m_UseDynamicShadowmap;
	bool			m_UseStaticShadowmap;
	bool			m_UseShadowMaskTexture;


	D3DXHANDLE		m_WorldMatrixParameter, 
		m_ViewMatrixParameter, 
		m_ProjectionMatrixParameter;
	D3DXHANDLE		m_InverseWorldMatrixParameter, 
		m_InverseViewMatrixParameter, 
		m_InverseProjectionMatrixParameter;
	D3DXHANDLE		m_WorldViewMatrixParameter,
		m_ViewProjectionMatrixParameter,
		m_WorldViewProjectionMatrixParameter,
		m_ViewProjectionInverseParameter;
	
	D3DXHANDLE		m_ViewToLightProjectionMatrixParameter;
	D3DXHANDLE		m_CameraPositionParameter;
	D3DXHANDLE		m_BonesParameter;
	D3DXHANDLE		m_TimeParameter;
	D3DXHANDLE		m_MirrorDarkCoefParameter, m_MirrorAnimCoefParameter;
	D3DXHANDLE		m_LightEnabledParameter, 
		m_LightsTypeParameter,
		m_LightsPositionParameter, 
		m_LightsDirectionParameter,
		m_LightsAngleParameter, 
		m_LightsColorParameter,
		m_AmbientLightParameter;
	D3DXHANDLE		m_LightsFallOffParameter,
		m_LightsStartRangeAttenuationParameter,
		m_LightsEndRangeAttenuationParameter;

	D3DXHANDLE      m_UseShadowMaskTextureParameter,
		m_UseStaticShadowmapParameter,
		m_UseDynamicShadowmapParameter;

	D3DXHANDLE      m_UseShadowMaskTextureParameter2,
		m_UseStaticShadowmapParameter2,
		m_UseDynamicShadowmapParameter2;

	D3DXHANDLE		m_UseRenderTargetSizeParameter;


	void SetNullParameters();
	void GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE	&l_Handle);		
	bool LoadEffect();
	void Unload();

public:
	CEffect();
	~CEffect();

	void SetLights(size_t NumOfLights, const Vect3f& center, const CFrustum* Frustum);
	bool SetLight(size_t Index, CLight* Light);
	bool Load(const std::string &FileName);
	bool Reload();
	void SetShadowMapParameters(bool UseShadowMaskTexture, bool UseStaticShadowmap, bool UseDynamicShadowmap, size_t idx);

	//DirectX Methods Interface
	LPD3DXEFFECT GetD3DEffect() const;
	D3DXHANDLE GetTechniqueByName(const std::string &TechniqueName);		

	//Getters parametros luces
	int * GetLightsType()
	{
		return m_LightsType;
	}
	float * GetLightsAngle()
	{
		return m_LightsAngle;
	}
	float * GetLightsFallOff()
	{
		return m_LightsFallOff;
	}
	float *	GetLightsStartRangeAttenuation()
	{
		return m_LightsStartRangeAttenuation;
	}
	float * GetLightsEndRangeAttenuation()
	{
		return m_LightsEndRangeAttenuation;
	}
	Vect3f * GetLightsDirection()
	{
		return m_LightsDirection;
	}
	Vect3f * GetLightsPosition()
	{
		return m_LightsPosition;
	}
	Vect3f * GetLightsColor()
	{
		return m_LightsColor;
	}

	//Getters handlers
	D3DXHANDLE	GetWorldMatrixParameter(){
		return m_WorldMatrixParameter;
	}
	D3DXHANDLE GetViewMatrixParameter(){
		return m_ViewMatrixParameter;
	}
	D3DXHANDLE GetProjectionMatrixParameter(){
		return m_ProjectionMatrixParameter;
	}
	D3DXHANDLE	GetInverseWorldMatrixParameter(){
		return m_InverseWorldMatrixParameter;
	}
	D3DXHANDLE GetInverseViewMatrixParameter(){
		return m_InverseViewMatrixParameter;
	}
	D3DXHANDLE GetInverseProjectionMatrixParameter(){
		return m_InverseProjectionMatrixParameter;
	}
	D3DXHANDLE GetWorldViewMatrixParameter(){
		return	m_WorldViewMatrixParameter;
	}
	D3DXHANDLE GetViewProjectionMatrixParameter(){
		return m_ViewProjectionMatrixParameter;
	}
	D3DXHANDLE GetWorldViewProjectionMatrixParameter(){
		return m_WorldViewProjectionMatrixParameter;
	}
	D3DXHANDLE GetViewProjectionInverseMatrixParameter(){
		return m_ViewProjectionInverseParameter;
	}
	D3DXHANDLE GetViewToLightProjectionMatrixParameter(){
		return m_ViewToLightProjectionMatrixParameter;
	}
	D3DXHANDLE GetCameraPositionParameter(){
		return m_CameraPositionParameter;
	}
	D3DXHANDLE GetBonesParameter(){
		return m_BonesParameter;
	}
	D3DXHANDLE GetTimeParameter(){
		return m_TimeParameter;
	}
	D3DXHANDLE GetMirrorDarkCoefParameter(){
		return m_MirrorDarkCoefParameter;
	}
	D3DXHANDLE GetMirrorAnimCoefParameter(){
		return m_MirrorAnimCoefParameter;
	}
	D3DXHANDLE GetLightEnabledParameter(){
		return m_LightEnabledParameter;
	}
	D3DXHANDLE GetLightsTypeParameter(){
		return m_LightsTypeParameter;
	}
	D3DXHANDLE GetLightsPositionParameter(){
		return m_LightsPositionParameter;
	}
	D3DXHANDLE GetLightsDirectionParameter(){
		return m_LightsDirectionParameter;
	}
	D3DXHANDLE GetLightsAngleParameter(){
		return m_LightsAngleParameter; 
	}
	D3DXHANDLE GetLightsColorParameter(){
		return m_LightsColorParameter;
	}
	D3DXHANDLE GetLightsAmbientParameter(){
		return m_AmbientLightParameter;
	}
	D3DXHANDLE GetLightsFallOffParameter() {		
		return m_LightsFallOffParameter;
	}
	D3DXHANDLE GetLightsStartRangeAttenuationParameter() {
		return m_LightsStartRangeAttenuationParameter;
	}
	D3DXHANDLE GetLightsEndRangeAttenuationParameter(){
		return m_LightsEndRangeAttenuationParameter;
	}

	D3DXHANDLE GetRenderTargetSizeParameter(){
		return m_UseRenderTargetSizeParameter;
	}
};

#endif