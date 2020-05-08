#ifndef BULLET_MANAGER_CLASS
#define BULLET_MANAGER_CLASS

#include <vector>

#include "Utils\VectorMapManager.h"
#include "Bullet.h"
#include "Character.h"

class CRenderManager;
class COmniLight;
class CFrustum;
class CSoldier;

class CBulletManager: public CVectorMapManager<CBullet>
{
public:
	CBulletManager(void):m_vShotLightsVector(NULL),m_iCounter(0),m_pBillboard(NULL){m_particles.clear();}
	~CBulletManager(void);
	void Update(float);
	void AddBullet(Vect3f &_Position, Vect3f &_Direction, CCharacter* player, float _Speed=BULLET_SPEED, float _Damage = BULLET_DAMAGE);
	void AddParticles(CSoldier* _pSoldier, Vect3f &_Position, bool bIsHeadShoot, const std::string& _Name, float size, int world);
	void Render (CRenderManager *, const CFrustum* Frustum);
	bool Init();
	void Destroy();
	COmniLight* GetLight();
	std::string GetBulletName();

private:
	typedef std::map<std::string, std::pair<CSoldier*, std::pair<float, float> > >	tdParticlesMap;

	int m_iCounter;
	std::vector<COmniLight *> m_vShotLightsVector;

	tdParticlesMap m_particles;

	CBillboard* m_pBillboard;
};

#endif