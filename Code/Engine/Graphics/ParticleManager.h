#pragma once
#include "Utils/VectorMapManager.h"
#include "Utils/MapManager.h"
#include "ParticleEmitter.h"


class CParticleManager: public CMapManager<CParticleEmitter>
{
private:
	std::string m_Path;
	CVectorMapManager<CParticleEmitter> m_ParticleCores;
	int m_World;

public:
	CParticleManager(void);
	~CParticleManager(void);
	
	void Destroy();
	void Reload();
	void Load(const std::string &FileName);
	
	CParticleEmitter* GetParticleEmitter(const std::string& name);
	
	void AddParticleEmitterInstance(const std::string& core_id, const std::string& instance_name, const Vect3f& Pos1, const Vect3f& Pos2, float size, int World);
	void RemoveParticleEmitterInstance(const std::string& instance_name);

	void Render(CRenderManager *RenderManager, const CFrustum* Frustum);
	void Update(float ElapsedTime);

	void SetWorld(int world);
};

