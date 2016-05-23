#pragma once
#include "Character.h"
class CXMLTreeNode;
class CStateMachine;
class CPlayer;

class CMonster :
	public CCharacter
{
public:
	CMonster();
	CMonster(const Vect3f &pos, const std::string &CoreName);
	CMonster(const Vect3f &pos, const std::string &CoreName, const std::string &Name);
	CMonster(const CXMLTreeNode& XmlData);
	~CMonster();
	
	void Update(float ElapsedTime);
	void SetStateMachine(CStateMachine* stateMachine){m_StateMachine = stateMachine;};
	void SetPosition(const Vect3f &Position);
	void SetPosition(const Vect3f & Direction, float ElapsedTime);
	void SetPlayer(CPlayer * player);
	CStateMachine * GetStateMachine(){return m_StateMachine;};
	bool isLighted(const Vect3f& pos, Vect3f dir, float distance);
	void Reload();
	void TakeDamage(float Damage){}
	void Reset();
	void SetTimeDead(float time){SetIsDead(true); m_TimeDead=time; ChangeCharacterAnimation(SHOOT_ANIM, 0.f);};
	
	void Freeze();
	void Unfreeze();

private: 	
	bool m_bFrozen;
	CStateMachine * m_StateMachine;
	Vect3f m_initPosition;
	float m_initYaw;
	float m_TimeDead;
	CPlayer* m_Player;
};

