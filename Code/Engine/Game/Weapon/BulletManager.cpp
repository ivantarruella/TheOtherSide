#include "BulletManager.h"
#include "BillboardManager.h"
#include "ParticleManager.h"
#include "RenderManager.h"
#include "include\PhysicsManager.h"
#include "Soldier.h"
#include <cstring>
#include "LightManager.h"
#include "Base.h"

#define NUM_MAX_LIGHTS			20
#define REMOVE_DAMAGE_TIME		0.75f
#define REMOVE_HEADSHOT_TIME	1.75f

CBulletManager::~CBulletManager(void)
{
	CBulletManager::Destroy();
}

bool CBulletManager::Init()
{
	m_iCounter = 0;
	m_vShotLightsVector.clear();
	return true;
}

void CBulletManager::Destroy()
{
	m_vShotLightsVector.clear();
	CVectorMapManager::Destroy();

	for (tdParticlesMap::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
		CORE->GetParticleManager()->RemoveParticleEmitterInstance(it->first);
	m_particles.clear();
}

void CBulletManager::Update(float _ElapsedTime)
{
	CBullet * l_pBullet = NULL;
	for (uint32 i=0 ; i < m_ResourcesVector.size(); i++ )
	{
		l_pBullet = this->GetResourceById(i);
		l_pBullet->Update(_ElapsedTime);
		if(l_pBullet->IsEnded())
		{
			m_ResourcesVector.erase(m_ResourcesVector.begin()+i);
			m_ResourcesMap.erase(l_pBullet->GetName());
			CHECKED_DELETE(l_pBullet);
		}
	}

	//damage particles
	for (tdParticlesMap::iterator it = m_particles.begin(); it != m_particles.end();) {
		CSoldier* pSoldier = it->second.first;
		if (it->second.second.second <= 0.f) {
			CORE->GetParticleManager()->RemoveParticleEmitterInstance(it->first);
			it = m_particles.erase(it);
		} 
		else {
			if (pSoldier != NULL) {
				bool bSecondFloor = pSoldier->GetPosition().y >= SECON_FLOOR_HEIGHT;
				float fy = (bSecondFloor ? (pSoldier->GetPosition().y + it->second.second.first - SECON_FLOOR_HEIGHT) : (pSoldier->GetPosition().y + it->second.second.first));
				Vect3f new_pos = Vect3f(pSoldier->GetPosition().x, fy, pSoldier->GetPosition().z);
				CParticleEmitter* pEmitter = CORE->GetParticleManager()->GetParticleEmitter(it->first);
				if (pEmitter != NULL)
					pEmitter->SetPos(new_pos);
			}
			
			it->second.second.second -= _ElapsedTime;
			++it;
		}
	}
}

void CBulletManager::AddBullet(Vect3f &_Position, Vect3f &_Direction, CCharacter* player, float _Speed, float _Damage)
{
	CBillboard *l_pBillboard = CORE->GetBillboardManager()->GetBillboardCore("bLaserHalo");
	CBullet * l_pBullet = new CBullet (*l_pBillboard);

	std::string l_sBulletName = GetBulletName();

	l_pBullet->SetPos(_Position+Vect3f(0.0f, 0.025f, 0.0f));
	l_pBullet->SetDirection(_Direction);
	l_pBullet->SetBulletSpeed(_Speed);
	l_pBullet->SetFirstPos(_Position);
	l_pBullet->SetDamage(_Damage);
	l_pBullet->SetName(l_sBulletName);
	l_pBullet->SetLight(GetLight());

	l_pBullet->Init(player);
	m_iCounter++;
	AddResource(l_sBulletName,l_pBullet);
}

void CBulletManager::AddParticles(CSoldier* _pSoldier, Vect3f &_Position, bool bIsHeadShoot, const std::string& _Name, float size, int world)
{
	// damage particles
	m_particles[_Name] = std::pair<CSoldier*, std::pair<float, float> >(_pSoldier, std::pair<float, float>(_Position.y, bIsHeadShoot ? REMOVE_HEADSHOT_TIME : REMOVE_DAMAGE_TIME));
	CORE->GetParticleManager()->AddParticleEmitterInstance(bIsHeadShoot ? "soldier_headshoot" : "soldier_damage", _Name, _Position, _Position, size, world); 
	CParticleEmitter* l_Particles = CORE->GetParticleManager()->GetParticleEmitter(_Name);
	if (l_Particles!=NULL) {
		l_Particles->SetEnabled(true);
	}
}

std::string CBulletManager::GetBulletName()
{

	std::string l_szBulletName = "Bullet";
	char l_str[100]="";
	sprintf_s(l_str,100,"%d",m_iCounter);
	l_szBulletName.append(l_str);
	return l_szBulletName;
}

COmniLight* CBulletManager::GetLight()
{
	COmniLight* l_pLight=NULL;
	if(m_iCounter<NUM_MAX_LIGHTS){
		CColor l_ColGreen = CColor(81.f/255.f,238.f/255.f,19.f/255.f);
		l_pLight = new COmniLight ();
		l_pLight->SetColor(l_ColGreen);
		l_pLight->SetStartRangeAttenuation(0.f);
		std::string l_sBulletName = GetBulletName();
		if(l_pLight!=NULL)
		{
			CORE->GetLightManager()->AddResource(l_sBulletName,l_pLight);
			m_vShotLightsVector.push_back(l_pLight);
		}

	}else
	{
		int l_iIndex = m_iCounter%NUM_MAX_LIGHTS;
		l_pLight= m_vShotLightsVector.at(l_iIndex);
		if(l_pLight==NULL)
			return NULL;

		for(int i = l_iIndex+1; l_pLight->GetEnabled() && i < l_iIndex+NUM_MAX_LIGHTS;i++)
		{
			l_pLight= m_vShotLightsVector.at(i%NUM_MAX_LIGHTS);
			if(l_pLight==NULL)
				return NULL;
		}
	}	
	if(l_pLight !=NULL)
	{
		l_pLight->SetEnabled(true);
		l_pLight->SetVisible(true);
		l_pLight->SetActive(true);
		l_pLight->SetStartRangeAttenuation(0.f);
		l_pLight->SetEndRangeAttenuation(2.f);
	}

	return l_pLight;
}


void CBulletManager::Render (CRenderManager * RenderManager, const CFrustum* Frustum)
{
	if (m_ResourcesVector.size()) 
	{
		LPDIRECT3DDEVICE9 l_Device=RenderManager->GetDevice();

		//PreRender
		//Activa el alpha
		l_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		l_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		l_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		l_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		//End PreRender

		for(uint16 i = 0; i<m_ResourcesVector.size();i++){
	
			m_ResourcesVector.at(i)->Render(RenderManager, Frustum);
		}

		//PostRender
		//Desactiva el alpha
		//l_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
		//l_Device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
		l_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		l_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		l_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		//end PostRender
	}
}



