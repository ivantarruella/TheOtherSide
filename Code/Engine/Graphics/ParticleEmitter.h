#pragma once
#ifndef INC_PARTICLE_EMITER_H_
#define INC_PARTICLE_EMITER_H_

#define NUMPARTICLES 1000

#include "Math\Vector3.h"
#include "Math\Color.h"
#include <string>
#include "Particle.h"
#include "utils\RecyclingArray.h"


class CCamera;
class CTexture;
class CXMLTreeNode;
class CFrustum;

class CParticleEmitter : public CNamed
{
public:
	CParticleEmitter()
	:m_vPos(0.0f, 0.0f, 0.0f), m_fMinEmitRate(0.0f), m_fMaxEmitRate(0.0f), m_fAngle(0.0f)
	,m_fMinTimeLife(0.0f), m_fMaxTimeLife(0.0f), m_bGravity(false)
	,m_fNumNewPartsExcess(0.0f), m_sTexture(NULL), m_Particles(NUMPARTICLES), m_bEmitterEnabled(true)
	{
	};

	/*CParticleEmitter(CXMLTreeNode &parser):m_Particles(NUMPARTICLES){};
	CParticleEmitter(CParticleEmitter& other):m_Particles(NUMPARTICLES){};*/
	virtual ~CParticleEmitter(){};

	void Update(float deltaTime, int world);
	void Render(CRenderManager * RM, int world, const CFrustum* Frustum);
	
	void SetPos(const Vect3f& pos);
	void SetMinRate(float min);
	void SetMaxRate(float max);
	void SetMinTimeLife(float min){m_fMinTimeLife = min;};
	void SetMaxTimeLife(float max){m_fMaxTimeLife = max;};
	void SetGravity(bool gravity){m_bGravity = gravity;};
	void SetAngle(float angle){m_fAngle = angle;};
	void SetType(std::string type){m_Type = type;};
	void SetEnabled(bool enabled) {m_bEmitterEnabled = enabled;};

	const Vect3f& GetPos();
	float GetMinRate();
	float GetMaxRate();
	float GetMinTimeLife(){return m_fMinTimeLife;};
	float GetMaxTimeLife(){return m_fMaxTimeLife;};
	bool GetGravity(){return m_bGravity;};
	float GetAngle(){return m_fAngle;};
	std::string GetType(){return m_Type;};
	bool GetEnabled() {return m_bEmitterEnabled;};
	void SetWorld(int world);

	std::vector<SColor> GetColorVector(){return m_vColors;};
	std::vector<SDirection> GetDirectionVector(){return m_vDirections;};
	std::vector<SSize> GetSizeVector(){return m_vSizes;};
	std::vector<SAceleration> GetAcelerationVector(){return m_vAcelerations;};
	
	void SetNumNewPartsExcess(float num);
	float GetNumNewPartsExcess();	
	void SetTexture(CTexture* texture);
	CTexture* GetTexture();

	//Función virtual pura que calcula la posición de la partícula según el tipo de emisor
	virtual void GenerateParticlePosition(CParticle * particle) =0;

protected:
	void Load(CXMLTreeNode &parser);
	void Copy(CParticleEmitter &other);
	void CreateParticle(float fTimeDelta);

	Vect3f m_vPos;
	std::string m_Type;

	float m_fMinTimeLife;
	float m_fMaxTimeLife;
	
	float m_fMinEmitRate;
	float m_fMaxEmitRate;
	bool m_bGravity;
	float m_fAngle;

	std::vector<SColor> m_vColors;
	std::vector<SDirection> m_vDirections;
	std::vector<SSize> m_vSizes;
	std::vector<SAceleration> m_vAcelerations;

	float m_fNumNewPartsExcess;
	CTexture* m_sTexture;
	CRecyclingArray<CParticle> m_Particles;
	
	bool m_bEmitterEnabled;
	int m_World;
};

#endif

          