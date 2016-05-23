#pragma once
#ifndef INC_ENEMYMANAGER_H_
#define INC_ENEMYMANAGER_H_

#include <string>
#include <map>
#include <vector>
#include "Vector3.h"

class CSoldier;
class CMonster;
class CSpotLight;
class CPhysicUserData;
class CRenderableObject;

class CEnemyManager
{
private:
	std::map <std::string, CMonster*> m_Monsters;	
	std::map <std::string, CSoldier*> m_Soldiers;
	std::map <CMonster*, CSoldier*> m_Conects;
	std::vector<CRenderableObject*> m_DeadBodies;
	float m_fTime;

public:
	CEnemyManager();
	~CEnemyManager();

	void Update(float ElapsedTime);

	void AddMonster(std::string name, CMonster* monster);
	void AddSoldier(std::string name, CSoldier* soldier);
	void SetConection(CMonster* monster, CSoldier* soldier);

	void CreateDinamicEnemy(const std::string &InstanceNameSoldier, const Vect3f &posSoldier,
		const std::string &InstanceNameMonster, const Vect3f &posMonster);

	void RepositionEnemies(bool isChangingToRealWorld);

	CMonster* GetMonster(std::string name);	
	CSoldier* GetSoldier(int id);
	CSoldier* GetSoldier(std::string name);
	CSoldier* GetSoldier(CPhysicUserData * userData);
	int GetNumSoldiers(){return m_Soldiers.size();};
	CMonster* GetMonsterConnected(CSoldier* soldier);
	CSoldier* GetSoldierConnected(CMonster* monster);

	void ComunicateTimeDead(CSoldier* soldier, float time);
	void RemoveEnemies(CSoldier* soldier);
	bool isCoverBusy(int cover);

	void EraseConnection(CMonster* monster);
	void EraseSoldier(CSoldier* soldier);
	void EraseMonster(CMonster* monster);
	void RemoveDeadBodies();

	void CheckLightedMonsters(CSpotLight *Light);
	void AlertCloseSoldiers(CSoldier* soldier);
	void ResetAll();
	void FreezeAll();
	void UnfreezeAll();
	void FreezeSoldier(std::string name);
	void UnfreezeSoldier(std::string name);
	void FreezeMonster(std::string name);
	void UnfreezeMonster(std::string name);
};

#endif INC_ENEMYMANAGER_H_