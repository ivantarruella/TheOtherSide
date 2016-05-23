#ifndef BULLET_MANAGER_CLASS
#define BULLET_MANAGER_CLASS

#include <vector>

#define NUM_MAX_LIGHTS 20

#include "Utils\VectorMapManager.h"
#include "Bullet.h"
#include "Character.h"

class CRenderManager;
class COmniLight;
class CFrustum;

class CBulletManager: public CVectorMapManager<CBullet>
{
public:
	CBulletManager(void):m_vShotLightsVector(NULL),m_iCounter(0){}
	~CBulletManager(void);
	void Update(float);
	void AddBullet(Vect3f &_Position, Vect3f &_Direction, CCharacter* player, float _Speed=BULLET_SPEED, float _Damage = BULLET_DAMAGE);
	void Render (CRenderManager *, const CFrustum* Frustum);
	bool Init();
	void Destroy();
	COmniLight* GetLight();
	std::string GetBulletName();

private:
	int m_iCounter;
	std::vector<COmniLight *> m_vShotLightsVector;
};

#endif