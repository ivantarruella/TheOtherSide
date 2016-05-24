#pragma once
#include "Character.h"

#define SOLDIER_CAPSULE_HEIGHT	1.17f


class CXMLTreeNode;
class CTaskManager;
class CPlayer;

class CSoldier :
	public CCharacter
{
public:
	CSoldier();
	CSoldier(const CXMLTreeNode& XmlData);
	CSoldier(const Vect3f &pos, const std::string &CoreName, const std::string &Name);
	~CSoldier();


	void Update(float ElapsedTime);
	void SetTaskManager(CTaskManager* taskManager){m_TaskManager = taskManager;};
	void SetPosition(const Vect3f &Position);
	void SetPosition(const Vect3f & Direction, float ElapsedTime);
	void SetPlayer(CPlayer * player);
	CTaskManager * GetTaskManager(){return m_TaskManager;};
	void Reload();
	void Reset();
	void GetAlarm();
	void Freeze();
	void Unfreeze();
	bool isAlarm(){return m_Alarm;};
	void SetTimeDead(float time){m_TimeDead=time;};
	int GetCover(){return m_Cover;};
	void SetCover(int cover){m_Cover = cover;};
	void GetHurt();
	void CreateCapsuleForBullets();
	void RemoveCapsuleForBullets();

private:
	bool m_bFrozen;
	bool m_Alarm;
	CTaskManager* m_TaskManager;
	Vect3f m_initPosition;
	float m_initYaw;
	float m_TimeDead;
	int m_Cover;
	CPlayer* m_Player;
};

