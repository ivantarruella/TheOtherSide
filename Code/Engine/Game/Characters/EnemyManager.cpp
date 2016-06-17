#include "EnemyManager.h"
#include "Monster.h"
#include "Soldier.h"
#include "SpotLight.h"
#include "StateMachine.h"
#include "Renderer\RenderableObjectsLayersManager.h"
#include "RenderableObjectsManager.h"
#include "PhysicsManager.h"
#include "Core.h"

#include "Base.h"

CEnemyManager::CEnemyManager() 
	:   m_fTime(0.0f)
{
	m_DeadBodies.clear();
}

CEnemyManager::~CEnemyManager()
{
	RemoveDeadBodies();

	m_Monsters.clear();
	m_Soldiers.clear();
	m_Conects.clear();
}

void CEnemyManager::Update(float ElapsedTime)
{
	if (m_DeadBodies.size()>0)
	{
		m_fTime += ElapsedTime;
	
		if (m_fTime > 60.0f) 
		{
			RemoveDeadBodies();
			m_fTime = 0.0f;
		}
	}
	else
		m_fTime = 0.0f;
}

void CEnemyManager::AddMonster(std::string name, CMonster* monster)
{
	m_Monsters.insert(std::pair<std::string, CMonster*>(name,monster));
}

void CEnemyManager::AddSoldier(std::string name, CSoldier* soldier)
{	
	m_Soldiers.insert(std::pair<std::string, CSoldier*>(name,soldier));
}

void CEnemyManager::CreateDynamicEnemy(const std::string &InstanceNameSoldier, const Vect3f &posSoldier, const std::string &InstanceNameMonster, const Vect3f &posMonster)
{
	CRenderableObjectsManager* l_enemiesME = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_ME");
	l_enemiesME->AddAnimatedModelInstance("Monster", InstanceNameMonster, posMonster);

	CRenderableObjectsManager* l_enemiesMR = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_MR");
	l_enemiesMR->AddAnimatedModelInstance("Soldier", InstanceNameSoldier, posSoldier);

	CMonster* l_monster = GetMonster(InstanceNameMonster);
	CSoldier* l_soldier = GetSoldier(InstanceNameSoldier);

	SetConection(l_monster, l_soldier);

	l_monster->Freeze();
	l_soldier->Freeze();

	l_enemiesME->GetResource(InstanceNameMonster)->SetVisible(false);
	l_enemiesMR->GetResource(InstanceNameSoldier)->SetVisible(false);
}

void CEnemyManager::ActivateDynamicEnemy(const std::string &InstanceNameSoldier, const std::string &InstanceNameMonster)
{
	CMonster* l_monster = GetMonster(InstanceNameMonster);
	CSoldier* l_soldier = GetSoldier(InstanceNameSoldier);
	if (l_monster && l_soldier) {
		CRenderableObjectsManager* l_enemiesME = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_ME");
		CRenderableObjectsManager* l_enemiesMR = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_MR");
		l_enemiesME->GetResource(InstanceNameMonster)->SetVisible(true);
		l_enemiesMR->GetResource(InstanceNameSoldier)->SetVisible(true);

		l_monster->Unfreeze();
		l_soldier->Unfreeze();
	}
}

void CEnemyManager::RepositionEnemies(bool isChangingToRealWorld)
{
	std::map<CMonster*, CSoldier*>::iterator l_it = m_Conects.begin();
	for(; l_it!=m_Conects.end(); l_it++)
	{
		if(isChangingToRealWorld)
		{
			Vect3f l_pos = l_it->first->GetPosition();
			float l_yaw = l_it->first->GetYaw();
			l_pos.x = (-1) * l_pos.x;
			l_it->second->SetPosition(l_pos);
			l_it->second->SetYaw(l_yaw);
		}
		else{
			Vect3f l_pos = l_it->second->GetPosition();
			float l_yaw = l_it->second->GetYaw();
			l_pos.x = (-1) * l_pos.x;
			l_it->first->SetPosition(l_pos);
			l_it->first->SetYaw(l_yaw);
		}
	}
}

void CEnemyManager::SetConection(CMonster* monster, CSoldier* soldier)
{
	m_Conects.insert(std::pair<CMonster*, CSoldier*>(monster,soldier));
}

void CEnemyManager::ComunicateTimeDead(CSoldier* soldier, float time)
{
	if (soldier!=NULL) {
		CMonster* l_Monster = GetMonsterConnected(soldier);

		if (l_Monster != NULL)
			l_Monster->SetTimeDead(time);
		
		soldier->SetTimeDead(time);
	}
}

void CEnemyManager::RemoveEnemies(CSoldier* soldier)
{
	CMonster* l_Monster = GetMonsterConnected(soldier);
	if (!soldier || !l_Monster)
		return;

	EraseConnection(l_Monster);
	EraseSoldier(soldier);
	EraseMonster(l_Monster);

	CORE->GetPhysicsManager()->ReleasePhysicActor(soldier->GetPhysicActor());
	CORE->GetPhysicsManager()->ReleasePhysicActor(l_Monster->GetPhysicActor());

	if(l_Monster->GetCreatePhysics() && l_Monster->GetPhysicController())
		CORE->GetPhysicsManager()->ReleasePhysicController(l_Monster->GetPhysicController());
	if(soldier->GetCreatePhysics() && soldier->GetPhysicController())
		CORE->GetPhysicsManager()->ReleasePhysicController(soldier->GetPhysicController());

	CRenderableObjectsManager* l_ObjectMan1 = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_ME");
	CRenderableObjectsManager* l_ObjectMan2 = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_MR");

	m_DeadBodies.push_back(l_ObjectMan1->GetResource(l_Monster->GetName()));
	m_DeadBodies.push_back(l_ObjectMan2->GetResource(soldier->GetName()));

	m_fTime = 0.0f;
}

void CEnemyManager::RemoveDeadBodies()
{
	CRenderableObjectsManager* l_ObjectManME = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_ME");
	CRenderableObjectsManager* l_ObjectManMR = CORE->GetRenderableObjectsLayersManager()->GetRenderableObjectManager("enemies_MR");

	for (size_t i=0; i<m_DeadBodies.size(); ++i) {
		bool bRemoved=false;
		bRemoved=l_ObjectManME->RemoveResource(m_DeadBodies[i]->GetName());
		if (!bRemoved) {
			l_ObjectManMR->GetResource(m_DeadBodies[i]->GetName())->SetCreatePhysics(true);
			l_ObjectManMR->RemoveResource(m_DeadBodies[i]->GetName());
		}
	}

	m_DeadBodies.clear();
}

bool CEnemyManager::isCoverBusy(int cover)
{
	std::map<std::string, CSoldier*>::iterator l_it = m_Soldiers.begin();
	for(; l_it!=m_Soldiers.end(); l_it++)
	{
		if(l_it->second->GetCover() == cover)
		{
			return true;
		}
	}
	return false;
}

CMonster* CEnemyManager::GetMonster(std::string name)
{
	std::map<std::string, CMonster*>::iterator l_it;
	l_it = m_Monsters.find(name);
	if(l_it == m_Monsters.end())
		return NULL;
	else
	return l_it->second;
}

CSoldier* CEnemyManager::GetSoldier(int id)
{	
	std::map<std::string, CSoldier*>::iterator l_it;
	unsigned int l_id = id;
	if(l_id >= m_Soldiers.size() || l_id < 0)
		return NULL;

	int n=0;
	for(l_it=m_Soldiers.begin(); l_it!=m_Soldiers.end(); l_it++)
	{
		if(n==id)
		{
			return l_it->second;
		}
		n++;
	}
	return NULL;
	/*l_it = m_Soldiers.at(m_Soldiers.begin()+id);
	if(l_it == m_Soldiers.end())
		return NULL;
	else
		return l_it->second;*/
}

CSoldier* CEnemyManager::GetSoldier(std::string name)
{	
	std::map<std::string, CSoldier*>::iterator l_it;
	l_it = m_Soldiers.find(name);
	if(l_it == m_Soldiers.end())
		return NULL;
	else
		return l_it->second;
}


CSoldier* CEnemyManager::GetSoldier(CPhysicUserData * userData)
{
	std::map<std::string, CSoldier*>::iterator l_it;
	for(l_it=m_Soldiers.begin(); l_it!=m_Soldiers.end(); l_it++)
	{
		if(userData==l_it->second->GetPhysicUserData())
			return l_it->second;
	}
	return NULL;
}

CMonster* CEnemyManager::GetMonsterConnected(CSoldier* soldier)
{
	CMonster* l_aux=NULL;
	std::map<CMonster*, CSoldier*>::iterator l_it = m_Conects.begin();
	for(; l_it!=m_Conects.end(); l_it++)
	{
		if(soldier == l_it->second)
		{
			l_aux = l_it->first;
			//l_it = m_Conects.end();
			return l_aux;
		}
	}
	return l_aux;
}

CSoldier* CEnemyManager::GetSoldierConnected(CMonster* monster)
{
	std::map<CMonster*, CSoldier*>::iterator l_it;
	l_it = m_Conects.find(monster);
	if(l_it == m_Conects.end())
		return NULL;
	else
		return l_it->second;
}

void CEnemyManager::EraseConnection(CMonster* monster)
{
	std::map<CMonster*, CSoldier*>::iterator l_it;
	l_it = m_Conects.find(monster);
	if(l_it != m_Conects.end())
		m_Conects.erase(l_it);
}

void CEnemyManager::EraseSoldier(CSoldier* soldier)
{
	std::string l_name = soldier->GetName();
	m_Soldiers.erase(l_name);

	std::map<CMonster*, CSoldier*>::iterator l_it = m_Conects.begin();
	for(; l_it!=m_Conects.end(); l_it++)
	{
		if(soldier == l_it->second)
		{
			m_Monsters.erase(l_it->first->GetName());
			m_Conects.erase(l_it->first);
			l_it = m_Conects.end();
		}
	}
}

void CEnemyManager::EraseMonster(CMonster* monster)
{
	std::string l_name = monster->GetName();
	m_Monsters.erase(l_name);

	std::map<CMonster*, CSoldier*>::iterator l_it;
	l_it = m_Conects.find(monster);
	if(l_it != m_Conects.end())
	{
		m_Soldiers.erase(l_it->second->GetName());
		m_Conects.erase(monster);
	}

}

void CEnemyManager::CheckLightedMonsters(CSpotLight *Light)
{
	CMonster *l_Monster;
	std::map<std::string, CMonster*>::iterator it;

	for ( it=m_Monsters.begin() ; it != m_Monsters.end(); it++ )
	{
		l_Monster=(*it).second;
		if(l_Monster!=NULL && l_Monster->isLighted(Light->GetPosition(), Light->GetDirection(),Light->GetEndRangeAttenuation()))
			l_Monster->GetStateMachine()->ChangeState("ESCAPE");
	}

}


void CEnemyManager::AlertCloseSoldiers(CSoldier* soldier)
{
	std::map<std::string, CSoldier*>::iterator l_it = m_Soldiers.begin();
	for(; l_it!=m_Soldiers.end(); l_it++)
	{
		if(l_it->second!=soldier)
		{
			if(abs(l_it->second->GetPosition().y - soldier->GetPosition().y) < 2.0f)
			{
				float l_distance = l_it->second->GetPosition().Distance(soldier->GetPosition());
				if(l_distance < 20.0f)
				{
					l_it->second->GetAlarm();
				}
			}
		}
	}
}

void CEnemyManager::ResetAll()
{
	std::map<std::string, CSoldier*>::iterator l_it = m_Soldiers.begin();
	while(m_Soldiers.size() > 0)
	{
		RemoveEnemies(l_it->second);
		l_it = m_Soldiers.begin();
	}

	RemoveDeadBodies();
}

void CEnemyManager::FreezeAll()
{
	std::map<std::string, CSoldier*>::iterator l_it = m_Soldiers.begin();
	for(; l_it!=m_Soldiers.end(); l_it++)
	{
		l_it->second->Freeze();
	}
	std::map<std::string, CMonster*>::iterator l_it1 = m_Monsters.begin();
	for(; l_it1!=m_Monsters.end(); l_it1++)
	{
		l_it1->second->Freeze();
	}
}

void CEnemyManager::UnfreezeAll()
{
	std::map<std::string, CSoldier*>::iterator l_it = m_Soldiers.begin();
	for(; l_it!=m_Soldiers.end(); l_it++)
	{
		l_it->second->Unfreeze();
	}
	std::map<std::string, CMonster*>::iterator l_it1 = m_Monsters.begin();
	for(; l_it1!=m_Monsters.end(); l_it1++)
	{
		l_it1->second->Unfreeze();
	}
}

void CEnemyManager::FreezeSoldier(std::string name)
{
	std::map<std::string, CSoldier*>::iterator l_it;
	l_it = m_Soldiers.find(name);

	if(l_it != m_Soldiers.end())
	{
		l_it->second->Freeze();
	}
}

void CEnemyManager::UnfreezeSoldier(std::string name)
{
	std::map<std::string, CSoldier*>::iterator l_it;
	l_it = m_Soldiers.find(name);

	if(l_it != m_Soldiers.end())
	{
		l_it->second->Unfreeze();
	}
}

void CEnemyManager::FreezeMonster(std::string name)
{
	std::map<std::string, CMonster*>::iterator l_it;
	l_it = m_Monsters.find(name);

	if(l_it != m_Monsters.end())
	{
		l_it->second->Freeze();
	}
}

void CEnemyManager::UnfreezeMonster(std::string name)
{
	std::map<std::string, CMonster*>::iterator l_it;
	l_it = m_Monsters.find(name);

	if(l_it != m_Monsters.end())
	{
		l_it->second->Unfreeze();
	}
}

