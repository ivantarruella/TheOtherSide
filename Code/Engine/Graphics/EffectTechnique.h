#pragma once
#ifndef __CLASS_EFFECTTECHNIQUE_H__
#define __CLASS_EFFECTTECHNIQUE_H__

#include <string>
#include "Effect.h"
#include "xml/XMLTreeNode.h"

class CEffectTechnique
{
private:
	bool	m_UseCameraPosition;
	bool	m_UseInverseProjMatrix;
	bool	m_UseInverseViewMatrix;
	bool	m_UseInverseWorldMatrix;
	bool	m_UseLights;
	int		m_NumOfLights;
	bool	m_UseLightAmbientColor;
	bool	m_UseProjMatrix;
	bool	m_UseViewMatrix;
	bool	m_UseWorldMatrix;
	bool	m_UseWorldViewMatrix;
	bool	m_UseWorldViewProjectionMatrix;
	bool	m_UseViewProjectionMatrix;
	bool	m_UseViewToLightProjectionMatrix;
	bool	m_UseViewProjectionInverseMatrix;
	bool	m_UseTime;
	bool    m_UseRenderTargetSize;
	CEffect *m_Effect;
	D3DXHANDLE	m_D3DTechnique;
	std::string m_TechniqueName;


public:
	CEffectTechnique(const CXMLTreeNode& xml_data);
	~CEffectTechnique();

	inline CEffect * GetEffect() const {return m_Effect;}
	bool BeginRender();

	bool Refresh();
	bool setLightMap(CTexture &Texture);
	//int getNumLights(){return m_NumOfLights;}	

	//DirectX Methods Interface
	D3DXHANDLE GetD3DTechnique();  
};

#endif