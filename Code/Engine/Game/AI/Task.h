#pragma once
#ifndef INC_TASK_H_
#define INC_TASK_H_

#include <string>
#include <vector>
#include "Vector3.h"
#include "Character.h"
#include "Soldier.h"
#include "EnemyManager.h"
#include "PhysicsManager.h"
#include "Core.h"

#include "Base.h"

inline float degToDot( float degrees ) {
	float l_aux = degrees /360.0f;
	return 1 * (1 - l_aux) - 1 *l_aux;
}

inline float dotToDeg( float dot ) {
	float l_aux = (1 - dot)/2.0f;
	return l_aux * 360.0f;
}

class CTask
{
protected:
	std::string m_Name;	
	float m_Time;
	CCharacter* m_Player;
	CSoldier* m_Owner;
	
	float m_HearDistance;
	float m_VisionDistance;
	float m_VisionAngle;
	std::vector<CTask *> m_Sons; 

public:
	CTask()
		: m_HearDistance(20.0f)
		, m_VisionDistance(50.0f)
		, m_VisionAngle(140.0f)
		, m_Player(NULL)
	{};

	virtual ~CTask(){};

	virtual bool checkCondition(float ElapsedTime)=0;
	virtual void doAction(float ElapsedTime)=0;
	virtual void destroy()=0;
	virtual void Reset()=0;
	virtual void GetHurt(){};

	float getTime(){return m_Time;};
	void setTime(const float &time){m_Time = time;};

	std::string getName(){return m_Name;};
	void setName(const std::string &name){m_Name = name;};

	virtual void SetPlayer(CCharacter* player){m_Player = player;};
	void SetOwner(CSoldier* owner){m_Owner = owner;};

	void AddSon(CTask* task){m_Sons.push_back(task);};
	CTask* GetSon(int i){return m_Sons[i];};

	bool isFaced()
	{
		Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());
		l_aux.y = 0.0f;
		l_aux.Normalize();
	
		float l_vSuma = m_Owner->GetFront() * l_aux;
		if(l_vSuma < 0.94f)
			return false;
		return true;
	};

	bool isFaced(Vect3f pos)
	{
		Vect3f l_pos = m_Owner->GetPosition();
		l_pos.y = pos.y;
		Vect3f l_aux = (pos - l_pos).Normalize();
	
		float l_vSuma = m_Owner->GetFront() * l_aux;
		if(l_vSuma < 0.94f)
			return false;
		return true;
	}

	bool isAtLeft()
	{
		Vect3f l_up(0.0f, 1.0f, 0.0f);
		Vect3f l_right = l_up ^ m_Owner->GetFront();
		l_right.Normalize();
		float l_d = -(l_right * m_Owner->GetPosition());

		float v( (l_right * m_Player->GetPosition()) + l_d );

		return v <= 0.0f;
	};

	bool isAtLeft(Vect3f pos)
	{
		Vect3f l_up(0.0f, 1.0f, 0.0f);
		Vect3f l_right = l_up ^ m_Owner->GetFront();
		l_right.Normalize();
		float l_d = -(l_right * m_Owner->GetPosition());

		float v( (l_right * pos) + l_d );

		return v <= 0.0f;
	}

	bool isHeared()
	{
		Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());		
		if(l_aux.Length() > m_HearDistance)
			return false;
		return true;
	};

	bool isSeen()
	{
		Vect3f l_aux = (m_Player->GetPosition() - m_Owner->GetPosition());		
		if(l_aux.Length() > m_VisionDistance)
			return false;
		l_aux.Normalize();
		float l_vSuma = m_Owner->GetFront() * l_aux;
		if(l_vSuma < degToDot(m_VisionAngle/2.0f))
			return false;
		return true;
	};

	bool isNotBlocked(Vect3f pos)
	{
	
		CPhysicUserData * l_user;	
		CPhysicUserData * l_pla = m_Player->GetPhysicUserData();
		SCollisionInfo l_info;
		Vect3f l_fron = m_Owner->GetFront();
		Vect3f l_play = pos;
		l_play.y = m_Owner->GetPosition().y;
		Vect3f l_dir = l_play - m_Owner->GetPosition();
		float l_distance = l_dir.Length() - 0.5f;
		l_dir.Normalize();
		Vect3f l_pos = m_Owner->GetPosition();
		l_pos.y = pos.y + 1.1f;

		l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos +l_dir*0.35f, l_dir , 0x1, l_info);
		if(l_info.m_fDistance < l_distance)
		{
			return false;
		}

		return true;
	};

	bool isNotBlockedEyeLine()
	{	
		CPhysicUserData * l_user;	
		CPhysicUserData * l_pla = m_Player->GetPhysicUserData();
		SCollisionInfo l_info;
		Vect3f l_pos = m_Owner->GetPosition();
		l_pos.y = m_Player->GetPosition().y;
		Vect3f l_dir = m_Player->GetPosition() - l_pos;
		l_dir.Normalize();
		l_pos.y = m_Player->GetPosition().y + 0.6f;

		l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos + 0.35f * l_dir, l_dir , 0x1, l_info);
		if(l_user == m_Player->GetPhysicUserData())
		{
			return true;
		}
		return false;
	}

	bool isNotBlockedEyeLine(Vect3f pos1)
	{	
		CPhysicUserData * l_user;	
		CPhysicUserData * l_pla = m_Player->GetPhysicUserData();
		SCollisionInfo l_info;
		Vect3f l_pos = pos1;
		l_pos.y = m_Player->GetPosition().y;
		Vect3f l_dir = m_Player->GetPosition() - l_pos;
		l_dir.Normalize();
		l_pos.y = pos1.y + 1.1f;

		//l_user = m_Player->GetPhysicUserData();
		//l_user->SetName("Mierda");

		l_user = CORE->GetPhysicsManager()->RaycastClosestActor(l_pos, l_dir , 0x1, l_info);
			if(l_user == m_Player->GetPhysicUserData())// || l_user->GetName()=="Weapon")
			{
				return true;
			}
			/*else{			
				std::string l_name = l_user->GetName();
			}*/

		//CSoldier* l_Soldier = (CSoldier*)CORE->GetEnemyManager()->GetSoldier(l_user->GetName());
		for(int i=0; i<CORE->GetEnemyManager()->GetNumSoldiers(); i++)
		{
			CSoldier* l_Soldier = (CSoldier*)CORE->GetEnemyManager()->GetSoldier(i);
			if(l_Soldier->GetPhysicUserData() == l_user)
			{
				Vect3f l_aux = l_Soldier->GetPosition();
				l_aux.y += 1.1f;
				return isNotBlockedEyeLine(l_aux + 0.80f*l_dir);
			}
		}		

		return false;
	};
};

#endif INC_TASK_H_